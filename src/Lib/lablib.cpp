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

#include <QErrorMessage>
#include <QFile>
#include <QTextStream>

#include "lablib.h"

lcLablib::lcLablib( QPlainTextEdit *argDebugMessagesTextEdit, QObject *argParent ) :
    QObject{ argParent },
    clientIPsToClientsMap{ new QMap< QString, lcClient* > },
    debugMessagesTextEdit{ argDebugMessagesTextEdit },
    labSettings{ "Economic Laboratory", "Labcontrol", this },
    occupiedPorts{ new QVector< int > },
    sessionsModel{ new SessionsModel{ this } },
    settingsItems{ new QVector< QString* >{ ( int )settingsItems_t::SETTINGS_ITEMS_QUANTITY, nullptr } }
{

    ReadSettings();

    DetectInstalledZTreeVersionsAndLaTeXHeaders();

    // Initialize all 'netstat' query mechanisms
    if ( ( *settingsItems )[ ( int )settingsItems_t::NETSTAT_COMMAND ] ) {
        netstatAgent = new lcNetstatAgent{ ( *settingsItems )[ ( int )settingsItems_t::NETSTAT_COMMAND ] };
        netstatAgent->moveToThread( &netstatThread );
        connect( &netstatThread, &QThread::finished, netstatAgent, &QObject::deleteLater );
        connect( netstatAgent, &lcNetstatAgent::QueryFinished, this, &lcLablib::GotNetstatQueryResult );
        netstatThread.start();
        netstatTimer = new QTimer{ this };
        connect( netstatTimer, &QTimer::timeout, netstatAgent, &lcNetstatAgent::QueryClientConnections );
        netstatTimer->start( 500 );
    }

    // Initialize the server for client help requests retrieval
    if ( clientHelpNotificationServerPort && ( *settingsItems )[ ( int )settingsItems_t::SERVER_IP ] ) {
        clientHelpNotificationServer = new lcClientHelpNotificationServer{ clientIPsToClientsMap, ( *settingsItems )[ ( int )settingsItems_t::SERVER_IP ], clientHelpNotificationServerPort, this };
    }
}

lcLablib::~lcLablib () {
    if ( netstatTimer ) {
        netstatTimer->stop();
        delete netstatTimer;
    }
    netstatThread.quit();
    netstatThread.wait();
    delete adminUsers;
    if ( clients ) {
        for ( QVector< lcClient* >::iterator it = clients->begin(); it != clients->end(); ++it ) {
            delete *it;
        }
    }
    delete clients;
    delete InstalledZTreeVersions;
    delete occupiedPorts;
    delete webcams;
    for (auto s: *settingsItems) {
        delete s;
    }
    delete settingsItems;
}

bool lcLablib::CheckPathAndComplain( const QString * const argPath, const QString &argVariableName, const QString &argComplaint ) {
    if ( !QFile::exists( *argPath ) ) {
        QMessageBox::information( nullptr, tr( "Specified path '%1' does not exist" ).arg( argVariableName ), tr( "The path specified by '%1' does not exist. %2" ).arg( argVariableName ).arg( argComplaint ) );
        debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] The path specified by '%1' does not exist. %2" ).arg( argVariableName ).arg( argComplaint ) );
        return false;
    }
    debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] '%1': %2" ).arg( argVariableName ).arg( *argPath ) );
    return true;
}

