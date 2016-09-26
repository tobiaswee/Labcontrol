#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

namespace lc {

class Settings : public QObject {
    Q_OBJECT

public:
    Settings() = delete;
    explicit Settings( const QSettings &argSettings, QObject *argParent = nullptr );
    Settings( const Settings &argSettings ) = delete;
    Settings& operator=( const Settings &argSettings ) = delete;
    Settings( Settings &&argSettings ) = delete;
    Settings& operator=( Settings &&argSettings ) = delete;

    QString GetLocalzLeafName() const;
    void SetLocalzLeafName( const QString &argLocalzLeafName );

    const QString browserCmd;
    const QString dvipsCmd;
    const QString fileMngr;
    const QString latexCmd;
    const QString lcInstDir;
    const QString localUserName;
    const QString lprCmd;
    const QString netstatCmd;
    const QString netwBrdAddr;
    const QString orseeUrl;
    const QString pingCmd;
    const QString postscriptViewer;
    const QString ps2pdfCmd;
    const QString pkeyPathRoot;
    const QString pkeyPathUser;
    const QString rmCmd;
    const QString scpCmd;
    const QString serverIP;
    const QString sshCmd;
    const QString tasksetCmd;
    const QString termEmulCmd;
    const QString userNameOnClients;
    const QString vncViewer;
    const QString wakeonlanCmd;
    const QStringList webcams;
    const QString wineCmd;
    const QString wmctrlCmd;
    const QString xsetCmd;
    const QString zTreeInstDir;

private:
    static bool CheckPathAndComplain( const QString &argPath, const QString &argVariableName,
                                      const QString &argMessage );
    static QString GetLocalUserName();
    static QString ReadSettingsItem( const QString &argVariableName,
                                     const QString &argMessage,
                                     const QSettings &argSettings,
                                     bool argItemIsFile );

    QString localzLeafName;
};

}

inline QString lc::Settings::GetLocalzLeafName() const {
    return localzLeafName;
}

inline void lc::Settings::SetLocalzLeafName( const QString &argLocalzLeafName ) {
    localzLeafName = argLocalzLeafName;
}

#endif // SETTINGS_H
