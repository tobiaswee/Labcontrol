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

#include "session.h"

lc::Session::Session( QPlainTextEdit * const argDebugMessagesTextEdit,
                      const QString &argZTreeDataTargetPath, const int argZTreePort,
                      const QString &argZTreeVersionPath, bool argPrintReceiptsForLocalClients,
                      const QString &argAnonymousReceiptsPlaceholder,
                      const QString &argLatexHeaderName,
                      const QVector<QString*> * const argSettingsItems ):
    zTreePort{ argZTreePort },
    anonymousReceiptsPlaceholder{ argAnonymousReceiptsPlaceholder },
    debugMessagesTextEdit{ argDebugMessagesTextEdit },
    latexHeaderName{ argLatexHeaderName },
    printReceiptsForLocalClients{ argPrintReceiptsForLocalClients },
    settingsItems{ argSettingsItems },
    zTreeDataTargetPath{ argZTreeDataTargetPath },
    zTreeVersionPath{ argZTreeVersionPath }
{
    // This part ensures, that both class instances are created in the same minute, so that the payment file name can be guessed correctly
    QDateTime current_time;
    current_time  = QDateTime::currentDateTime();

    // If in the last three seconds of a minute, wait for the next one to start
    if ( QTime::currentTime().second() > 56 ) {
        QTimer::singleShot( 5000, this, SLOT( InitializeClasses() ) );
    } else {
        InitializeClasses();
    }

    if ( ( *settingsItems )[ ( int )settingsItems_t::WMCTRL_COMMAND ] ) {
        QTimer::singleShot( 5000, this, SLOT( RenameWindow() ) );
    }
}

lc::Session::~Session() {
    delete receiptsHandler;
    delete zTreeInstance;
}

QVariant lc::Session::GetDataItem( int argIndex ) {
    switch ( argIndex ) {
    case 0:
        return QVariant{ zTreeVersionPath.split( '_', QString::KeepEmptyParts, Qt::CaseInsensitive )[ 1 ] };
    case 1:
        return QVariant{ zTreePort };
    default:
        return QVariant{};
    }
}

void lc::Session::InitializeClasses() {
    // Create the new data directory
    QDir dir{ zTreeDataTargetPath };
    QString date_string( QDateTime::currentDateTime().toString( "yyMMdd_hhmm" ) );
    if ( !dir.mkdir( zTreeDataTargetPath + "/" + date_string + "-" + QString::number( zTreePort ) ) ) {
        throw lcDataTargetPathCreationFailed{};
    }
    zTreeDataTargetPath.append( "/" + date_string + "-" + QString::number( zTreePort ) );
    debugMessagesTextEdit->appendPlainText( "[DEBUG] New session's chosen_zTree_data_target_path: " + zTreeDataTargetPath );

    zTreeInstance = new ZTree{ debugMessagesTextEdit, zTreeDataTargetPath,
                               zTreePort, zTreeVersionPath, settingsItems };
    // Only create a 'Receipts_Handler' instance, if all neccessary variables were set
    if ( latexHeaderName != "None found" && ( *settingsItems )[ ( int )settingsItems_t::DVIPS_COMMAND ] && ( *settingsItems )[ ( int )settingsItems_t::LATEX_COMMAND ] ) {
        receiptsHandler = new ReceiptsHandler{ debugMessagesTextEdit, zTreeDataTargetPath,
                                               printReceiptsForLocalClients,
                                               anonymousReceiptsPlaceholder,
                                               latexHeaderName, settingsItems };
    } else {
        debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] No ReceiptsHandler instance was created." ) );
    }
}

void lc::Session::RenameWindow() {
    // Example: wmctrl -r <window name> -T <new name>

    QStringList arguments;
    arguments << "-r" << "zTree - Untitled Treatment 1" << "-T" << QString{ "zTree on port " + QString::number( zTreePort ) };

    // Start the process
    QProcess renameZTreeWindowProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    renameZTreeWindowProcess.setProcessEnvironment( env );
    renameZTreeWindowProcess.startDetached( *( *settingsItems )[ ( int )settingsItems_t::WMCTRL_COMMAND ], arguments );

    debugMessagesTextEdit->appendPlainText( "[DEBUG] Renamed window" );

    // emit session_started();
}
