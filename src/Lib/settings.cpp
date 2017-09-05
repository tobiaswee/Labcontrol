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
#include <QDir>
#include <QFile>
#include <QProcessEnvironment>

#include "client.h"
#include "settings.h"

lc::Settings::Settings( const QSettings &argSettings, QObject *argParent ) :
    QObject{ argParent },
    defaultReceiptIndex{ GetDefaultReceiptIndex( argSettings ) },
    browserCmd{ ReadSettingsItem( "browser_command",
                                  "Opening ORSEE in a browser will not work.",
                                  argSettings, true ) },
    clientBrowserCmd{ ReadSettingsItem( "client_browser_command",
                                  "Opening a browser window on clients will not work.",
                                  argSettings, false ) },
    dvipsCmd{ ReadSettingsItem( "dvips_command",
                                "Receipts creation will not work.",
                                argSettings, true ) },
    fileMngr{ ReadSettingsItem( "file_manager",
                                "The display of preprints will not work.",
                                argSettings, true ) },
    killallCmd{ ReadSettingsItem( "killall_command",
                                  "Killing 'zleaf.exe' instances will not work.",
                                  argSettings, true ) },
    latexCmd{ ReadSettingsItem( "latex_command",
                                "Receipts creation will not work.",
                                argSettings, true ) },
    lcInstDir{ ReadSettingsItem( "labcontrol_installation_directory",
                                 "Labcontrol will missbehave with high propability.",
                                 argSettings, true ) },
    localUserName{ GetLocalUserName() },
    localzLeafSize{ ReadSettingsItem( "local_zLeaf_size",
                               "Resolution of local zLeaf window",
                               argSettings, false ) },
    lprCmd{ ReadSettingsItem( "lpr_command",
                              "Receipts printing will not work.",
                              argSettings, true ) },
    netstatCmd{ ReadSettingsItem( "netstat_command",
                                  "Detection of active zLeaf connections will not work.",
                                  argSettings, true ) },
    netwBrdAddr{ ReadSettingsItem( "network_broadcast_address",
                                   "Booting the clients will not work.",
                                   argSettings, false ) },
    orseeUrl{ ReadSettingsItem( "orsee_url",
                                "Opening ORSEE in a browser will not work.",
                                argSettings, false ) },
    pingCmd{ ReadSettingsItem( "ping_command",
                               "Status updates for the clients will not work.",
                               argSettings, true ) },
    postscriptViewer{ ReadSettingsItem( "postscript_viewer",
                                        "Viewing the generated receipts postscript file will not work.",
                                        argSettings, true ) },
    ps2pdfCmd{ ReadSettingsItem( "ps2pdf_command",
                                 "Converting and viewing the generated receipts file will not work.",
                                 argSettings, true ) },
    pkeyPathRoot{ ReadSettingsItem( "pkey_path_root",
                                    "Administration actions concerning the clients will not be available.",
                                    argSettings, true ) },
    pkeyPathUser{ ReadSettingsItem( "pkey_path_user",
                                    "Many actions concerning the clients will not be available.",
                                    argSettings, true ) },
    rmCmd{ ReadSettingsItem( "rm_command",
                             "Cleanup of the zTree data target path will not work.",
                             argSettings, true ) },
    scpCmd{ ReadSettingsItem( "scp_command",
                              "Beaming files to the clients will not be possible.",
                              argSettings, true ) },
    serverIP{ ReadSettingsItem( "server_ip",
                                "Starting zLeaves and retrieving client help messages will not work.",
                                argSettings, false ) },
    sshCmd{ ReadSettingsItem( "ssh_command",
                              "All actions concerning the clients will not be possible.",
                              argSettings, true ) },
    tasksetCmd{ ReadSettingsItem( "taskset_command",
                                  "Running z-Leaves or z-Tree will be possible.",
                                  argSettings, true ) },
    termEmulCmd{ ReadSettingsItem( "terminal_emulator_command",
                                   "Conducting administrative tasks will not be possible.",
                                   argSettings, true ) },
    userNameOnClients{ ReadSettingsItem( "user_name_on_clients",
                                         "All actions concerning the clients performed by the experiment user will not work.",
                                         argSettings, false ) },
    vncViewer{ ReadSettingsItem( "vnc_viewer",
                                 "Viewing the clients' screens will not work.",
                                 argSettings, true ) },
    wakeonlanCmd{ ReadSettingsItem( "wakeonlan_command",
                                    "Booting the clients will not work.",
                                    argSettings, true ) },
    webcamDisplayCmd{ ReadSettingsItem( "webcam_command",
                                        "Displaying the laboratory's webcams will not work.",
                                        argSettings, true ) },
    webcams{ argSettings.value( "webcams", "" ).toString().split( '|', QString::SkipEmptyParts,
                                                                  Qt::CaseInsensitive ) },
    wineCmd{ ReadSettingsItem( "wine_command",
                               "Running z-Leaves or z-Tree will be possible.",
                               argSettings, true ) },
    wmctrlCmd{ ReadSettingsItem( "wmctrl_command",
                                 "Setting zTree's window title to its port number will not work.",
                                 argSettings, true ) },
    xsetCmd{ ReadSettingsItem( "xset_command",
                               "Deactivating the screen saver on the clients will not be possible.",
                               argSettings, true ) },
    zTreeInstDir{ ReadSettingsItem( "ztree_installation_directory",
                                    "zTree will not be available.",
                                    argSettings, true ) },
    restartCrashedSessionScript{ ReadSettingsItem( "restart_crashed_session_script",
                               "Script to be called after session crash",
                               argSettings, false ) },
    adminUsers{ GetAdminUsers( argSettings ) },
    installedLaTeXHeaders{ DetectInstalledLaTeXHeaders() },
    installedZTreeVersions{ DetectInstalledzTreeVersions() },
    clientHelpNotificationServerPort{ GetClientHelpNotificationServerPort( argSettings ) },
    chosenzTreePort{ GetInitialPort( argSettings ) },
    clients{ CreateClients( argSettings, pingCmd ) },
    localzLeafName{ ReadSettingsItem( "local_zLeaf_name",
                                      "The local zLeaf default name will default to 'local'.",
                                      argSettings, false ) },
    clIPsToClMap{ CreateClIPsToClMap( clients ) }
{
    // Let the local zLeaf name default to 'local' if none was given in the settings
    if ( localzLeafName.isEmpty() ) {
        qDebug() << "'local_zLeaf_name' was not set, defaulting to 'local'";
        localzLeafName = "local";
    }
    if ( webcams.isEmpty() ) {
        qDebug() << "'webcams' was not properly set. No stationary webcams will be available.";
    } else {
        qDebug() << "The following webcams where loaded:" << webcams;
    }
    qDebug() << "Detected z-Tree versions" << installedZTreeVersions;
}

