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

#include "client.h"

lcClient::lcClient( QPlainTextEdit *argDebugMessagesTextEdit, QString *argIP, QString *argMAC, QString *argName, unsigned short int argXPosition, unsigned short int argYPosition, bool argHasWebcam, const QVector< QString* > * const argSettingsItems ):
    hasWebcam{ argHasWebcam },
    ip{ *argIP },
    mac{ *argMAC },
    name{ *argName },
    xPosition{ argXPosition },
    yPosition{ argYPosition },
    debugMessagesTextEdit{ argDebugMessagesTextEdit },
    protectedCycles{ 0 },
    settingsItems{ argSettingsItems }
{
    qRegisterMetaType< state_t >( "STATE" );

    if ( ( *settingsItems )[ ( int )settingsItems_t::PING_COMMAND ] ) {
        pinger = new lcClientPinger{ &ip, ( *settingsItems )[ ( int )settingsItems_t::PING_COMMAND ] };
        pinger->moveToThread( &pingerThread );
        connect( &pingerThread, &QThread::finished, pinger, &QObject::deleteLater );
        connect( this, &lcClient::PingWanted, pinger, &lcClientPinger::doPing );
        connect( pinger, &lcClientPinger::PingFinished, this, &lcClient::GotStatusChanged );
        // connect(pinger, &ClientPinger::ping_string, this, &Client::display_ping_string);
        pingerThread.start();

        pingTimer = new QTimer{ this };
        connect( pingTimer, &QTimer::timeout, this, &lcClient::RequestAPing ) ;
        pingTimer->start( 3000 );
    }

    debugMessagesTextEdit->appendPlainText( tr ( "[DEBUG] Created client %1\n\tMac: %2\tIP: %3\n\tPosition: %4x%5\t\tWebcam = %6" )
                                            .arg( name ).arg( mac ).arg( ip ).arg( QString::number( xPosition ) )
                                            .arg( QString::number( yPosition ) ).arg( QString::number( hasWebcam ) ) );
}

lcClient::~lcClient() {
    if ( pingTimer ) {
        pingTimer->stop();
    }
    delete pingTimer;
    pingerThread.quit();
    pingerThread.wait();
}

void lcClient::BeamFile( const QString &argFileToBeam, const QString * const argPublickeyPathUser, const QString * const argUserNameOnClients ) {
    if ( state < state_t::RESPONDING ) {
        return;
    }

    QStringList arguments;
    arguments << "-2" << "-i" << *argPublickeyPathUser << "-l" << "32768" << "-r" << argFileToBeam << QString{ *argUserNameOnClients + "@" + name + ":media4ztree" };

    // Start the process
    QProcess beamFileProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    beamFileProcess.setProcessEnvironment( env );
    beamFileProcess.startDetached( *( *settingsItems )[ ( int )settingsItems_t::RCP_COMMAND ], arguments );

    debugMessagesTextEdit->appendPlainText( "[DEBUG] " + *( *settingsItems )[ ( int )settingsItems_t::RCP_COMMAND ] + " " +  arguments.join( " " ) );
}

void lcClient::Boot( const QString * const argNetworkBroadcastAddress ) {
    if ( state == state_t::SHUTTING_DOWN || state == state_t::RESPONDING ) {
        return;
    }
    QStringList arguments;
    arguments << "-i" << *argNetworkBroadcastAddress << mac;

    // Start the process
    QProcess wakeonlanProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    wakeonlanProcess.setProcessEnvironment( env );
    wakeonlanProcess.startDetached( *( *settingsItems )[ ( int )settingsItems_t::WAKEONLAN_COMMAND ], arguments );

    // Output message via the debug messages tab
    debugMessagesTextEdit->appendPlainText( "[DEBUG] " +
                                            *( *settingsItems )[ ( int )settingsItems_t::WAKEONLAN_COMMAND ] +" " + arguments.join( " " ) );

    pingTimer->start( 3000 );

    protectedCycles = 7;
    GotStatusChanged( state_t::BOOTING );
}

void lcClient::DeactiveScreensaver( const QString * const argPublickeyPathUser, const QString * const argUserNameOnClients ) {
    QStringList arguments;
    arguments << "-i" << *argPublickeyPathUser << QString{ *argUserNameOnClients + "@" + name }
              << *( *settingsItems )[ ( int )settingsItems_t::XSET_COMMAND ] << "-display" << ":0.0" << "dpms" << "force" <<  "on";

    // Start the process
    QProcess deactiveScreensaverProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    deactiveScreensaverProcess.setProcessEnvironment( env );
    deactiveScreensaverProcess.startDetached( *( *settingsItems )[ ( int )settingsItems_t::SSH_COMMAND ], arguments );

    // Output message via the debug messages tab
    debugMessagesTextEdit->appendPlainText("[DEBUG] " + *( *settingsItems )[ ( int )settingsItems_t::SSH_COMMAND ] + " " +  arguments.join(" "));
}

