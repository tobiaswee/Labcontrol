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

#include "client.h"
#include "clienthelpnotificationserver.h"
#include "settings.h"

#include <QMessageBox>
#include <QNetworkConfigurationManager>
#include <QNetworkSession>
#include <QTcpServer>
#include <QTcpSocket>

lc::ClientHelpNotificationServer::ClientHelpNotificationServer(const Settings *const argSettings,
                                                               QObject *argParent) :
    QObject{argParent},
    hostAddress{settings->serverIP},
    settings{argSettings}
{
    QNetworkConfigurationManager manager;
    if (manager.capabilities()
            & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings{QSettings::UserScope, QLatin1String{"QtProject"}};
        settings.beginGroup(QLatin1String{"QtNetwork"});
        const QString id{settings.value(QLatin1String{"DefaultNetworkConfiguration"}).toString()};
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config{manager.configurationFromIdentifier(id)};
        if ((config.state() & QNetworkConfiguration::Discovered)
                != QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }
        networkSession = new QNetworkSession{config, this};
        connect(networkSession, &QNetworkSession::opened,
                this, &ClientHelpNotificationServer::OpenSession);
        networkSession->open();
    } else {
        OpenSession();
    }

    connect(helpMessageServer, &QTcpServer::newConnection,
            this, &ClientHelpNotificationServer::SendReply);
}

void lc::ClientHelpNotificationServer::OpenSession()
{
    // Save the used configuration
    if (networkSession) {
        QNetworkConfiguration config{networkSession->configuration()};
        QString id;
        if (config.type() == QNetworkConfiguration::UserChoice) {
            id = networkSession->sessionProperty(QLatin1String{"UserChoiceConfiguration"}).toString();
        } else {
            id = config.identifier();
        }

        QSettings settings{QSettings::UserScope, QLatin1String{"QtProject"}};
        settings.beginGroup(QLatin1String{"QtNetwork"});
        settings.setValue(QLatin1String{"DefaultNetworkConfiguration"}, id);
        settings.endGroup();
    }

    helpMessageServer = new QTcpServer{this};
    if ( !helpMessageServer->listen( hostAddress, settings->clientHelpNotificationServerPort ) ) {
        QMessageBox messageBox{ QMessageBox::Critical, tr( "Unable to start the client help notification server" ),
                                tr( "Unable to start the client help notification server.\nThe following error occurred:\n\n%1." ).arg( helpMessageServer->errorString() ), QMessageBox::Ok };
        messageBox.exec();
        return;
    }
}

void lc::ClientHelpNotificationServer::SendReply()
{
    QByteArray block;
    QDataStream out{&block, QIODevice::WriteOnly};
    out.setVersion(QDataStream::Qt_5_2);
    out << static_cast<quint16>(0);
    out << QString{"Help demand retrieved."};
    out.device()->seek(0);
    out << static_cast<quint16>(block.size() - sizeof(quint16));

    auto clientConnection = helpMessageServer->nextPendingConnection();
    QString peerAddress = clientConnection->peerAddress().toString();
    QString peerName;
    bool unknownClient = false;
    if (settings->clIPsToClMap.contains(peerAddress)) {
        peerName = settings->clIPsToClMap[peerAddress]->name;
    } else {
        unknownClient = true;
    }

    connect(clientConnection, &QTcpSocket::disconnected,
            clientConnection, &QTcpSocket::deleteLater);
    clientConnection->write(block);
    clientConnection->disconnectFromHost();

    if (unknownClient) {
        QMessageBox requestReceivedBox{QMessageBox::Information,
                                       tr("Unknown client asked for help."),
                                       tr("An unknown client with IP '%1' asked"
                                          " for help.").arg(peerAddress),
                                       QMessageBox::Ok};
        requestReceivedBox.exec();
    } else {
        QMessageBox requestReceivedBox{QMessageBox::Information,
                                       tr("'%1' asked for help.").arg( peerName ),
                                       tr("'%1' asked for help.").arg(peerName),
                                       QMessageBox::Ok};
        requestReceivedBox.exec();
    }
}