lc::Settings::~Settings() {
    for ( QVector< Client* >::iterator it = clients.begin(); it != clients.end(); ++it ) {
        delete *it;
    }
}

bool lc::Settings::CheckPathAndComplain( const QString &argPath, const QString &argVariableName,
                                         const QString &argMessage ) {
    if ( !QFile::exists( argPath ) ) {
        qDebug() << "The path" << argPath << "specified by" << argVariableName
                 << "does not exist:" << argMessage;
        return false;
    }
    qDebug() << argVariableName << ":" << argPath;
    return true;
}

QVector< lc::Client* > lc::Settings::CreateClients( const QSettings &argSettings,
                                                    const QString &argPingCmd ) {
    QVector< Client* > tempClientVec;

    // Get the client quantity to check the value lists for clients creation for correct length
    int clientQuantity = 0;
    if ( !argSettings.contains("client_quantity" ) ) {
        qWarning() << "'client_quantity' was not set. The client quantity will be guessed"
                      " by the amount of client IPs set in 'client_ips'.";
        clientQuantity = argSettings.value( "client_ips", "" ).toString()
                         .split( '/', QString::SkipEmptyParts, Qt::CaseSensitive ).length();
        qDebug() << "'clientQuantity':" << clientQuantity;
    } else {
        bool ok = true;
        clientQuantity = argSettings.value( "client_quantity" ).toInt( &ok );
        if ( !ok ) {
            qWarning() << "The variable 'client_quantity' was not convertible to int";
        }
        qDebug() << "'clientQuantity':" << clientQuantity;
    }

    // Create all the clients in the lab
    QStringList clientIPs = argSettings.value( "client_ips" ).toString()
                            .split( '|', QString::SkipEmptyParts, Qt::CaseSensitive );
    if ( clientIPs.length() != clientQuantity ) {
        qWarning() << "The quantity of client ips does not match the client quantity. Client"
                      " creation will fail. No clients will be available for interaction.";
        return tempClientVec;
    }
    qDebug() << "Client IPs:" << clientIPs.join( " / " );

    QStringList clientMACs = argSettings.value( "client_macs" ).toString()
                             .split( '|', QString::SkipEmptyParts, Qt::CaseSensitive );
    if ( clientMACs.length() != clientQuantity ) {
        qWarning() << "The quantity of client macs does not match the client quantity. Client"
                      " creation will fail. No clients will be available for interaction.";
        return tempClientVec;
    }
    qDebug() << "Client MACs:" << clientMACs.join( " / " );

    QStringList clientNames = argSettings.value( "client_names" ).toString()
                              .split( '|', QString::SkipEmptyParts, Qt::CaseSensitive );
    if ( clientNames.length() != clientQuantity ) {
        qWarning() << "The quantity of client names does not match the client quantity. Client"
                      " creation will fail. No clients will be available for interaction.";
        return tempClientVec;
    }
    qDebug() << "Client names:" << clientNames.join( " / " );

    QStringList clientXPositions = argSettings.value( "client_xpos" ).toString()
                                   .split( '|', QString::SkipEmptyParts, Qt::CaseSensitive );
    if ( clientXPositions.length() != clientQuantity ) {
        qWarning() << "The quantity of client x positions does not match the client quantity."
                      " Client creation will fail. No clients will be available for interaction.";
        return tempClientVec;
    }
    qDebug() << "clientXPositions:" << clientXPositions.join( " / " );

    QStringList clientYPositions = argSettings.value( "client_ypos" ).toString()
                                   .split( '|', QString::SkipEmptyParts, Qt::CaseSensitive );
    if ( clientYPositions.length() != clientQuantity ) {
        qWarning() << "The quantity of client y positions does not match the client quantity."
                      " Client creation will fail. No clients will be available for interaction.";
        return tempClientVec;
    }
    qDebug() << "clientYPositions:" << clientYPositions.join( " / " );

    for ( int i = 0; i < clientQuantity; i++ ) {
        tempClientVec.append( new Client{ clientIPs[ i ], clientMACs[ i ], clientNames[ i ],
                                          clientXPositions[ i ].toUShort(),
                                          clientYPositions[ i ].toUShort(), argPingCmd } );
    }

    return tempClientVec;
}