// void Client::display_ping_string(QString *ping_string) {
//     debug_messages_text_edit->appendPlainText("[DEBUG] " + *ping_string);
//     delete ping_string;
// }

void lcClient::GotStatusChanged( state_t argState ) {
    if ( ( protectedCycles > 0 ) && ( state == state_t::BOOTING ) && ( argState != state_t::RESPONDING ) ) {
        return;
    }
    if ( ( protectedCycles > 0 ) && ( state == state_t::SHUTTING_DOWN ) && argState != state_t::NOT_RESPONDING ) {
        return;
    }
    state = argState;
    debugMessagesTextEdit->appendPlainText( "[DEBUG] " + name + " status changed to: " + QString::number( ( int )argState ) );
}

void lcClient::KillZLeaf( const QString * const argPublickeyPathUser, const QString * const argUserNameOnClients ) {
    QStringList arguments;
    arguments << "-i" << *argPublickeyPathUser << QString{ *argUserNameOnClients + "@" + name }
              << QString{ *( *settingsItems )[ ( int )settingsItems_t::LABCONTROL_INSTALLATION_DIRECTORY ] + "/scripts/kill_zLeaf_labcontrol2.sh" };

    // Start the process
    QProcess killZLeafProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    killZLeafProcess.setProcessEnvironment( env );
    killZLeafProcess.startDetached( *( *settingsItems )[ ( int )settingsItems_t::SSH_COMMAND ], arguments );

    // Output message via the debug messages tab
    debugMessagesTextEdit->appendPlainText( "[DEBUG] " + *( *settingsItems )[ ( int )settingsItems_t::SSH_COMMAND ] + " " +  arguments.join( " " ) );

    // Restart the ping_timer, because it is stopped when a zLeaf is started
    pingTimer->start( 3000 );
}

void lcClient::OpenFilesystem( const QString * const argUserToBeUsed ) {
    if ( state < state_t::RESPONDING ) {
        return;
    }
    QStringList arguments = QStringList{} << QString{ "sftp://" + *argUserToBeUsed + "@" + name };

    QProcess openFilesystemProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    openFilesystemProcess.setProcessEnvironment( env );
    openFilesystemProcess.startDetached( *( *settingsItems )[ ( int )settingsItems_t::FILE_MANAGER ], arguments );
    debugMessagesTextEdit->appendPlainText( "[DEBUG] " + *( *settingsItems )[ ( int )settingsItems_t::FILE_MANAGER ] + " " + arguments.join( " " ) );
}

void lcClient::OpenTerminal( const QString &argCommand, const bool &argOpenAsRoot, const QString * const argPublickeyPathUser, const QString * const argUserNameOnClients ) {
    if ( ( *settingsItems )[ ( int )settingsItems_t::TERMINAL_EMULATOR_COMMAND ] ) {
        if ( state < state_t::RESPONDING ) {
            return;
        }

        QStringList *arguments = nullptr;
        arguments = new QStringList;
        if ( !argOpenAsRoot ) {
            *arguments << "--title" << name << "-e" <<
                          QString{ *( *settingsItems )[ ( int )settingsItems_t::SSH_COMMAND ] + " -i " + *argPublickeyPathUser + " " + *argUserNameOnClients + "@" + name };
        } else {
            *arguments << "--title" << name << "-e" <<
                          QString{ *( *settingsItems )[ ( int )settingsItems_t::SSH_COMMAND ] + " -i " + *argPublickeyPathUser + " " + "root@" + name };
        }

        if ( ( *settingsItems )[ ( int )settingsItems_t::TERMINAL_EMULATOR_COMMAND ]->contains( "konsole" ) ) {
            arguments->prepend( "--new-tab" );
            arguments->prepend( "--show-tabbar" );
        } else {
            if ( ( *settingsItems )[ ( int )settingsItems_t::TERMINAL_EMULATOR_COMMAND ]->contains( "gnome-terminal" ) ) {
                arguments->prepend( "--tab" );
            }
        }

        if ( !argCommand.isEmpty() ) {
            arguments->last().append( " '" + argCommand + "'" );
        }

        QProcess openTerminalProcess;
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        openTerminalProcess.setProcessEnvironment( env );
        openTerminalProcess.startDetached( *( *settingsItems )[ ( int )settingsItems_t::TERMINAL_EMULATOR_COMMAND ], *arguments );
        debugMessagesTextEdit->appendPlainText("[DEBUG] " +
                                               *( *settingsItems )[ ( int )settingsItems_t::TERMINAL_EMULATOR_COMMAND ] + " " + arguments->join(" "));
        delete arguments;
    }
}

void lcClient::RequestAPing() {
    if ( protectedCycles > 0 ) {
        --protectedCycles;
    }
    emit PingWanted();
}

void lcClient::SetStateToZLEAF_RUNNING( QString argClientIP ) {
    if ( argClientIP != ip ) {
        return;
    }
    if ( state != state_t::ZLEAF_RUNNING ) {
        pingTimer->stop();
        // Inform the ClientPinger instance, that zLeaf is now running
        pinger->setStateToZLEAF_RUNNING();
        this->GotStatusChanged( state_t::ZLEAF_RUNNING );
        debugMessagesTextEdit->appendPlainText( "[DEBUG] Client '" + name + "' got 'ZLEAF_RUNNING' signal." );
    }
}

