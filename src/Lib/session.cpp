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

#include <memory>

#include "session.h"
#include "settings.h"

extern std::unique_ptr< lc::Settings > settings;

lc::Session::Session( const QString &argZTreeDataTargetPath, const quint16 argZTreePort,
                      const QString &argZTreeVersionPath, bool argPrintReceiptsForLocalClients,
                      const QString &argAnonymousReceiptsPlaceholder,
                      const QString &argLatexHeaderName, QObject *argParent ):
    QObject{ argParent },
    zTreePort{ argZTreePort },
    anonymousReceiptsPlaceholder{ argAnonymousReceiptsPlaceholder },
    latexHeaderName{ argLatexHeaderName },
    printReceiptsForLocalClients{ argPrintReceiptsForLocalClients },
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

    if ( !settings->wmctrlCmd.isEmpty() ) {
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
    qDebug() << "New session's chosen_zTree_data_target_path:" << zTreeDataTargetPath;

    zTreeInstance = new ZTree{ zTreeDataTargetPath, zTreePort, zTreeVersionPath, this };
    connect( zTreeInstance, &ZTree::ZTreeClosed,
             this, &Session::OnzTreeClosed );
    // Only create a 'Receipts_Handler' instance, if all neccessary variables were set
    if ( latexHeaderName != "None found" && !settings->dvipsCmd.isEmpty()
         && !settings->latexCmd.isEmpty() ) {
        receiptsHandler = new ReceiptsHandler{ zTreeDataTargetPath,
                                               printReceiptsForLocalClients,
                                               anonymousReceiptsPlaceholder,
                                               latexHeaderName };
    } else {
        qDebug() << "No 'ReceiptsHandler' instance was created.";
    }
}

void lc::Session::OnzTreeClosed( int argExitCode ) {
    qDebug() << "z-Tree running on port" << zTreePort << "closed with exit code" << argExitCode;
    emit SessionFinished( this );
}

void lc::Session::RenameWindow() {
    // Example: wmctrl -r <window name> -T <new name>

    QStringList arguments;
    arguments << "-r" << "zTree - Untitled Treatment 1" << "-T" << QString{ "zTree on port " + QString::number( zTreePort ) };

    // Start the process
    QProcess renameZTreeWindowProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    renameZTreeWindowProcess.setProcessEnvironment( env );
    renameZTreeWindowProcess.startDetached( settings->wmctrlCmd, arguments );

    qDebug() << "Renamed window";

    // emit session_started();
}
