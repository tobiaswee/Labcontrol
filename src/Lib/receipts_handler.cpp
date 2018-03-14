/*
 * Copyright 2014-2018 Markus Prasser, Tobias Weiss
 *
 * This file is part of Labcontrol.
 *
 *  Labcontrol is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Labcontrol is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Labcontrol.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <memory>

#include <QDebug>

#include "receipts_handler.h"
#include "settings.h"

lc::ReceiptsHandler::ReceiptsHandler(const Settings *const argSettings,
                                     const QString &argZTreeDataTargetPath,
                                     bool argPrintReceiptsForLocalClients,
                                     const QString &argAnonymousReceiptsPlaceholder,
                                     const QString &argLatexHeaderName,
                                     QObject *argParent) :
    QObject{argParent},
    anonymousReceiptsPlaceholder{argAnonymousReceiptsPlaceholder},
    dateString{QDateTime::currentDateTime().toString("yyMMdd_hhmm")},
    expectedPaymentFileName{dateString + ".pay"},
    expectedPaymentFilePath{argZTreeDataTargetPath + "/" + dateString + ".pay"},
    latexHeaderName{argLatexHeaderName},
    paymentFile{expectedPaymentFilePath},
    printReceiptsForLocalClients{argPrintReceiptsForLocalClients},
    settings{argSettings},
    timer{new QTimer{this}},
zTreeDataTargetPath{argZTreeDataTargetPath}
{
    qDebug() << "Expected payment file name is:" << expectedPaymentFilePath;

    // Create a QTimer regularly checking if the payment file was created and print it if so
    connect(timer, &QTimer::timeout,
            this, &ReceiptsHandler::PrintReceipts);
    timer->start(2000);
}

lc::ReceiptsHandler::ReceiptsHandler(const Settings *const argSettings,
                                     const QString &argZTreeDataTargetPath,
                                     bool argPrintReceiptsForLocalClients,
                                     const QString &argAnonymousReceiptsPlaceholder,
                                     const QString &argLatexHeaderName,
                                     const QString &argDateString, QObject *argParent) :
    QObject{argParent},
    anonymousReceiptsPlaceholder{argAnonymousReceiptsPlaceholder},
    dateString{argDateString},
    expectedPaymentFileName{argDateString + ".pay"},
    expectedPaymentFilePath{argZTreeDataTargetPath + "/" + argDateString + ".pay"},
    latexHeaderName{argLatexHeaderName},
    paymentFile{expectedPaymentFilePath},
    printReceiptsForLocalClients{argPrintReceiptsForLocalClients},
    settings{argSettings},
    zTreeDataTargetPath{argZTreeDataTargetPath}
{
    qDebug() << "Expected payment file name is:" << expectedPaymentFilePath;

    PrintReceipts();
}

void lc::ReceiptsHandler::PrintReceipts()
{
    // If the payment file exists, print it
    if (paymentFile.exists()) {
        qDebug() << "The payment file has been created and will be printed";
        if (timer) {
            timer->stop();
        }

        CreateReceiptsFromPaymentFile();
    }
}

void lc::ReceiptsHandler::CreateReceiptsFromPaymentFile()
{
    // Get the data needed for receipts creation from the payment file
    QVector<QString> *rawParticipantsData = nullptr;
    rawParticipantsData = GetParticipantsDataFromPaymentFile();
    for (int i = 0; i < rawParticipantsData->size(); i++) {
        qDebug() << "Payment file line" << QString::number(i)
                 << ":\t" << rawParticipantsData->at(i);
    }

    // Extract the data of the participant's whose receipts shall be printed
    /* The tab separated fields in the payment file are:
     * SUBJECT  COMPUTER    INTERESTED  NAME    PROFIT  SIGNATURE
     */
    QVector<paymentEntry_t *> *participants = new QVector<paymentEntry_t *>;
    double overall_payoff = 0.0;
    for (QVector<QString>::iterator it = rawParticipantsData->begin();
            it != rawParticipantsData->end() - 1; ++it) {
        // Split the lines containing the participants' data into their inidivual parts
        QStringList temp_participant_data = it->split('\t', QString::KeepEmptyParts);
        qDebug() << temp_participant_data.join(" - ");
        if (!printReceiptsForLocalClients && temp_participant_data.at(3).contains("local")) {
            qDebug() << "Receipt for local client" << temp_participant_data.at(1) << "will not be printed.";
        } else {
            // Create a new struct instance for participant data and fill it
            paymentEntry_t *participant  = new paymentEntry_t;
            participant->computer = temp_participant_data.at(1);
            participant->name = temp_participant_data.at(3);
            participant->payoff = temp_participant_data.at(4).toDouble();
            overall_payoff += participant->payoff;
            participants->append(participant);
        }
    }
    delete rawParticipantsData;
    rawParticipantsData = nullptr;

    // Make receipts overview anonymous if requested (at this stage just names are removed, so that the overview still containts the client names
    if (!anonymousReceiptsPlaceholder.isEmpty()) {
        MakeReceiptsAnonymous(participants, false);
    }

    // Load the LaTeX header
    QString *latexText = LoadLatexHeader();
    if (latexText == nullptr) {
        for (auto s : *participants) {
            delete s;
        }
        delete participants;
        participants = nullptr;
        return;
    }

    // Write the comprehension table
    latexText->append("\n\\COMPREHENSION{\n");
    unsigned short int zeile = 0;
    for (auto s : *participants) {
        latexText->append(expectedPaymentFileName + " & " + s->computer + " & " + s->name + " & " +
                          QString::number(s->payoff, 'f', 2) + " \\EUR\\\\\n");
        if (zeile % 2 == 0) {
            latexText->append("\\rowcolor[gray]{0.9}\n");
        }
        ++zeile;
    }

    // MISSING: Appending show up entries to the overview

    // Make also the clients on the receipts anonymous. This is done as second step, so that the beforehand created overview still contains the clients
    if (!anonymousReceiptsPlaceholder.isEmpty()) {
        MakeReceiptsAnonymous(participants, true);
    }

    // Add the LaTeX middle sequence
    latexText->append("}{" + QString::number(overall_payoff, 'f', 2) + "}\n\n%%Einzelquittungen\n");

    // Write the single receipts
    for (auto s : *participants) {
        if (s->payoff >= 0) {
            latexText->append("\\GAINRECEIPT{" + expectedPaymentFileName + "}{" + s->computer + "}{" + s->name
                              + "}{" + QString::number(s->payoff, 'f', 2) + "}\n");
        } else {
            latexText->append("\\LOSSRECEIPT{" + expectedPaymentFileName + "}{" + s->computer + "}{" + s->name
                              + "}{" + QString::number(s->payoff, 'f', 2) + "}\n");
        }
        delete s;
    }
    delete participants;
    participants = nullptr;

    // Append LaTeX ending
    latexText->append( "\\end{document}" );

    qDebug() << *latexText;

    // Create the tex file
    QFile *texFile = new QFile{zTreeDataTargetPath + "/" + dateString + ".tex"};
    qDebug() << "Tex file" << texFile->fileName() << "will be created for receipts printing.";
    // Clean up any already existing files
    if (texFile->exists()) {
        if (!texFile->remove()) {
            QMessageBox messageBox(QMessageBox::Critical, "Tex file removing failed",
                                   "There already exists a tex file at '" + texFile->fileName()
                                   + "' which cannot be removed. The creation of the receipts printout may fail.", QMessageBox::Ok);
            messageBox.exec();
        }
    }
    // Create a new file
    if (!texFile->open(QIODevice::Text | QIODevice::WriteOnly)) {
        QMessageBox messageBox( QMessageBox::Critical, "Tex file creation failed",
                                "The creation of the tex file for receipts printing at '" + texFile->fileName()
                                + "' failed. Receipts printing will not work.", QMessageBox::Ok);
        messageBox.exec();
        return;
    }

    // Open a QTextStream to write to the file
    QTextStream out{texFile};

    out << *latexText;
    delete latexText;
    latexText = nullptr;

    receiptsPrinter = new ReceiptsPrinter{dateString, settings,
                                          zTreeDataTargetPath, this};
    receiptsPrinter->start();
    connect(receiptsPrinter, &ReceiptsPrinter::PrintingFinished,
            this, &ReceiptsHandler::DeleteReceiptsPrinterInstance);
    connect(receiptsPrinter, &ReceiptsPrinter::ErrorOccurred,
            this, &ReceiptsHandler::DisplayMessageBox);

    // Clean up
    texFile->close();
    delete texFile;
}

