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

#include <QDebug>
#include <QErrorMessage>
#include <QFile>
#include <QTextStream>

#include "lablib.h"

lc::Lablib::Lablib( QObject *argParent ) :
    QObject{ argParent },
    labSettings{ "Labcontrol", "Labcontrol", this },
    sessionsModel{ new SessionsModel{ this } }
{
    for ( const auto &s : settings->GetClients() ) {
        connect( this, &Lablib::ZLEAF_RUNNING,
                 s, &Client::SetStateToZLEAF_RUNNING );
    }
    DetectInstalledZTreeVersionsAndLaTeXHeaders();

    // Initialize all 'netstat' query mechanisms
    if ( !settings->netstatCmd.isEmpty() ) {
        netstatAgent = new NetstatAgent{ settings->netstatCmd };
        netstatAgent->moveToThread( &netstatThread );
        connect( &netstatThread, &QThread::finished, netstatAgent, &QObject::deleteLater );
        connect( netstatAgent, &NetstatAgent::QueryFinished,
                 this, &Lablib::GotNetstatQueryResult );
        netstatThread.start();
        netstatTimer = new QTimer{ this };
        connect( netstatTimer, &QTimer::timeout,
                 netstatAgent, &NetstatAgent::QueryClientConnections );
        netstatTimer->start( 500 );
    }

    // Initialize the server for client help requests retrieval
    if ( settings->clientHelpNotificationServerPort && !settings->serverIP.isEmpty() ) {
        clientHelpNotificationServer = new ClientHelpNotificationServer{ this };
    }
}

lc::Lablib::~Lablib () {
    if ( netstatTimer ) {
        netstatTimer->stop();
        delete netstatTimer;
    }
    netstatThread.quit();
    netstatThread.wait();
}

bool lc::Lablib::CheckIfUserIsAdmin() const {
    for ( const auto &s : settings->adminUsers ) {
        if ( s == settings->localUserName ) {
            qDebug() << "User" << settings->localUserName << "has administrative rights.";
            return true;
        }
    }
    return false;
}

void lc::Lablib::DetectInstalledZTreeVersionsAndLaTeXHeaders() {
}

void lc::Lablib::GotNetstatQueryResult( QStringList *argActiveZLeafConnections ) {
    if ( argActiveZLeafConnections != nullptr ) {
        for ( auto s: *argActiveZLeafConnections ) {
            // Set all given clients' statuses to 'ZLEAF_RUNNING'
            emit ZLEAF_RUNNING( s );
        }
    }
    else
        qDebug() << "Netstat status query failed.";
    delete argActiveZLeafConnections;
}

void lc::Lablib::ShowOrsee() {
    QProcess showOrseeProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    showOrseeProcess.setProcessEnvironment( env );
    QString program{ settings->browserCmd };
    QStringList arguments{ QStringList{} << settings->orseeUrl };
    showOrseeProcess.startDetached( program, arguments );

    // Output message via the debug messages tab
    qDebug() << program << arguments.join( " " );
}

void lc::Lablib::ShowPreprints() {
    // Start the process
    QProcess showPreprintsProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    showPreprintsProcess.setProcessEnvironment( env );
    QString program{ settings->fileMngr };
    QStringList arguments{ QStringList{} << settings->lcDataDir +  "/preprints" };
    showPreprintsProcess.startDetached( program, arguments );

    // Output message via the debug messages tab
    qDebug() << program << arguments.join( " " );
}

void lc::Lablib::StartNewSession( QVector< Client* > argAssocCl,
                                  QString argParticipNameReplacement,
                                  bool argPrintLocalReceipts, QString argReceiptsHeader,
                                  QString argzTreeDataTargetPath, quint16 argzTreePort,
                                  QString argzTreeVersion ) {
    if ( !QDir( argzTreeDataTargetPath ).exists() ) {
        QMessageBox messageBox{ QMessageBox::Critical, tr( "Data target path does not exist" ),
                    tr( "Your chosen data target path does not exist."
                        " Do you want it to be created automatically?" ),
                    QMessageBox::Yes | QMessageBox::No };
        messageBox.exec();
        if ( messageBox.clickedButton() == messageBox.button( QMessageBox::No ) ) {
            QMessageBox messageBox{ QMessageBox::Critical, tr( "Data target directory will not"
                                                               " be created" ),
                                    tr( "Your chosen data target directory does not exist and"
                                        " will not be created. Please choose another one." ),
                                    QMessageBox::Ok };
            messageBox.exec();
            return;
        } else {
            if ( !QDir().mkpath( argzTreeDataTargetPath ) ) {
                QMessageBox messageBox{ QMessageBox::Critical, tr( "Data target directory could"
                                                                   " not be created" ),
                                        tr( "Your chosen data target directory does not exist"
                                            " and could not be created. Please choose another"
                                            " one." ), QMessageBox::Ok };
                messageBox.exec();
                return;
            }
        }
    }
    try {
        sessionsModel->push_back( new Session{ std::move( argAssocCl ),
                                               argzTreeDataTargetPath,
                                               argzTreePort, argzTreeVersion,
                                               argPrintLocalReceipts,
                                               argParticipNameReplacement,
                                               argReceiptsHeader } );
        occupiedPorts.append( sessionsModel->back()->zTreePort );
    }
    catch ( Session::lcDataTargetPathCreationFailed ) {
        QMessageBox::information( nullptr, tr( "Chosen data target path could not be created" ),
                                  tr( "The path specified by your chosen data target path '%1'"
                                      " could not be created. Please check if it is a valid"
                                      " location and you have all needed permissions." )
                                  .arg( argzTreeDataTargetPath ) );
    }
}

void lc::Lablib::SetLocalZLeafDefaultName( const QString &argName ) {
    settings->SetLocalzLeafName( argName );
    labSettings.setValue( "local_zLeaf_name", argName );
}

//Returns the commandline that is issued on the client when zleaf is started
QStringList lc::Lablib::getzLeafArgs( int sessionPort, QString zleafVersion ){
    QStringList arguments;
    if ( sessionPort == 7000 ) {
        arguments << "DISPLAY=:0.0" << settings->tasksetCmd << "0x00000001" << settings->wineCmd
                  << QString{ settings->zTreeInstDir + "/zTree_" + zleafVersion + "/zleaf.exe" }
                  << "/server" << settings->serverIP;
    } else {
        arguments << "DISPLAY=:0.0" << settings->tasksetCmd << "0x00000001" << settings->wineCmd
                  << QString{ settings->zTreeInstDir + "/zTree_" + zleafVersion + "/zleaf.exe" }
                  << "/server" << settings->serverIP << "/channel"
                  << QString::number( sessionPort- 7000 );
    }

    //Return the crafted QStringList
    return arguments;
}
