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

#include "helpmessagewindow.h"
#include "ui_helpmessagewindow.h"

#include <QNetworkConfigurationManager>
#include <QNetworkSession>
#include <QSettings>
#include <QTcpSocket>

#include <iostream>

lc::HelpMessageWindow::HelpMessageWindow(const QString &argServerIP,
                                         const uint16_t argServerPort,
                                         QWidget *argParent)
    : QMainWindow{argParent}, helpMessageSocket{new QTcpSocket{this}},

      serverAddress{argServerIP},
      serverPort{argServerPort}, ui{new Ui::HelpMessageWindow} {
  ui->setupUi(this);

  connect(ui->PBAskForHelp, &QPushButton::clicked, this,
          &lc::HelpMessageWindow::RequestHelp);
  connect(helpMessageSocket, &QTcpSocket::readyRead, this,
          &lc::HelpMessageWindow::ReadHelpReply);
  connect(helpMessageSocket,
          static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>(
              &QAbstractSocket::error),
          this, &HelpMessageWindow::DisplayError);

  QNetworkConfigurationManager manager;
  if (manager.capabilities() &
      QNetworkConfigurationManager::NetworkSessionRequired) {
    // Get saved network configuration
    QSettings settings{QSettings::UserScope, QLatin1String{"QtProject"}};
    settings.beginGroup(QLatin1String{"QtNetwork"});
    const QString id =
        settings.value(QLatin1String{"DefaultNetworkConfiguration"}).toString();
    settings.endGroup();

    // If the saved network configuration is not currently discovered use the
    // system default
    QNetworkConfiguration config = manager.configurationFromIdentifier(id);
    if ((config.state() & QNetworkConfiguration::Discovered) !=
        QNetworkConfiguration::Discovered) {
      config = manager.defaultConfiguration();
    }

    networkSession = new QNetworkSession{config, this};
    connect(networkSession, &QNetworkSession::opened, this,
            &lc::HelpMessageWindow::OpenedSession);

    networkSession->open();
  }
}

/*!
 * \brief Destroy the HelpMessageWindow instance
 */
lc::HelpMessageWindow::~HelpMessageWindow() { delete ui; }

/*!
 * \brief Display an error message to the user if requesting help failed
 *
 * \param socketError The error that occurred upon requesting help
 */
void lc::HelpMessageWindow::DisplayError(
    const QAbstractSocket::SocketError socketError) {
  QString errorMessage;

  switch (socketError) {
  case QAbstractSocket::RemoteHostClosedError:
    return;
  case QAbstractSocket::HostNotFoundError:
    errorMessage = tr("An error occurred: The server could not be found for "
                      "error reporting:\n");
    break;
  case QAbstractSocket::ConnectionRefusedError:
    errorMessage = tr("An error occurred: The connection was refused by the "
                      "laboratory server:\n");
    break;
  default:
    errorMessage = tr("The following error occured:\n");
  }
  errorMessage.append(tr("%1").arg(helpMessageSocket->errorString()));
  errorMessage.append(
      "\n\nPlease raise your hand to notify the experimenters.");

  ui->LSendingSuccess->setText(errorMessage);
}

/*!
 * \brief Handle the opening of a new QNetworkSession session
 */
void lc::HelpMessageWindow::OpenedSession() {
  // Save the used configuration
  QNetworkConfiguration config = networkSession->configuration();
  QString id;
  if (config.type() == QNetworkConfiguration::UserChoice)
    id = networkSession
             ->sessionProperty(QLatin1String{"UserChoiceConfiguration"})
             .toString();
  else
    id = config.identifier();

  QSettings settings{QSettings::UserScope, QLatin1String{"QtProject"}};
  settings.beginGroup(QLatin1String{"QtNetwork"});
  settings.setValue(QLatin1String{"DefaultNetworkConfiguration"}, id);
  settings.endGroup();
}

/*!
 * \brief Read help request reply from laboratory server and notify user
 */
void lc::HelpMessageWindow::ReadHelpReply() {
  QDataStream in{helpMessageSocket};
  in.setVersion(QDataStream::Qt_5_2);

  if (blockSize == 0) {
    if (helpMessageSocket->bytesAvailable() <
        static_cast<int>(sizeof(quint16))) {
      return;
    }

    in >> blockSize;
  }

  if (helpMessageSocket->bytesAvailable() < blockSize) {
    return;
  }

  QString serverAnswer;
  in >> serverAnswer;

  if (serverAnswer == "Help demand retrieved.") {
    ui->LSendingSuccess->setText(
        tr("Help message successfully sent.\nPlease wait for the experimenter "
           "to show up at your booth."));
    ui->PBAskForHelp->setEnabled(false);
  } else {
    ui->LSendingSuccess->setText(tr("An error occurred sending the help "
                                    "message. Please raise your arm.\n\n'%1'")
                                     .arg(serverAnswer));
  }
}

/*!
 * \brief Request help from the laboratory server just by connecting to it
 */
void lc::HelpMessageWindow::RequestHelp() {
  blockSize = 0;
  helpMessageSocket->abort();
  helpMessageSocket->connectToHost(serverAddress, serverPort);
}