void lcLablib::DetectInstalledZTreeVersionsAndLaTeXHeaders() {
    // Detect the installed LaTeX headers
    if ( ( *settingsItems )[ ( int )settingsItems_t::LABCONTROL_INSTALLATION_DIRECTORY ] ) {
        QDir laTeXDirectory{ *( *settingsItems )[ ( int )settingsItems_t::LABCONTROL_INSTALLATION_DIRECTORY ], "*header.tex", QDir::Name, QDir::CaseSensitive | QDir::Files | QDir::Readable };
        if ( !laTeXDirectory.exists() || laTeXDirectory.entryList().isEmpty() ) {
            QMessageBox messageBox{ QMessageBox::Critical, tr( "No LaTeX headers found" ),
                        tr( "No LaTeX headers could be found in '%1'. Receipts printing will not work" )
                        .arg( *( *settingsItems )[ ( int )settingsItems_t::LABCONTROL_INSTALLATION_DIRECTORY ] ), QMessageBox::Ok };
            messageBox.exec();
            installedLaTeXHeaders = new QStringList{ "None found" };
            debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] No LaTeX headers could be found in '%1'." )
                                                    .arg( *( *settingsItems )[ ( int )settingsItems_t::LABCONTROL_INSTALLATION_DIRECTORY ] ) );
        } else {
            installedLaTeXHeaders = new QStringList{ laTeXDirectory.entryList() };
            installedLaTeXHeaders->replaceInStrings( "_header.tex", "" );
            debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] LaTeX headers: %1" ).arg( installedLaTeXHeaders->join( " / " ) ) );
        }
    }

    // Detect the installed zTree versions
    if ( ( *settingsItems )[ ( int )settingsItems_t::ZTREE_INSTALLATION_DIRECTORY ] ) {
        QDir zTreeDirectory{ *( *settingsItems )[ ( int )settingsItems_t::ZTREE_INSTALLATION_DIRECTORY ], "zTree*", QDir::Name, QDir::NoDotAndDotDot | QDir::Dirs | QDir::Readable | QDir::CaseSensitive };
        if ( zTreeDirectory.entryList().isEmpty() ) {
            QMessageBox messageBox{ QMessageBox::Critical, tr( "zTree not found" ),
                        tr( "No zTree installation found in '%1'. Running zTree will not be possible." )
                        .arg( *( *settingsItems )[ ( int )settingsItems_t::ZTREE_INSTALLATION_DIRECTORY ] ), QMessageBox::Ok };
            messageBox.exec();

            debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] No zTree versions could be found in '%1'." ).arg( *( *settingsItems )[ ( int )settingsItems_t::ZTREE_INSTALLATION_DIRECTORY ] ) );
        }
        else {
            InstalledZTreeVersions = new QStringList{ zTreeDirectory.entryList() };
            InstalledZTreeVersions->replaceInStrings( "zTree_", "" );

            debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] zTree versions: %1" ).arg( InstalledZTreeVersions->join( " / " ) ) ) ;
        }
    }
}

void lcLablib::GotNetstatQueryResult( QStringList *argActiveZLeafConnections ) {
    if ( argActiveZLeafConnections != nullptr ) {
        for ( auto s: *argActiveZLeafConnections ) {
            // Set all given clients' statuses to 'ZLEAF_RUNNING'
            emit ZLEAF_RUNNING( s );
        }
    }
    else
        debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] Netstat status query failed." ) );
    delete argActiveZLeafConnections;
}

