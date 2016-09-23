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

#ifndef RECEIPTSPRINTER_H
#define RECEIPTSPRINTER_H

#include <QMessageBox>
#include <QProcess>
#include <QThread>

#include "global.h"

namespace lc {

//! A class for receipts creation.
/*!
  This class is used to do the actual printing of the receipts in an own thread.
*/
class ReceiptsPrinter : public QThread {
    Q_OBJECT

    void run() Q_DECL_OVERRIDE {
        // Compile the TeX file to dvi
        QStringList arguments;
        arguments << "-interaction" << "batchmode" << QString{ *dateString + ".tex" };

        QProcess *process = nullptr;
        process = new QProcess{};
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        process->setProcessEnvironment( env );
        process->setWorkingDirectory( *workpath );
        process->start( latexCmd, arguments );
        if( !process->waitForFinished( processTimeOut ) ) {
            QMessageBox message_box{ QMessageBox::Warning, "dvi creation failed", "The creation of the receipts dvi timed out after 30 seconds. Automatic receipts creation will not work.", QMessageBox::Ok };
            message_box.exec();
            delete process;
            process = nullptr;
            return;
        }
        delete process;
        process = nullptr;

        // Convert the dvi file to postscript
        arguments = QStringList{};
        arguments << "-q*" << "-o" << QString{ *dateString + ".ps" } << QString{ *dateString + ".dvi" };

        process = new QProcess{};
        process->setProcessEnvironment( env );
        process->setWorkingDirectory( *workpath );
        process->start( dvipsCmd, arguments );
        if( !process->waitForFinished( processTimeOut ) ) {
            emit ErrorOccurred(new QString{ "The conversion of the receipts dvi to postscript timed out after 30 seconds. Automatic receipts creation will not work." }, new QString{ "dvi to postscript conversion failed" } );
            delete process;
            process = nullptr;
            return;
        }
        delete process;
        process = nullptr;

        // Print the postscript file
        if ( !lprCmd.isEmpty() ) {
            arguments = QStringList{};
            arguments << QString{ *workpath + "/" + *dateString + ".ps" };

            process = new QProcess{};
            process->setProcessEnvironment( env );
            process->setWorkingDirectory( *workpath );
            process->start( lprCmd, arguments );
            if( !process->waitForFinished( processTimeOut ) ) {
                emit ErrorOccurred( new QString{ "The receipts postscript file was successfully created but could not be printed." }, new QString{ "Printing failed" } );
            }
            delete process;
            process = nullptr;
        }

        // Convert the postscript file to pdf
        if ( !ps2pdfCmd.isEmpty() ) {
            arguments = QStringList{};
            arguments << QString{ *workpath + "/" + *dateString + ".ps" } << QString{ *workpath + "/" + *dateString + ".pdf" };

            process = new QProcess{};
            process->setProcessEnvironment( env );
            process->setWorkingDirectory( *workpath );
            process->start( ps2pdfCmd, arguments );
            if( !process->waitForFinished( processTimeOut ) ) {
                emit ErrorOccurred( new QString{ "The receipts were successfully printed but the creation of the PDF file failed." }, new QString{ "PDF creation failed" } );
            }
            delete process;
            process = nullptr;

            // Show the postscript file if the conversion succeeded
            if ( !postscriptViewer.isEmpty() ) {
                arguments = QStringList{};
                arguments << QString{ *workpath + "/" + *dateString + ".ps" };

                process = new QProcess{};
                process->setProcessEnvironment( env );
                process->setWorkingDirectory( *workpath );
                process->startDetached( postscriptViewer, arguments );
                delete process;
                process = nullptr;
            }
        }

        // Clean up the zTree working path
        if ( !rmCmd.isEmpty() ) {
            arguments = QStringList{};
            arguments << QString{ *workpath + "/" + *dateString + ".aux" }
                      << QString{ *workpath + "/" + *dateString + ".dvi" }
                      << QString{ *workpath + "/" + *dateString + ".log" }
                      << QString{ *workpath + "/" + *dateString + ".tex" };

            process = new QProcess{};
            process->setProcessEnvironment( env );
            process->setWorkingDirectory( *workpath );
            process->start( rmCmd, arguments);
            if( !process->waitForFinished( processTimeOut ) ) {
                emit ErrorOccurred(new QString("The cleanup of the temporary files for receipts creation timed out. Some spare files may be left in your zTree working directory."), new QString("Cleanup failed"));
            }
            delete process;
            process = nullptr;
          }

        emit PrintingFinished();
    }
public:
    explicit ReceiptsPrinter( const QString * const argDateString,
                              const QString * const argWorkpath,
                              QObject *argParent = nullptr );

signals:
    void ErrorOccurred(QString *error_message, QString *heading);
    void PrintingFinished();

private:
    const QString * const dateString;                           //! The date string contained in the file paths
    const QString dvipsCmd;
    const QString latexCmd;
    const QString lprCmd;
    const QString postscriptViewer;
    const int processTimeOut = 15000;                           //! The maximum time which will be granted to a started process
    const QString ps2pdfCmd;
    const QString rmCmd;
    const QString vncViewer;
    const QString * const workpath;                             //! The path were zTree was ordered to store all its data
};

}

#endif // RECEIPTSPRINTER_H
