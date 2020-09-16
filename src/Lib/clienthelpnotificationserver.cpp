/*
 * Copyright 2014-2020 Markus Prasser
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

#include "clienthelpnotificationserver.h"
#include "settings.h"

#include <QNetworkConfigurationManager>
#include <QNetworkSession>
#include <QTcpServer>
#include <QTcpSocket>

extern std::unique_ptr<lc::Settings> settings;

/*!
 * \brief Create a new instance and start listening as soon as possible
 *
 * \param argParent The instance's parent QObject
 */
lc::ClientHelpNotificationServer::ClientHelpNotificationServer(
    QObject *argParent)
    : QObject{argParent}, hostAddress{settings->serverIP} {
  QNetworkConfigurationManager manager;
  if (manager.capabilities() &
      QNetworkConfigurationManager::NetworkSessionRequired) {
    // Get saved network configuration
    QSettings qSettings{QSettings::UserScope, QLatin1String{"QtProject"}};
    qSettings.beginGroup(QLatin1String{"QtNetwork"});
    const QString id =
        qSettings.value(QLatin1String{"DefaultNetworkConfiguration"})
            .toString();
    qSettings.endGroup();

    // If the saved network configuration is not currently discovered use the
    // system default
    QNetworkConfiguration config = manager.configurationFromIdentifier(id);
    if ((config.state() & QNetworkConfiguration::Discovered) !=
        QNetworkConfiguration::Discovered) {
      config = manager.defaultConfiguration();
    }
    networkSession = new QNetworkSession{config, this};
    connect(networkSession, &QNetworkSession::opened, this,
            &ClientHelpNotificationServer::OpenSession);
    networkSession->open();
  } else {
    OpenSession();
  }

  connect(helpMessageServer, &QTcpServer::newConnection, this,
          &ClientHelpNotificationServer::SendReply);
}

/*!
 * \brief Open a new network session and start listening for incoming
 * connections
 */
void lc::ClientHelpNotificationServer::OpenSession() {
  // Save the used configuration
  if (networkSession) {
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice) {
      id = networkSession
               ->sessionProperty(QLatin1String{"UserChoiceConfiguration"})
               .toString();
    } else {
      id = config.identifier();
    }

    QSettings qSettings(QSettings::UserScope, QLatin1String{"QtProject"});
    qSettings.beginGroup(QLatin1String{"QtNetwork"});
    qSettings.setValue(QLatin1String{"DefaultNetworkConfiguration"}, id);
    qSettings.endGroup();
  }

  helpMessageServer = new QTcpServer{this};
  if (!helpMessageServer->listen(QHostAddress{hostAddress},
                                 settings->clientHelpNotificationServerPort)) {
    QMessageBox messageBox{
        QMessageBox::Critical,
        tr("Unable to start the client help notification server"),
        tr("Unable to start the client help notification server.\nThe "
           "following error occurred:\n\n%1.")
            .arg(helpMessageServer->errorString()),
        QMessageBox::Ok};
    messageBox.exec();
    return;
  }
}

void lc::ClientHelpNotificationServer::SendReply() {
  QByteArray block;
  QDataStream out{&block, QIODevice::WriteOnly};
  out.setVersion(QDataStream::Qt_5_2);
  out << static_cast<quint16>(0);
  out << QString{"Help demand retrieved."};
  out.device()->seek(0);
  out << static_cast<quint16>(static_cast<unsigned int>(block.size()) -
                              sizeof(quint16));

  const auto clientConnection = helpMessageServer->nextPendingConnection();
  const auto peerAddress = clientConnection->peerAddress().toString();
  QString peerName;
  if (settings->clIPsToClMap.contains(peerAddress)) {
    peerName = settings->clIPsToClMap[peerAddress]->name;
  }

  connect(clientConnection, &QTcpSocket::disconnected, clientConnection,
          &QTcpSocket::deleteLater);
  clientConnection->write(block);
  clientConnection->disconnectFromHost();

  if (peerName.isEmpty()) {
    QMessageBox requestReceivedBox{
        QMessageBox::Information, tr("Unknown client asked for help."),
        tr("An unknown client with IP '%1' asked for help.").arg(peerAddress),
        QMessageBox::Ok};
    requestReceivedBox.exec();
  } else {
    QMessageBox requestReceivedBox{
        QMessageBox::Information, tr("'%1' asked for help.").arg(peerName),
        tr("'%1' asked for help.").arg(peerName), QMessageBox::Ok};
    requestReceivedBox.exec();
  }
}