void lcLablib::ReadSettings() {
    QStringList simpleLoadableItems = { QStringList{}
                                         << "dvips_command"
                                         << "file_manager"
                                         << "labcontrol_installation_directory"
                                         << "latex_command"
                                         << "local_zLeaf_name"
                                         << "lpr_command"
                                         << "netstat_command"
                                         << "network_broadcast_address"
                                         << "orsee_command"
                                         << "ping_command"
                                         << "postscript_viewer"
                                         << "ps2pdf_command"
                                         << "public_key_path_root"
                                         << "public_key_path_user"
                                         << "rcp_command"
                                         << "rm_command"
                                         << "server_ip"
                                         << "ssh_command"
                                         << "terminal_emulator_command"
                                         << "user_name_on_clients"
                                         << "vnc_viewer"
                                         << "wakeonlan_command"
                                         << "wmctrl_command"
                                         << "xset_command"
                                         << "ztree_installation_directory" };

    QStringList theItemsErrorComplaints = { QStringList{}
                                         << "Receipts creation will not work."
                                         << "The display of preprints will not work."
                                         << "Labcontrol will missbehave with high propability."
                                         << "Receipts creation will not work."
                                         << "The local zLeaf default name will default to 'local'."
                                         << "Receipts printing will not work."
                                         << "Detection of active zLeaf connections will not work."
                                         << "Booting the clients will not work."
                                         << "Opening ORSEE in a browser will not work."
                                         << "Status updates for the clients will not work."
                                         << "Viewing the generated receipts postscript file will not work."
                                         << "Converting and viewing the generated receipts file will not work."
                                         << "Administration actions concerning the clients will not be available."
                                         << "Many actions concerning the clients will not be available."
                                         << "Beaming files to the clients will not be possible."
                                         << "Cleanup of the zTree data target path will not work."
                                         << "Starting zLeaves and retrieving client help messages will not work."
                                         << "All actions concerning the clients will not be possible."
                                         << "Conducting administrative tasks will not be possible."
                                         << "All actions concerning the clients performed by the experiment user will not work."
                                         << "Viewing the client's screens will not work."
                                         << "Booting the clients will not work."
                                         << "Setting zTree's window title to its port number will not work."
                                         << "Deactivating the screen saver on the clients will not be possible."
                                         << "zTree will not be available" };

    bool is_file[] = { true, true, true, true, false, true, true, false, false, true, true, true, true,
                       true, true, true, false, true, true, false, true, true, true, false, true };

    QString *tempItemStorage = nullptr;
    for ( int i = 0; i < ( int )settingsItems_t::SETTINGS_ITEMS_QUANTITY; i++ ) {
        tempItemStorage = ReadSettingsItem( simpleLoadableItems[ i ], theItemsErrorComplaints[ i ], is_file[ i ] );
        settingsItems->replace( i, tempItemStorage );
    }
    tempItemStorage = nullptr;

    // Let the local zLeaf name default to 'local' if none was given in the settings
    if ( !( *settingsItems )[ ( int )settingsItems_t::LOCAL_ZLEAF_NAME ] ) {
        settingsItems->replace( ( int )settingsItems_t::LOCAL_ZLEAF_NAME, new QString{ tr( "local" ) } );
    }

    // Read the list of users with administrative rights
    if ( !labSettings.contains( "admin_users" ) ) {
        QMessageBox messageBox{ QMessageBox::Information, tr( "'admin_users' not set" ),
                    tr( "The 'admin_users' variable was not set. No users will be able to conduct administrative tasks." ) };
        messageBox.exec();
        debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] 'admin_users' was not set. No permission for administrative tasks." ) );
    } else {
        adminUsers = new QStringList{ labSettings.value( "admin_users", "" ).toString().split( '|', QString::SkipEmptyParts, Qt::CaseInsensitive ) };
        debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] 'admin_users': %1").arg( adminUsers->join(" / ") ) );
    }

    // Read the port the ClientHelpNotificationServer shall listen on
    clientHelpNotificationServerPort = labSettings.value( "client_help_server_port", 0 ).toUInt();
    if ( !clientHelpNotificationServerPort ) {
        QMessageBox messageBox{ QMessageBox::Information, tr( "The ClientHelpNotificationServer will be deactivated" ),
                               tr( "The 'client_help_server_port' variable was not set or set to zero. The ClientHelpNotificationServer will be deactivated. Clients' help requests will be ignored by the server." ) };
        messageBox.exec();
        debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] The ClientHelpNotificationServer will be deactivated since 'client_help_server_port' was not set or set to zero." ) );
    } else {
        debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] 'client_help_server_port': %1" ).arg( QString::number( clientHelpNotificationServerPort ) ) );
    }

    // Read the default receipt index for the 'CBReceipts' combobox
    if ( !labSettings.contains( "default_receipt_index" ) ) {
        QMessageBox messageBox(QMessageBox::Information, tr( "'default_receipt_index' not set" ), tr( "The 'default_receipt_index' variable was not set. It will default to '0'." ) );
        messageBox.exec();
        debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] 'default_receipt_index' was not set. It will default to '0'." ) );
    }
    defaultReceiptIndex = labSettings.value( "default_receipt_index", 0 ).toInt();
    debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] 'default_receipt_index': %1").arg( QString::number( defaultReceiptIndex ) ) );

    // Read the initial port number
    if ( !labSettings.contains( "initial_port" ) ) {
        QMessageBox messageBox{ QMessageBox::Information, tr( "'initial_port' not set" ),
                    tr( "The 'initial_port' variable was not set. Labcontrol will default to port 7000 for new zTree instances." ) };
        messageBox.exec();
        debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] 'initial_port' was not set. Labcontrol will default to port 7000 for new zTree instances." ) );
    }
    chosenZTreePort = labSettings.value( "initial_port", 7000 ).toInt();
    debugMessagesTextEdit->appendPlainText( tr("[DEBUG] 'initial_port': %1" ).arg( QString::number( chosenZTreePort ) ) );

    // Get a list of available webcams in the lab
    if ( !labSettings.contains( "webcams" ) ) {
        QMessageBox messageBox{ QMessageBox::Information, tr( "'webcams' not set" ), tr( "The 'webcams' variable was not set. No stationary webcams will be available." ) };
        messageBox.exec();
        debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] 'webcams' was not set. No stationary webcams will be available." ) );
    }
    else {
        webcams = new QStringList{ labSettings.value( "webcams" ).toString().split( '|', QString::SkipEmptyParts, Qt::CaseInsensitive ) };
        debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] 'webcams': %1" ).arg( webcams->join( " / " ) ) );
    }

    // Get the client quantity to check the value lists for clients creation for correct length
    int clientQuantity = 0;
    if ( !labSettings.contains("client_quantity" ) ) {
        QMessageBox messageBox{ QMessageBox::Information, tr( "'client_quantity' not set" ),
                    tr( "The 'client_quantity' variable was not set. The client quantity will be guessed by the amount of client ips set in 'client_ips'." ) };
        messageBox.exec();
        debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] 'client_quantity' was not set. The client quantity will be guessed by the amount of client IPs set in 'client_ips'." ) );
        clientQuantity = labSettings.value( "client_ips", "" ).toString().split( '/', QString::SkipEmptyParts, Qt::CaseSensitive ).length();
        debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] 'client_quantity': %1" ).arg( QString::number( clientQuantity ) ) );
    } else {
        clientQuantity = labSettings.value( "client_quantity" ).toInt();
        debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] 'client_quantity': %1" ).arg( QString::number( clientQuantity ) ) );
    }

    // Create all the clients in the lab
    QStringList clientIPs = labSettings.value( "client_ips" ).toString().split( '|', QString::SkipEmptyParts, Qt::CaseSensitive );
    if ( clientIPs.length() != clientQuantity ) {
        QMessageBox messageBox{ QMessageBox::Critical, tr( "Wrong client ip quantity" ),
                    tr( "The quantity of client ips does not match the client quantity. Client creation will fail. No clients will be available for interaction." ), QMessageBox::Ok };
        messageBox.exec();
        return;
    }
    debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] Client IPs: %1").arg( clientIPs.join( " / " ) ) );
    QStringList clientMACs = labSettings.value( "client_macs" ).toString().split( '|', QString::SkipEmptyParts, Qt::CaseSensitive );
    if ( clientMACs.length() != clientQuantity ) {
        QMessageBox messageBox{ QMessageBox::Critical, tr( "Wrong client mac quantity" ),
                    tr( "The quantity of client macs does not match the client quantity. Client creation will fail. No clients will be available for interaction." ), QMessageBox::Ok };
        messageBox.exec();
        return;
    }
    debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] Client MACs: %1").arg( clientMACs.join( " / " ) ) );
    QStringList clientNames = labSettings.value( "client_names" ).toString().split( '|', QString::SkipEmptyParts, Qt::CaseSensitive );
    if ( clientNames.length() != clientQuantity ) {
        QMessageBox messageBox{ QMessageBox::Critical, tr( "Wrong client name quantity" ),
                    tr( "The quantity of client names does not match the client quantity. Client creation will fail. No clients will be available for interaction." ), QMessageBox::Ok };
        messageBox.exec();
        return;
    }
    debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] Client names: %1" ).arg( clientNames.join( " / " ) ) );
    QStringList clientWebcams = labSettings.value( "client_webcams" ).toString().split( '|', QString::SkipEmptyParts, Qt::CaseSensitive );
    if ( clientWebcams.length() != clientQuantity ) {
        QMessageBox messageBox{ QMessageBox::Critical, tr( "Wrong client webcam property quantity" ),
                    tr( "The quantity of client webcam properties does not match the client quantity. Client creation will fail. No clients will be available for interaction." ), QMessageBox::Ok };
        messageBox.exec();
        return;
    }
    debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] Client webcams: %1").arg( clientWebcams.join( " / " ) ) );
    QStringList clientXPositions = labSettings.value( "client_xpos" ).toString().split( '|', QString::SkipEmptyParts, Qt::CaseSensitive );
    if ( clientXPositions.length() != clientQuantity ) {
        QMessageBox messageBox{ QMessageBox::Critical, tr( "Wrong client x positions quantity" ),
                    tr( "The quantity of client x positions does not match the client quantity. Client creation will fail. No clients will be available for interaction." ), QMessageBox::Ok };
        messageBox.exec();
        return;
    }
    debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] client_xpos: %1" ).arg( clientXPositions.join( " / " ) ) );
    QStringList clientYPositions = labSettings.value( "client_ypos" ).toString().split( '|', QString::SkipEmptyParts, Qt::CaseSensitive );
    if ( clientYPositions.length() != clientQuantity ) {
        QMessageBox messageBox{ QMessageBox::Critical, tr( "Wrong client y positions quantity" ),
                    tr( "The quantity of client y positions does not match the client quantity. Client creation will fail. No clients will be available for interaction." ), QMessageBox::Ok };
        messageBox.exec();
        return;
    }
    debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] client_ypos: %1" ).arg( clientYPositions.join( " / " ) ) );

    clients = new QVector< lcClient* >;
    for ( int i = 0; i < clientQuantity; i++ ) {
        clients->append( new lcClient{ debugMessagesTextEdit, &clientIPs[ i ], &clientMACs[ i ], &clientNames[ i ], clientXPositions[ i ].toUShort(),
                                       clientYPositions[ i ].toUShort(), clientWebcams[ i ].toInt(), settingsItems } );

        // Add an corresponding entry to the 'client_ips_to_clients_map' std::map<QString, Client*>
        ( *clientIPsToClientsMap )[ clients->last()->ip ] = clients->last();

        // Get the address of the Client instance in RAM for display
        const void *clientPointerAddress = static_cast< const void* >( ( *clientIPsToClientsMap )[ clients->last()->ip ] );
        QString clientPointerAddressString;
        QTextStream clientPointerAddressStream ( &clientPointerAddressString );
        clientPointerAddressStream << clientPointerAddress;

        // Connect the 'Client' instance to the 'ZLEAF_RUNNING(QString client_ip)' signal
        connect( this, &lcLablib::ZLEAF_RUNNING, clients->last(), &lcClient::SetStateToZLEAF_RUNNING );

        debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] Added '%1' to 'client_ips_to_clients_map': '%2'" ).arg( clients->last()->name ).arg( clientPointerAddressString ) );
    }
}

