/*
 * Copyright 2014-2016 Markus Prasser
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

#ifndef RECEIPTS_HANDLER_H
#define RECEIPTS_HANDLER_H

#include <QDateTime>
#include <QFile>
#include <QMessageBox>
#include <QObject>
#include <QPlainTextEdit>
#include <QTextStream>
#include <QTimer>

#include "receiptsprinter.h"

namespace lc {

//! A struct representing one payoff entry.
/*!
  This class represents a single payoff entry which will be used in the receipts creation process. Multiple instances of this will be used to represent the individual participants' outcomes.
*/
struct paymentEntry_t {QString computer; QString name; double payoff;};

//! A class to handle receipts printing.
/*!
  This class is element of every session and is used to handle the receipts printing.
*/
class ReceiptsHandler : public QObject {
    Q_OBJECT

public:
    explicit ReceiptsHandler( QPlainTextEdit *argDebugMessagesTextEdit, const QString &argZTreeDataTargetPath,
                              const bool &argPrintReceiptsForLocalClients,
                              const QString &argAnonymousReceiptsPlaceholder,
                              const QString &argLatexHeaderName, QObject *argParent = nullptr );
    explicit ReceiptsHandler( QPlainTextEdit *argDebugMessagesTextEdit, const QString &argZTreeDataTargetPath,
                              const bool &argPrintReceiptsForLocalClients,
                              const QString &argAnonymousReceiptsPlaceholder,
                              const QString &argLatexHeaderName,
                              const QString * const argDateString, QObject *argParent = nullptr );
    ~ReceiptsHandler();

signals:
    void PrintingFinished();

private slots:
    /*! Deletes the ReceiptsPrinter instance after successful printing
     */
    void DeleteReceiptsPrinterInstance();
    /*! Displays QMessageBox widgets for errors given by the ReceiptsPrinter instance
     */
    void DisplayMessageBox(QString *argErrorMessage, QString *argHeading);
    /*! Prints the receipts
     */
    void PrintReceipts();

private:
    void CreateReceiptsFromPaymentFile();
    QVector<QString> *GetParticipantsDataFromPaymentFile();
    QString *LoadLatexHeader();
    void MakeReceiptsAnonymous( QVector<paymentEntry_t*> *argDataVector, bool argAlsoAnonymizeClients );

    const QString * const anonymousReceiptsPlaceholder;         //! Placeholder which shall be inserted for participant names if anonymous printing is desired (QString != "")
    QString *dateString = nullptr;
    QPlainTextEdit * const debugMessagesTextEdit = nullptr;     //! A pointer to the programs debug_messages_text_edit to be able to emit debugging messages
    QString expectedPaymentFileName;                            //! The name of the expected payment file
    QString expectedPaymentFilePath;                            //! The path of the expected payment file
    const QString * const latexHeaderName;                      //! The name of the chosen LaTeX header template
    QFile *paymentFile = nullptr;                               //! A pointer to the '*.pay' file being watched for existance and starting the printing process
    const bool * const printReceiptsForLocalClients;            //! Stores if receipts shall be printed for local clients
    ReceiptsPrinter *receiptsPrinter = nullptr;                 //! Creates new thread for receipts printing
    QTimer *timer = nullptr;                                    //! Used for regular checking if the payment file was created
    const QString * const zTreeDataTargetPath;                  //! A reference to the data target path stored in the session class instance
};

}

#endif // RECEIPTS_HANDLER_H
