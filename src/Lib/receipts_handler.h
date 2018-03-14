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
struct paymentEntry_t {
    QString computer;
    QString name;
    double payoff;
};

class Settings;

//! A class to handle receipts printing.
/*!
  This class is element of every session and is used to handle the receipts printing.
*/
class ReceiptsHandler : public QObject
{
    Q_OBJECT

public:
    explicit ReceiptsHandler(const Settings *const argSettings,
                             const QString &argZTreeDataTargetPath,
                             bool argPrintReceiptsForLocalClients,
                             const QString &argAnonymousReceiptsPlaceholder,
                             const QString &argLatexHeaderName, QObject *argParent = nullptr);
    explicit ReceiptsHandler(const Settings *const argSettings,
                             const QString &argZTreeDataTargetPath,
                             bool argPrintReceiptsForLocalClients,
                             const QString &argAnonymousReceiptsPlaceholder,
                             const QString &argLatexHeaderName,
                             const QString &argDateString, QObject *argParent = nullptr);

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
    void MakeReceiptsAnonymous(QVector<paymentEntry_t *> *argDataVector,
                               bool argAlsoAnonymizeClients);

    //! Placeholder which shall be inserted for participant names if anonymous printing is desired (QString != "")
    const QString anonymousReceiptsPlaceholder;
    //! The expected date string of the payment file in form 'yyMMdd_hhmm'
    const QString dateString;
    //! The name of the expected payment file
    const QString expectedPaymentFileName;
    //! The path of the expected payment file
    const QString expectedPaymentFilePath;
    //! The name of the chosen LaTeX header template
    const QString latexHeaderName;
    //! A pointer to the '*.pay' file being watched for existance and starting the printing process
    QFile paymentFile;
    //! Stores if receipts shall be printed for local clients
    const bool printReceiptsForLocalClients;
    //! Creates new thread for receipts printing
    ReceiptsPrinter *receiptsPrinter = nullptr;
    const Settings *const settings = nullptr;
    //! Used for regular checking if the payment file was created
    QTimer *timer = nullptr;
    //! A reference to the data target path stored in the session class instance
    const QString zTreeDataTargetPath;
};

} // namespace lc

#endif // RECEIPTS_HANDLER_H