QString *lcLablib::ReadSettingsItem( const QString &argVariableName, const QString &argComplaint, bool argItemIsFile ) {
    if ( !labSettings.contains( argVariableName ) ) {
        QMessageBox::information( nullptr, tr( "'%1' not set" ).arg( argVariableName ), tr( "The '%1' variable was not set. %2" ).arg( argVariableName ).arg( argComplaint ) );
        debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] '%1' was not set. %2" ).arg( argVariableName ).arg( argComplaint ) );

        return nullptr;
    }
    else {
        QString *tempString = new QString{ labSettings.value( argVariableName ).toString() };
        if ( argItemIsFile && !CheckPathAndComplain( tempString, argVariableName, argComplaint ) ) {
            delete tempString;
            tempString = nullptr;
        }
        // Empty strings count as not set, so delete them for correct error handling in the rest of Labcontrol
        if ( tempString && tempString->isEmpty() ) {
            delete tempString;
            tempString = nullptr;
        }
        return tempString;
    }
    return nullptr;
}

void lcLablib::SetAnonymousReceiptsPlaceholder( const QString &argPlaceHolder ) {
    anonymousReceiptsPlaceholder = argPlaceHolder;
    debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] anonymous_receipts_placeholder set to: '%1'" ).arg( anonymousReceiptsPlaceholder ) );
}