QMap< QString, lc::Client* > lc::Settings::CreateClIPsToClMap( const QVector< Client* > &argClients ) {
    QMap< QString, Client* > tempMap;
    for ( const auto &s : argClients ) {
        tempMap.insert( s->ip, s );

        // Get the address of the Client instance in RAM for display
        const void *clientPtrAddr = static_cast< const void* >( tempMap[ s->ip ] );

        qDebug() << "Added" << s->name
                 << "to 'clientIPsToClientsMap':" << clientPtrAddr;
    }
    return tempMap;
}

QStringList lc::Settings::DetectInstalledLaTeXHeaders() const {
    QStringList tempLaTeXHeaders{ "None found" };
    // Detect the installed LaTeX headers
    if ( !lcInstDir.isEmpty() ) {
        QDir laTeXDirectory{ lcInstDir, "*_header.tex", QDir::Name,
                             QDir::CaseSensitive | QDir::Files | QDir::Readable };
        if ( !laTeXDirectory.exists() || laTeXDirectory.entryList().isEmpty() ) {
            qDebug() << "Receipts printing will not work. No LaTeX headers could be found in"
                     << lcInstDir;
        } else {
            tempLaTeXHeaders = laTeXDirectory.entryList();
            tempLaTeXHeaders.replaceInStrings( "_header.tex", "" );
            qDebug() << "LaTeX headers:" << tempLaTeXHeaders.join( " / " );
        }
    }
    return tempLaTeXHeaders;
}