void lc::ReceiptsHandler::DeleteReceiptsPrinterInstance()
{
    receiptsPrinter->quit();
    receiptsPrinter->wait();
    receiptsPrinter->deleteLater();
    receiptsPrinter = nullptr;
    qDebug() << "Deleted 'ReceiptsPrinter' instance.";

    emit PrintingFinished();
}

void lc::ReceiptsHandler::DisplayMessageBox(QString *argErrorMessage,
                                            QString *argHeading)
{
    QMessageBox messageBox(QMessageBox::Warning, *argHeading,
                           *argErrorMessage, QMessageBox::Ok);
    delete argHeading;
    delete argErrorMessage;
    messageBox.exec();
}

QVector<QString> *lc::ReceiptsHandler::GetParticipantsDataFromPaymentFile()
{
    // Create the vector to store the single lines of the file
    QVector<QString> *participantsData = new QVector<QString>;

    // Open the payment file for reading and create a QTextStream
    paymentFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&paymentFile);
    in.setCodec("ISO 8859-1");

    // Read the file line by line and store them in the vector
    while (true) {
        QString line = in.readLine();
        if (line.isNull()) {
            break;
        }
        participantsData->append(line);
    }

    // Remove the first line, since it is not needed
    participantsData->erase(participantsData->begin());

    // Close the file afterwards
    paymentFile.close();

    return participantsData;
}

QString *lc::ReceiptsHandler::LoadLatexHeader()
{
    // Prepare all facilities to read the latex header file
    QFile latexHeaderFile(settings->lcDataDir + "/" + latexHeaderName + "_header.tex");
    if (!latexHeaderFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox messageBox{QMessageBox::Critical,
                               tr("LaTeX header could not be loaded"),
                               tr("The LaTeX header at '%1/%2_header.tex' could"
                                  " not be loaded. Receipts printing will not work.")
                               .arg(settings->lcDataDir).arg(latexHeaderName), QMessageBox::Ok};
        messageBox.exec();
        return nullptr;
    }
    QTextStream in(&latexHeaderFile);

    QString *header = nullptr;
    header = new QString(in.readAll());

    latexHeaderFile.close();

    return header;
}

void lc::ReceiptsHandler::MakeReceiptsAnonymous(QVector<paymentEntry_t *> *argDataVector,
                                                bool argAlsoAnonymizeClients)
{
    if (!argAlsoAnonymizeClients) {
        qDebug() << "Names are made anonymous";
        for (QVector<paymentEntry_t *>::iterator it = argDataVector->begin(); it != argDataVector->end();
                ++it) {
            (*it)->name = anonymousReceiptsPlaceholder;
        }
    } else {
        qDebug() << "Clients and names are made anonymous";
        for (QVector<paymentEntry_t *>::iterator it = argDataVector->begin(); it != argDataVector->end();
                ++it) {
            (*it)->name = anonymousReceiptsPlaceholder;
            (*it)->computer = "\\hspace{1cm}";
        }
    }
}