void lcLablib::SetChosenLaTeXHeader( const QString &argLatexHeader ) {
    chosenLaTeXHeader = argLatexHeader;
    debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] chosen_latex_header set to: '%1'" ).arg( chosenLaTeXHeader ) );
}

void lcLablib::SetChosenZTreeDataTargetPath( const QString &argZTreeDataTargetPath ) {
    chosenZTreeDataTargetPath = argZTreeDataTargetPath;
    debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] chosen_zTree_data_target_path set to: '%1'" ).arg( chosenZTreeDataTargetPath ) );
}

void lcLablib::SetChosenZTreePort( const int &argPort ) {
    chosenZTreePort = argPort;
    debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] chosen_zTree_port set to: '%1'" ).arg( QString::number( chosenZTreePort ) ) );
}

void lcLablib::SetChosenZTreeVersion( const QString &argZTreeVersion ) {
    chosenZTreeVersion = QString{ "zTree_" + argZTreeVersion };
    debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] chosen_zTree_version set to: '%1'" ).arg( chosenZTreeVersion ) );
}

void lcLablib::SetPrintReceiptsForLocalClients( const bool &argPrintReceiptsForLocalClients ) {
    PrintReceiptsForLocalClients = argPrintReceiptsForLocalClients;
    debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] Set print_receipts_for_local_clients to : '%1'" ).arg( QString::number( PrintReceiptsForLocalClients ) ) );
}

