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

#ifndef CLIENTHELPNOTIFICATIONSERVER_H
#define CLIENTHELPNOTIFICATIONSERVER_H

#include "src/Lib/client.h"

#include <QObject>
#include <QHostAddress>
#include <QMessageBox>
#include <QtNetwork>

class lcClientHelpNotificationServer : public QObject
{
    Q_OBJECT
public:
    explicit lcClientHelpNotificationServer( const QMap< QString, lcClient* > * const argClientIPsToClientsMap,
            const QString * const argServerIP, const unsigned short int &argServerPort, QObject *argParent = nullptr );

signals:

public slots:

private:
    const QMap< QString, lcClient* > * const clientIPsToClientsMap = nullptr;
    QTcpServer *helpMessageServer = nullptr;
    const QHostAddress hostAddress;
    QNetworkSession *networkSession = nullptr;
    const unsigned short int serverPort = 0;

private slots:
    void OpenSession();
    void SendReply();
};

#endif // CLIENTHELPNOTIFICATIONSERVER_H
