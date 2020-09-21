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
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include "client.h"
#include "settings.h"

namespace pt = boost::property_tree;

lc::Settings::Settings( pt::ptree root ) :
    //defaultReceiptIndex{ GetDefaultReceiptIndex( argSettings ) },
    binary_paths(root.get_child("binary_paths")),
    client(root.get_child("client")),
    server(root.get_child("server")),
    webcams_root(server.get_child("webcams")),
    clientBrowserCmd(QString::fromStdString(binary_paths.get<std::string>("client_browser_command",
                                                     "Opening a browser window on clients will not work."))),
    clientChromiumCmd(QString::fromStdString(binary_paths.get<std::string>("client_chromium_command",
                                                      "Opening a chromium window on clients will not work."))),
    dvipsCmd(QString::fromStdString(binary_paths.get<std::string>("dvips_command",
                                             "Receipts creation will not work."))),
    fileMngr(QString::fromStdString(binary_paths.get<std::string>("file_manager",
                                             "The display of preprints will not work."))),
    killallCmd(QString::fromStdString(binary_paths.get<std::string>("killall_command",
                                               "Killing 'zleaf.exe' instances will not work."))),
    latexCmd(QString::fromStdString(binary_paths.get<std::string>("latex_command",
                                             "Receipts creation will not work."))),
    lcDataDir(QString::fromStdString(binary_paths.get<std::string>("labcontrol_data_directory",
                                              "Datapath not set. Labcontrol will missbehave with high propability."))),
    localUserName{ GetLocalUserName() },
    localzLeafSize(QString::fromStdString(server.get<std::string>("local_zLeaf_size",
                                             "Resolution of local zLeaf window"))),
    lprCmd(QString::fromStdString(binary_paths.get<std::string>("lpr_command",
                                           "Receipts printing will not work."))),
    netstatCmd(QString::fromStdString(binary_paths.get<std::string>("netstat_command",
                                               "Detection of active zLeaf connections will not work."))),
    netwBrdAddr(QString::fromStdString(client.get<std::string>( "network_broadcast_address",
                                            "Booting the clients will not work."))),
    orseeUrl(QString::fromStdString(server.get<std::string>("orsee_url",
                                       "Opening ORSEE in a browser will not work."))),
    pingCmd(QString::fromStdString(binary_paths.get<std::string>("ping_command",
                                            "Status updates for the clients will not work."))),
    postscriptViewer(QString::fromStdString(binary_paths.get<std::string>("postscript_viewer",
                                                     "Viewing the generated receipts postscript file will not work."))),
    ps2pdfCmd(QString::fromStdString(binary_paths.get<std::string>("ps2pdf_command",
                                              "Converting and viewing the generated receipts file will not work."))),
    pkeyPathRoot(QString::fromStdString(server.get<std::string>("pkey_path_root",
                                           "Administration actions concerning the clients will not be available."))),
    pkeyPathUser(QString::fromStdString(server.get<std::string>("pkey_path_user",
                                            "Many actions concerning the clients will not be available."))),
    rmCmd(QString::fromStdString(binary_paths.get<std::string>("rm_command",
                                          "Cleanup of the zTree data target path will not work."))),
    scpCmd(QString::fromStdString(binary_paths.get<std::string>("scp_command", "Beaming files to the clients will not be possible."))),
    serverIP(QString::fromStdString(server.get<std::string>("server_ip",
                                       "Starting zLeaves and retrieving client help messages will not work."))),
    sshCmd(QString::fromStdString(binary_paths.get<std::string>("ssh_command",
                                           "All actions concerning the clients will not be possible."))),
    tasksetCmd(QString::fromStdString(binary_paths.get<std::string>("taskset_command",
                                               "Running z-Leaves or z-Tree will be possible."))),
    termEmulCmd(QString::fromStdString(binary_paths.get<std::string>("terminal_emulator_command",
                                                "Conducting administrative tasks will not be possible."))),
    userNameOnClients(QString::fromStdString(client.get<std::string>("user_name_on_clients",
                                                "All actions concerning the clients performed by the experiment user will not work."))),
    vncViewer(QString::fromStdString(binary_paths.get<std::string>("vnc_viewer",
                                              "Viewing the clients' screens will not work."))),
    wakeonlanCmd(QString::fromStdString(binary_paths.get<std::string>("wakeonlan_command",
                                                 "Booting the clients will not work."))),
    webcamDisplayCmd(QString::fromStdString(binary_paths.get<std::string>("webcam_command",
                                                     "Displaying the laboratory's webcams will not work."))),
    webcams{ GetWebcams(server) },
    webcams_names{ GetWebcamsNames(server) },
    wineCmd(QString::fromStdString(binary_paths.get<std::string>("wine_command",
                                            "Running z-Leaves or z-Tree will be possible."))),
    wmctrlCmd(QString::fromStdString(binary_paths.get<std::string>("wmctrl_command",
                                              "Setting zTree's window title to its port number will not work."))),
    xsetCmd(QString::fromStdString(binary_paths.get<std::string>("xset_command",
                                            "Deactivating the screen saver on the clients will not be possible."))),
    zTreeInstDir(QString::fromStdString(binary_paths.get<std::string>("ztree_installation_directory",
                                                 "zTree will not be available."))),
    restartCrashedSessionScript(QString::fromStdString(binary_paths.get<std::string>("restart_crashed_session_script",
                                                                "Script to be called after session crash"))),
    //adminUsers(server.get<QStringList>("admin_users")),
    adminUsers{ GetAdminUsers(server) },
    installedLaTeXHeaders{ DetectInstalledLaTeXHeaders() },
    installedZTreeVersions{ DetectInstalledzTreeVersions() },
    //clientHelpNotificationServerPort(server.get<unsigned short>("client_help_server_port")),
    clientHelpNotificationServerPort{ GetClientHelpNotificationServerPort(server) },
    chosenzTreePort(server.get<int>("initial_port")),
    clients{ CreateClients( client, pingCmd ) },
    localzLeafName(QString::fromStdString(server.get<std::string>("local_zLeaf_def_name",
                                             "The local zLeaf default name will default to 'local'."))),
    clIPsToClMap{ CreateClIPsToClMap( clients ) }
{
    // Let the local zLeaf name default to 'local' if none was given in the settings
    if ( localzLeafName.isEmpty() ) {
        qDebug() << "'local_zLeaf_name' was not set, defaulting to 'local'";
        localzLeafName = "local";
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

QVector< lc::Client* > lc::Settings::CreateClients( const pt::ptree &clients,
                                                    const QString &argPingCmd ) { // TO DO
    QVector< Client* > tempClientVec;

    // Get the client quantity to check the value lists for clients creation for correct length
    int clientQuantity = 0;
    if ( !clients.get<int>("client_quantity") ) {
        qWarning() << "'client_quantity' was not set. The client quantity will be guessed"
                      " by the amount of client IPs set in 'client_ips'.";
        BOOST_FOREACH(const pt::ptree::value_type &v, clients.get_child("clients")) {
            clientQuantity++;
        }
        qDebug() << "'clientQuantity':" << clientQuantity;
    } else {
        // bool ok = true; // unsure if possible to keep this extra safety query or if its even necessary
        clientQuantity = clients.get<int>( "client_quantity" );
        /*if ( !ok ) {
            qWarning() << "The variable 'client_quantity' was not convertible to int";
        }*/
        qDebug() << "'clientQuantity':" << clientQuantity;
    }

    QStringList clientIPs;
    QStringList clientMACs;
    QStringList clientNames;
    QStringList clientXPositions;
    QStringList clientYPositions;

    BOOST_FOREACH(const pt::ptree::value_type &v, clients.get_child("clients")) {
        clientNames += QString::fromStdString(v.first);
        clientIPs += QString::fromStdString(clients.get_child("clients").get_child(v.first).get<std::string>("ip"));
        clientMACs += QString::fromStdString(clients.get_child("clients").get_child(v.first).get<std::string>("mac"));
        clientXPositions += QString::fromStdString(clients.get_child("clients").get_child(v.first).get<std::string>("xpos"));
        clientYPositions += QString::fromStdString(clients.get_child("clients").get_child(v.first).get<std::string>("ypos"));
    }

    if ( clientIPs.length() != clientQuantity ) {
        qWarning() << "The quantity of client ips does not match the client quantity. Client"
                      " creation will fail. No clients will be available for interaction.";
        return tempClientVec;
    }
    qDebug() << "Client IPs:" << clientIPs.join( " / " );

    if ( clientMACs.length() != clientQuantity ) {
        qWarning() << "The quantity of client macs does not match the client quantity. Client"
                      " creation will fail. No clients will be available for interaction.";
        return tempClientVec;
    }
    qDebug() << "Client MACs:" << clientMACs.join( " / " );

    if ( clientNames.length() != clientQuantity ) {
        qWarning() << "The quantity of client names does not match the client quantity. Client"
                      " creation will fail. No clients will be available for interaction.";
        return tempClientVec;
    }
    qDebug() << "Client names:" << clientNames.join( " / " );

    if ( clientXPositions.length() != clientQuantity ) {
        qWarning() << "The quantity of client x positions does not match the client quantity."
                      " Client creation will fail. No clients will be available for interaction.";
        return tempClientVec;
    }
    qDebug() << "clientXPositions:" << clientXPositions.join( " / " );

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
    if ( !lcDataDir.isEmpty() ) {
        QDir laTeXDirectory{ lcDataDir, "*_header.tex", QDir::Name,
                             QDir::CaseSensitive | QDir::Files | QDir::Readable };
        if ( !laTeXDirectory.exists() || laTeXDirectory.entryList().isEmpty() ) {
            qDebug() << "Receipts printing will not work. No LaTeX headers could be found in"
                     << lcDataDir;
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

template <typename T>
std::vector<T> as_vector(pt::ptree const& pt, pt::ptree::key_type const& key) {
    std::vector<T> r;
    for (auto& item : pt.get_child(key))
        r.push_back(item.second.get_value<T>());
    return r;
}

QStringList lc::Settings::GetAdminUsers( const pt::ptree server ) {
    // Read the list of users with administrative rights

    QStringList adminUsers;
    for (auto adminUser : as_vector<std::string>(server, "admin_users")) {
        adminUsers << QString::fromStdString(adminUser);
    }

    if ( !adminUsers.length() ) {
        qDebug() << "The 'admin_users' variable was not set."
                    " No users will be able to conduct administrative tasks.";
        return QStringList{};
    } else {
        qDebug() << "'adminUsers':" << adminUsers.join( " / " );
        return adminUsers;
    }
    //return QStringList{};
}

quint16 lc::Settings::GetClientHelpNotificationServerPort( const pt::ptree server ) {
    // Read the port the ClientHelpNotificationServer shall listen on
    quint16 clientHelpNotificationServerPort = server.get<unsigned short>( "client_help_server_port", 0 );
    if ( !clientHelpNotificationServerPort ) {
        qDebug() << "The 'client_help_server_port' variable was not set or set to zero."
                    " The ClientHelpNotificationServer will be deactivated therefore."
                    " Clients' help requests will be ignored by the server.";
        return 0;
    } else {
        qDebug() << "'clientHelpNotificationServerPort':" << clientHelpNotificationServerPort;
        return clientHelpNotificationServerPort;
    }
    //return 0;
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

/*QString lc::Settings::ReadSettingsItem( const QString &argVariableName,
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
    //return QString{};
}*/

void lc::Settings::SetLocalzLeafSize( QString arg) {
    localzLeafSize = arg;
}

void lc::Settings::SetChosenZTreePort( const int argPort ){
    chosenzTreePort = argPort;
}

QStringList lc::Settings::GetWebcams(const pt::ptree server) {
    if ( !server.count("webcams") ) {
        qDebug() << "'webcams' was not properly set. No stationary webcams will be available.";
        return QStringList{};
    } else {
        std::stringstream webcam_stream;
        QStringList webcams;
        BOOST_FOREACH(const pt::ptree::value_type &v, server.get_child("webcams")) {
            webcam_stream << v.first << ": " << &v.second << ", ";
            webcams << QString::fromStdString(v.second.get_value<std::string>());
        }
        qDebug() << "The following webcams where loaded:" << qPrintable(QString::fromStdString(webcam_stream.str()));
        return webcams;
    }
}

QStringList lc::Settings::GetWebcamsNames(const pt::ptree server) {
    if ( !server.count("webcams") ) {
        qDebug() << "'webcams' was not properly set. No stationary webcams will be available.";
        return QStringList{};
    } else {
        std::stringstream webcam_stream;
        QStringList webcam_names;
        BOOST_FOREACH(const pt::ptree::value_type &v, server.get_child("webcams")) {
            webcam_names << QString::fromStdString(v.first);
        }
        return webcam_names;
    }
}
