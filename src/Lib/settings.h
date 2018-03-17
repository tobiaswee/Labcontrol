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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDebug>
#include <QObject>
#include <QSettings>

#include "client.h"

namespace lc {

class Settings : public QObject
{
    Q_OBJECT

public:
    Settings() = delete;
    explicit Settings(const QSettings &argSettings, QObject *argParent = nullptr);
    Settings(const Settings &argSettings) = delete;
    Settings &operator=(const Settings &argSettings) = delete;
    Settings(Settings &&argSettings ) = delete;
    Settings &operator=(Settings &&argSettings) = delete;
    ~Settings();

    int GetChosenZTreePort() const
    {
        return chosenzTreePort;
    }
    const QVector<Client *> &GetClients() const
    {
        return clients;
    }
    QString GetLocalzLeafName() const;
    void SetChosenZTreePort(const int argPort);
    void SetLocalzLeafName(const QString &argLocalzLeafName);

    const int defaultReceiptIndex = 0;
    const QString browserCmd;
    const QString clientBrowserCmd;
    const QString dvipsCmd;
    const QString fileMngr;
    const QString killallCmd;
    const QString latexCmd;
    const QString lcDataDir;
    const QString localUserName;
    QString localzLeafSize;
    void SetLocalzLeafSize(QString arg);
    QString GetLocalzLeafSize() const
    {
        return localzLeafSize;
    }
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
    const QString webcamDisplayCmd;
    const QStringList webcams;
    const QStringList webcams_names;
    const QString wineCmd;
    const QString wmctrlCmd;
    const QString xsetCmd;
    const QString zTreeInstDir;
    const QString restartCrashedSessionScript;
    const QStringList adminUsers;
    const QStringList installedLaTeXHeaders;
    const QStringList installedZTreeVersions;
    const quint16 clientHelpNotificationServerPort = 0;

private:
    static bool CheckPathAndComplain(const QString &argPath,
                                     const QString &argVariableName,
                                     const QString &argMessage);
    QVector<Client *> CreateClients(const QSettings &argSettings,
                                    const QString &argPingCmd);
    static QMap<QString, Client *> CreateClIPsToClMap(const QVector<Client *> &argClients);
    QStringList DetectInstalledLaTeXHeaders() const;
    QStringList DetectInstalledzTreeVersions() const;
    static QStringList GetAdminUsers(const QSettings &argSettings);
    static quint16 GetClientHelpNotificationServerPort(const QSettings &argSettings);
    static int GetDefaultReceiptIndex(const QSettings &argSettings);
    static int GetInitialPort(const QSettings &argSettings);
    static QString GetLocalUserName();
    static QString ReadSettingsItem(const QString &argVariableName,
                                    const QString &argMessage,
                                    const QSettings &argSettings,
                                    bool argItemIsFile);

    int chosenzTreePort = 0;
    QVector<Client *> clients;
    QString localzLeafName;

public:
    QMap<QString, Client *> clIPsToClMap;
};

} // namespace lc

inline QString lc::Settings::GetLocalzLeafName() const
{
    return localzLeafName;
}

inline void lc::Settings::SetLocalzLeafName(const QString &argLocalzLeafName)
{
    localzLeafName = argLocalzLeafName;
}

#endif // SETTINGS_H
