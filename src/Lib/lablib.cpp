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
    clientIPsToClientsMap{ new QMap< QString, Client* > },
    labSettings{ "Economic Laboratory", "Labcontrol", this },
    occupiedPorts{ new QVector< int > },
    sessionsModel{ new SessionsModel{ this } }
{
    ReadSettings();

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
        clientHelpNotificationServer = new ClientHelpNotificationServer{ clientIPsToClientsMap,
                                                                         this };
    }
}

lc::Lablib::~Lablib () {
    if ( netstatTimer ) {
        netstatTimer->stop();
        delete netstatTimer;
    }
    netstatThread.quit();
    netstatThread.wait();
    if ( clients ) {
        for ( QVector< Client* >::iterator it = clients->begin(); it != clients->end(); ++it ) {
            delete *it;
        }
    }
    delete clients;
    delete occupiedPorts;
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

void lc::Lablib::ReadSettings() {
    // Get the client quantity to check the value lists for clients creation for correct length
    int clientQuantity = 0;
    if ( !labSettings.contains("client_quantity" ) ) {
        QMessageBox messageBox{ QMessageBox::Information, tr( "'client_quantity' not set" ),
                    tr( "The 'client_quantity' variable was not set. The client quantity will be guessed by the amount of client ips set in 'client_ips'." ) };
        messageBox.exec();
        qDebug() << "'client_quantity' was not set. The client quantity will be guessed"
                    " by the amount of client IPs set in 'client_ips'.";
        clientQuantity = labSettings.value( "client_ips", "" ).toString().split( '/', QString::SkipEmptyParts, Qt::CaseSensitive ).length();
        qDebug() << "'clientQuantity':" << clientQuantity;
    } else {
        clientQuantity = labSettings.value( "client_quantity" ).toInt();
        qDebug() << "'clientQuantity':" << clientQuantity;
    }

    // Create all the clients in the lab
    QStringList clientIPs = labSettings.value( "client_ips" ).toString().split( '|', QString::SkipEmptyParts, Qt::CaseSensitive );
    if ( clientIPs.length() != clientQuantity ) {
        QMessageBox messageBox{ QMessageBox::Critical, tr( "Wrong client ip quantity" ),
                    tr( "The quantity of client ips does not match the client quantity. Client creation will fail. No clients will be available for interaction." ), QMessageBox::Ok };
        messageBox.exec();
        return;
    }
    qDebug() << "Client IPs:" << clientIPs.join( " / " );
    QStringList clientMACs = labSettings.value( "client_macs" ).toString().split( '|', QString::SkipEmptyParts, Qt::CaseSensitive );
    if ( clientMACs.length() != clientQuantity ) {
        QMessageBox messageBox{ QMessageBox::Critical, tr( "Wrong client mac quantity" ),
                    tr( "The quantity of client macs does not match the client quantity. Client creation will fail. No clients will be available for interaction." ), QMessageBox::Ok };
        messageBox.exec();
        return;
    }
    qDebug() << "Client MACs:" << clientMACs.join( " / " );
    QStringList clientNames = labSettings.value( "client_names" ).toString().split( '|', QString::SkipEmptyParts, Qt::CaseSensitive );
    if ( clientNames.length() != clientQuantity ) {
        QMessageBox messageBox{ QMessageBox::Critical, tr( "Wrong client name quantity" ),
                    tr( "The quantity of client names does not match the client quantity. Client creation will fail. No clients will be available for interaction." ), QMessageBox::Ok };
        messageBox.exec();
        return;
    }
    qDebug() << "Client names:" << clientNames.join( " / " );
    QStringList clientXPositions = labSettings.value( "client_xpos" ).toString().split( '|', QString::SkipEmptyParts, Qt::CaseSensitive );
    if ( clientXPositions.length() != clientQuantity ) {
        QMessageBox messageBox{ QMessageBox::Critical, tr( "Wrong client x positions quantity" ),
                    tr( "The quantity of client x positions does not match the client quantity. Client creation will fail. No clients will be available for interaction." ), QMessageBox::Ok };
        messageBox.exec();
        return;
    }
    qDebug() << "clientXPositions:" << clientXPositions.join( " / " );
    QStringList clientYPositions = labSettings.value( "client_ypos" ).toString().split( '|', QString::SkipEmptyParts, Qt::CaseSensitive );
    if ( clientYPositions.length() != clientQuantity ) {
        QMessageBox messageBox{ QMessageBox::Critical, tr( "Wrong client y positions quantity" ),
                    tr( "The quantity of client y positions does not match the client quantity. Client creation will fail. No clients will be available for interaction." ), QMessageBox::Ok };
        messageBox.exec();
        return;
    }
    qDebug() << "clientYPositions:" << clientYPositions.join( " / " );

    clients = new QVector< Client* >;
    for ( int i = 0; i < clientQuantity; i++ ) {
        clients->append( new Client{ clientIPs[ i ], clientMACs[ i ], clientNames[ i ],
                                     clientXPositions[ i ].toUShort(),
                                     clientYPositions[ i ].toUShort() } );

        // Add an corresponding entry to the 'client_ips_to_clients_map' std::map<QString, Client*>
        ( *clientIPsToClientsMap )[ clients->last()->ip ] = clients->last();

        // Get the address of the Client instance in RAM for display
        const void *clientPointerAddress = static_cast< const void* >( ( *clientIPsToClientsMap )[ clients->last()->ip ] );

        // Connect the 'Client' instance to the 'ZLEAF_RUNNING(QString client_ip)' signal
        connect( this, &Lablib::ZLEAF_RUNNING,
                 clients->last(), &Client::SetStateToZLEAF_RUNNING );

        qDebug() << "Added" << clients->last()->name
                 << "to 'clientIPsToClientsMap':" << clientPointerAddress;
    }
}

void lc::Lablib::SetPrintReceiptsForLocalClients( const bool &argPrintReceiptsForLocalClients ) {
    PrintReceiptsForLocalClients = argPrintReceiptsForLocalClients;
    qDebug() << "Set 'PrintReceiptsForLocalClients' to:" << PrintReceiptsForLocalClients;
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
    QStringList arguments{ QStringList{} << settings->lcInstDir +  "/preprints" };
    showPreprintsProcess.startDetached( program, arguments );

    // Output message via the debug messages tab
    qDebug() << program << arguments.join( " " );
}

void lc::Lablib::StartNewZTreeInstance( QString argDataTargetPath, int argPort,
                                        QString argzTreeVersion,
                                        bool argReceiptsForLocalClients,
                                        QString argAnonReceiptPlaceholder,
                                        QString argChosenLatexHeader ) {
    if ( !QDir( argDataTargetPath ).exists() ) {
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
            if ( !QDir().mkpath( argDataTargetPath ) ) {
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
        sessionsModel->push_back( new Session{ argDataTargetPath,
                                               argPort, argzTreeVersion,
                                               argReceiptsForLocalClients,
                                               argAnonReceiptPlaceholder,
                                               argChosenLatexHeader } );
        occupiedPorts->append( sessionsModel->back()->zTreePort );
    }
    catch ( Session::lcDataTargetPathCreationFailed ) {
        QMessageBox::information( nullptr, tr( "Chosen data target path could not be created" ),
                                  tr( "The path specified by your chosen data target path '%1'"
                                      " could not be created. Please check if it is a valid"
                                      " location and you have all needed permissions." )
                                  .arg( argDataTargetPath ) );
    }
}

void lc::Lablib::SetLocalZLeafDefaultName( const QString &argName ) {
    settings->SetLocalzLeafName( argName );
    labSettings.setValue( "local_zLeaf_name", argName );
}
