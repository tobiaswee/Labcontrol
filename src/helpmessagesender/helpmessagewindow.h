/*
 * Copyright 2018 Markus Prasser
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

#ifndef HELPMESSAGEWINDOW_H
#define HELPMESSAGEWINDOW_H

#include <QHostAddress>
#include <QMainWindow>

class QNetworkSession;
class QTcpSocket;

namespace Ui {
class HelpMessageWindow;
} // namespace Ui

class lcHelpMessageWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit lcHelpMessageWindow(const QString &argServerIP,
                                 const quint16 argServerPort,
                                 QWidget *const argParent = nullptr);
    ~lcHelpMessageWindow() override;

private:
    quint16 blockSize = 0;
    QTcpSocket *helpMessageSocket = nullptr;
    QNetworkSession *networkSession = nullptr;
    const QHostAddress serverAddress;
    const quint16 serverPort = 0;
    Ui::HelpMessageWindow *const ui = nullptr;

private slots:
    void DisplayError(QAbstractSocket::SocketError argSocketError);
    void OpenedSession();
    void ReadHelpReply();
    void RequestHelp();
};

#endif // HELPMESSAGEWINDOW_H