QStringList lc::Settings::DetectInstalledzTreeVersions() const {
    QStringList tempInstzTreeVersions;
    if ( !zTreeInstDir.isEmpty() ) {
        QDir zTreeDirectory{ zTreeInstDir, "zTree_*", QDir::Name,
                             QDir::NoDotAndDotDot | QDir::Dirs
                             | QDir::Readable | QDir::CaseSensitive };
        if ( zTreeDirectory.entryList().isEmpty() ) {
            qWarning() << "No zTree versions could be found in" << zTreeInstDir;
        } else {
            tempInstzTreeVersions = zTreeDirectory.entryList();
            tempInstzTreeVersions.replaceInStrings( "zTree_", "" );
        }
    }
    return tempInstzTreeVersions;
}

QStringList lc::Settings::GetAdminUsers( const QSettings &argSettings ) {
    // Read the list of users with administrative rights
    if ( !argSettings.contains( "admin_users" ) ) {
        qDebug() << "The 'admin_users' variable was not set."
                    " No users will be able to conduct administrative tasks.";
        return QStringList{};
    } else {
        QStringList adminUsers{ argSettings.value( "admin_users", "" ).toString()
                                .split( '|', QString::SkipEmptyParts, Qt::CaseInsensitive ) };
        qDebug() << "'adminUsers':" << adminUsers.join( " / " );
        return adminUsers;
    }
    return QStringList{};
}

quint16 lc::Settings::GetClientHelpNotificationServerPort( const QSettings &argSettings ) {
    // Read the port the ClientHelpNotificationServer shall listen on
    quint16 clientHelpNotificationServerPort = argSettings.value( "client_help_server_port",
                                                                  0 ).toUInt();
    if ( !clientHelpNotificationServerPort ) {
        qDebug() << "The 'client_help_server_port' variable was not set or set to zero."
                    " The ClientHelpNotificationServer will be deactivated therefore."
                    " Clients' help requests will be ignored by the server.";
        return 0;
    } else {
        qDebug() << "'clientHelpNotificationServerPort':" << clientHelpNotificationServerPort;
        return clientHelpNotificationServerPort;
    }
    return 0;
}

int lc::Settings::GetDefaultReceiptIndex( const QSettings &argSettings ) {
    // Read the default receipt index for the 'CBReceipts' combobox
    if ( !argSettings.contains( "default_receipt_index" ) ) {
        qDebug() << "'default_receipt_index' was not set. It will default to '0'.";
        return 0;
    }
    int tempIndex = argSettings.value( "default_receipt_index", 0 ).toInt();
    qDebug() << "'defaultReceiptIndex':" << tempIndex;
    return tempIndex;
}

int lc::Settings::GetInitialPort( const QSettings &argSettings ) {
    // Read the initial port number
    if ( !argSettings.contains( "initial_port" ) ) {
        qDebug() << "The 'initial_port' variable was not set."
                    " Labcontrol will default to port 7000 for new zTree instances.";
    }
    int initialPort = argSettings.value( "initial_port", 7000 ).toInt();
    qDebug() << "'initial_port':" << initialPort;
    return initialPort;
}

QString lc::Settings::GetLocalUserName() {
    const QProcessEnvironment env{ QProcessEnvironment::systemEnvironment() };
    QString userName;
    // For Linux
    if ( env.contains( "USER" ) ) {
        userName = env.value( "USER", "" );
        qDebug() << "The local user name is" << userName;
    } else if ( env.contains( "USERNAME" ) ) { // For Windows
        userName = env.value( "USERNAME", "" );
        qDebug() << "The local user name is" << userName;
    } else {
        qWarning() << "The local user name could not be queried";
    }
    return userName;
}

QString lc::Settings::ReadSettingsItem( const QString &argVariableName,
                                        const QString &argMessage,
                                        const QSettings &argSettings,
                                        bool argItemIsFile) {
    if ( !argSettings.contains( argVariableName ) ) {
        qDebug() << argVariableName << "was not set." << argMessage;
        return QString{};
    } else {
        QString tempString{ argSettings.value( argVariableName ).toString() };
        if ( argItemIsFile
             && !CheckPathAndComplain( tempString, argVariableName, argMessage ) ) {
            tempString.clear();
        }
        return tempString;
    }
    return QString{};
}
