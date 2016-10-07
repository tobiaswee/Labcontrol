#include <QDebug>
#include <QDir>
#include <QFile>
#include <QProcessEnvironment>

#include "settings.h"

lc::Settings::Settings( const QSettings &argSettings, QObject *argParent ) :
    QObject{ argParent },
    browserCmd{ ReadSettingsItem( "browser_command",
                                  "Opening ORSEE in a browser will not work.",
                                  argSettings, true ) },
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
    installedZTreeVersions{ DetectInstalledzTreeVersions() },
    localzLeafName{ ReadSettingsItem( "local_zLeaf_name",
                                      "The local zLeaf default name will default to 'local'.",
                                      argSettings, false ) }
{
    qDebug() << "The following webcams where loaded:" << webcams;
    qDebug() << "Detected z-Tree versions" << installedZTreeVersions;
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