void lcClient::ShowDesktop() {
    QStringList arguments;
    arguments << name;

    QProcess showDesktopProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    showDesktopProcess.setProcessEnvironment( env );
    showDesktopProcess.startDetached( *( *settingsItems )[ ( int )settingsItems_t::VNC_VIEWER ], arguments );

    // Output message via the debug messages tab
    debugMessagesTextEdit->appendPlainText( "[DEBUG] " + *( *settingsItems )[ ( int )settingsItems_t::VNC_VIEWER ] + " " +  arguments.join( " " ) );
}

void lcClient::Shutdown( const QString * const argPublickeyPathUser, const QString * const argUserNameOnClients ) {
    if ( state == state_t::NOT_RESPONDING || state == state_t::BOOTING || state == state_t::SHUTTING_DOWN ) {
        return;
    }
    QStringList arguments;
    arguments << "-i" << *argPublickeyPathUser << QString{ *argUserNameOnClients  + "@" + name } << "sudo shutdown -P now";

    // Start the process
    QProcess shutdownProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    shutdownProcess.setProcessEnvironment( env );
    shutdownProcess.startDetached( *( *settingsItems )[ ( int )settingsItems_t::SSH_COMMAND ], arguments );

    // Output message via the debug messages tab
    debugMessagesTextEdit->appendPlainText( "[DEBUG] " + *( *settingsItems )[ ( int )settingsItems_t::SSH_COMMAND ] + " " +  arguments.join( " " ) );

    // This additional 'ping_timer' start is needed for the case that the clients are shut down without prior closing of zLeaves
    pingTimer->start( 3000 );

    protectedCycles = 3;
    GotStatusChanged( state_t::SHUTTING_DOWN );
}

void lcClient::StartZLeaf( const QString * const argPublickeyPathUser, const QString * const argUserNameOnClients, const QString * const argZTreeVersion,
                           const QString * const argServerIP, unsigned short int argPort, const QString * const argFakeName ) {
    if ( state < state_t::RESPONDING ) {
        return;
    }

    // Create a QMessageBox for user interaction if there is already a zLeaf running
    QMessageBox *messageBoxRunningZLeafFound = nullptr;
    if ( state == state_t::ZLEAF_RUNNING ) {
        messageBoxRunningZLeafFound = new QMessageBox{ QMessageBox::Warning, "Running zLeaf found",
                QString{ "There is already a zLeaf running on " + name + "." }, QMessageBox::No | QMessageBox::Yes };
        messageBoxRunningZLeafFound->setInformativeText( "Do you want to start a zLeaf on client " + name + " nonetheless?" );
        messageBoxRunningZLeafFound->setDefaultButton( QMessageBox::No );
        messageBoxRunningZLeafFound->exec();
    }

    if ( ( messageBoxRunningZLeafFound != nullptr && messageBoxRunningZLeafFound->clickedButton() == messageBoxRunningZLeafFound->button( QMessageBox::Yes ) ) || state != state_t::ZLEAF_RUNNING ) {
        QStringList arguments;
        if ( argFakeName  == nullptr && argPort == 7000 ) {
            arguments << "-i" << *argPublickeyPathUser << QString{ *argUserNameOnClients + "@" + name }
                      << "DISPLAY=:0.0" << QString{ "/home/" + *argUserNameOnClients + "/start_zLeaf_labcontrol2.sh" } << *argZTreeVersion << *argServerIP;
        } else {
            if ( argFakeName  == nullptr ) {
                arguments << "-i" << *argPublickeyPathUser << QString{ *argUserNameOnClients + "@" + name }
                          << "DISPLAY=:0.0" << QString{ "/home/" + *argUserNameOnClients + "/start_zLeaf_labcontrol2.sh" } << *argZTreeVersion << *argServerIP << QString::number( static_cast< int >( argPort ) - 7000 );
            } else {
                arguments << "-i" << *argPublickeyPathUser << QString{ *argUserNameOnClients + "@" + name }
                          << "DISPLAY=:0.0" << QString{ "/home/" + *argUserNameOnClients + "/start_zLeaf_labcontrol2.sh" } << *argZTreeVersion << *argServerIP << QString::number( static_cast< int >( argPort ) - 7000 ) << *argFakeName ;
            }
        }

        // Start the process
        QProcess startZLeafProcess;
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        startZLeafProcess.setProcessEnvironment( env );
        startZLeafProcess.startDetached( *( *settingsItems )[ ( int )settingsItems_t::SSH_COMMAND ], arguments );

        // Output message via the debug messages tab
        debugMessagesTextEdit->appendPlainText( "[DEBUG] " + *( *settingsItems )[ ( int )settingsItems_t::SSH_COMMAND ] + " " +  arguments.join( " " ) );
    }
    delete messageBoxRunningZLeafFound;
}