void lcLablib::SetUserNameOnServer( const QString &argUserName ) {
    userNameOnServer = argUserName;
    debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] user_name_on_server set to: '%1'" ).arg( userNameOnServer ) );
}

void lcLablib::ShowOrsee() {
    // Start the process
    QProcess showOrseeProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    showOrseeProcess.setProcessEnvironment( env );
    showOrseeProcess.startDetached( *( *settingsItems )[ ( int )settingsItems_t::ORSEE_COMMAND ] );

    // Output message via the debug messages tab
    debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] %1" ).arg( *( *settingsItems )[ ( int )settingsItems_t::ORSEE_COMMAND ] ) );
}

void lcLablib::ShowPreprints() {
    // Start the process
    QProcess showPreprintsProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    showPreprintsProcess.setProcessEnvironment( env );
    QString program{ *( *settingsItems )[ ( int )settingsItems_t::FILE_MANAGER ] };
    QStringList arguments{ QStringList{} << *( *settingsItems )[ ( int )settingsItems_t::LABCONTROL_INSTALLATION_DIRECTORY ] +  "/preprints" };
    showPreprintsProcess.startDetached( program, arguments );

    // Output message via the debug messages tab
    debugMessagesTextEdit->appendPlainText( tr( "[DEBUG] %1 %2" ).arg( program ).arg( arguments.join( " " ) ) );
}

void lcLablib::StartNewZTreeInstance() {
    if ( !QDir( chosenZTreeDataTargetPath ).exists() ) {
        QMessageBox messageBox{ QMessageBox::Critical, tr( "Data target path does not exist" ),
                    tr( "Your chosen data target path does not exist. Do you want it to be created automatically?" ), QMessageBox::Yes | QMessageBox::No };
        messageBox.exec();
        if ( messageBox.clickedButton() == messageBox.button( QMessageBox::No ) ) {
            QMessageBox messageBox{ QMessageBox::Critical, tr( "Data target directory will not be created" ),
                        tr( "Your chosen data target directory does not exist and will not be created. Please chose another one." ), QMessageBox::Ok };
            messageBox.exec();
            return;
        }
        else {
            if ( !QDir().mkpath( chosenZTreeDataTargetPath ) ) {
                QMessageBox messageBox{ QMessageBox::Critical, tr( "Data target directory could not be created" ),
                            tr( "Your chosen data target directory does not exist and could not be created. Please chose another one." ), QMessageBox::Ok };
                messageBox.exec();
                return;
            }
        }
    }
    try {
        sessionsModel->push_back( new lcSession{ debugMessagesTextEdit, chosenZTreeDataTargetPath, chosenZTreePort, chosenZTreeVersion,
                                                 PrintReceiptsForLocalClients, anonymousReceiptsPlaceholder, chosenLaTeXHeader, settingsItems } );
        occupiedPorts->append( sessionsModel->back()->zTreePort );
    }
    catch ( lcSession::lcDataTargetPathCreationFailed ) {
        QMessageBox::information( nullptr, tr( "Chosen data target path could not be created" ),
                                  tr( "The path specified by your chosen data target path '%1' could not be created. Please check if it is a valid location and you have all needed permissions." )
                                  .arg( chosenZTreeDataTargetPath ) );
    }
}

void lcLablib::SetLocalZLeafDefaultName( const QString &argName ) {
    delete ( *settingsItems )[ ( int )settingsItems_t::LOCAL_ZLEAF_NAME ];
    ( *settingsItems )[ ( int )settingsItems_t::LOCAL_ZLEAF_NAME ] = new QString{ argName };
    labSettings.setValue( "local_zLeaf_name", argName );
}