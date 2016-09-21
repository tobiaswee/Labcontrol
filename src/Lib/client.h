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

#ifndef CLIENT_H
#define CLIENT_H

#include <QMessageBox>
#include <QPlainTextEdit>
#include <QProcess>
#include <QString>
#include <QTextStream>
#include <QThread>
#include <QTimer>

#include "global.h"
#include "clientpinger.h"

//! Class which represents the clients in the lab
/*!
  This class contains elements and functions needed to represent all functions of a client.
*/
class lcClient : public QObject
{
    Q_OBJECT

public slots:
    //! Sets the STATE of the client to 'ZLEAF_RUNNING'
    void SetStateToZLEAF_RUNNING( QString argClientIP );

public:
    const bool hasWebcam = false;
    const QString ip;
    const QString mac;
    const QString name;
    const unsigned short int xPosition = 1;
    const unsigned short int yPosition = 1;

    //! Client's constructor
    /*!
      @param argDebugMessagesTextEditPtr    A pointer to the debug messages text edit for verbose output
      @param argIP                          The IP address the represented client has
      @param argMAC                         The MAC address the represented client has
      @param argName                        The hostname of the represented client
      @param argXPosition                   The client's x coordinate in the lab
      @param argYPosition                   The client's y coordinate in the lab
      @param argHasWebcam                   If the represented client has a webcam installed
      @param argSettingsItems               A QVector storing many needed data QStrings
    */
    lcClient( QPlainTextEdit *argDebugMessagesTextEditPtr, QString *argIP, QString *argMAC, QString *argName, unsigned short int argXPosition, unsigned short int argYPosition, bool argHasWebcam, const QVector< QString* > * const argSettingsItems );
    //! Client's destructor
    ~lcClient();
    //! Beams the chosen file to the client's 'media4ztree' directory
    /*!
      @param argFileToBeam                  The file which shall be beamed to the client's 'media4ztree' directory
      @param argPublickeyPathUser           The path to the publickey for user login on the clients
      @param argUserNameOnClients           The name of the user on the clients
    */
    void BeamFile( const QString &argFileToBeam, const QString * const argPublickeyPathUser, const QString * const argUserNameOnClients );
    //! Boots the client
    /*!
      @param argNetworkBroadcastAddress     The network broadcast address of the lab
    */
    void Boot( const QString * const argNetworkBroadcastAddress );
    //! Runs the 'deactivate_screensaver.sh' script on the client to deactivate the screensaver
    /*!
      @param argPublickeyPathUser           The path to the publickey for user login on the clients
      @param argUserNameOnClients           The name of the user on the clients
    */
    void DeactiveScreensaver( const QString * const argPublickeyPathUser, const QString * const argUserNameOnClients );
    //! Returns the current state of the client
    /*!
      @return The current state of the client
    */
    state_t GetClientState() const { return state; }
    //! Kills all processes 'zleaf.exe' on the client
    /*!
     @param argPublickeyPathUser            The path to the publickey for user login on the clients
     @param argUserNameOnClients            The name of the user on the clients
    */
    void KillZLeaf( const QString * const argPublickeyPathUser, const QString * const argUserNameOnClients );
    //! Opens a file manager for the client's file system
    /*!
     @param argUserToBeUsed                 The name of the user on the clients
    */
    void OpenFilesystem( const QString * const argUserToBeUsed );
    //! Opens a terminal for the client
    /*!
     @param argCommand                      A command which shall be executed in the terminal window (Pass an empty QString if not wanted)
     @param argOpenAsRoot                   Run the terminal session as root (true) or as normal user (false)
     @param argPublickeyPathUser            The path to the publickey for user login on the clients
     @param argUserNameOnClients            The name of the user on the clients
    */
    void OpenTerminal( const QString &argCommand, const bool &argOpenAsRoot, const QString * const argPublickeyPathUser, const QString * const argUserNameOnClients );
    //! Shows the desktop of the given client
    void ShowDesktop();
    //! Shuts down the client
    /*!
     @param argPublickeyPathUser            The path to the publickey for user login on the clients
     @param argUserNameOnClients            The name of the user on the clients
    */
    void Shutdown( const QString * const argPublickeyPathUser, const QString * const argUserNameOnClients );
    //! Starts a zLeaf instance on the client
    /*!
     @param argPublickeyPathUser            The path to the publickey for user login on the clients
     @param argUserNameOnClients            The name of the user on the clients
     @param argZTreeVersion                 The version of zLeaf which shall be started
     @param argServerIP                     The ip of the server which is running zTree
     @param argPort                         The port zTree ist listening on on the server
     @param argFakeName                     The name the zLeaf instance shall have (if not the default, which is the hostname of the client)
    */
    void StartZLeaf( const QString * const argPublickeyPathUser, const QString * const argUserNameOnClients, const QString * const argZTreeVersion,
                     const QString * const argServerIP, unsigned short int argPort, const QString * const argFakeName = nullptr );

private:
    QPlainTextEdit * const debugMessagesTextEdit = nullptr;
    unsigned short int protectedCycles;
    lcClientPinger *pinger = nullptr;
    QThread pingerThread;
    const QVector<QString*> * const settingsItems;
    state_t state = state_t::UNINITIALIZED;
    QTimer *pingTimer = nullptr;                       //! QTimer used to trigger pings by pinger's ClientPinger instance

private slots:
    // void display_ping_string(QString *ping_string);
    void GotStatusChanged( state_t argState );
    void RequestAPing();

signals:
    void PingWanted();
};

#endif // CLIENT_H
