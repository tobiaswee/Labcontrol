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

#include <memory>

#include <QDebug>

#include "client.h"
#include "settings.h"

extern std::unique_ptr< lc::Settings > settings;

lc::Client::Client( QString *argIP, QString *argMAC,
                    QString *argName, unsigned short int argXPosition,
                    unsigned short int argYPosition ):
    ip{ *argIP },
    mac{ *argMAC },
    name{ *argName },
    xPosition{ argXPosition },
    yPosition{ argYPosition },
    protectedCycles{ 0 }
{
    qRegisterMetaType< state_t >( "STATE" );

    if ( !settings->pingCmd.isEmpty() ) {
        pinger = new ClientPinger{ ip, settings->pingCmd };
        pinger->moveToThread( &pingerThread );
        connect( &pingerThread, &QThread::finished,
                 pinger, &QObject::deleteLater );
        connect( this, &Client::PingWanted,
                 pinger, &ClientPinger::doPing );
        connect( pinger, &ClientPinger::PingFinished,
                 this, &Client::GotStatusChanged );
        // connect(pinger, &ClientPinger::ping_string, this, &Client::display_ping_string);
        pingerThread.start();

        pingTimer = new QTimer{ this };
        connect( pingTimer, &QTimer::timeout,
                 this, &Client::RequestAPing ) ;
        pingTimer->start( 3000 );
    }

    qDebug() << "Created client" << name << "with MAC" << mac << "and IP" << ip
             << "at position" << QString{ QString::number( xPosition ) + "x"
                                          + QString::number( yPosition ) };
}

lc::Client::~Client() {
    if ( pingTimer ) {
        pingTimer->stop();
    }
    delete pingTimer;
    pingerThread.quit();
    pingerThread.wait();
}

void lc::Client::BeamFile( const QString &argFileToBeam, const QString * const argPublickeyPathUser, const QString * const argUserNameOnClients ) {
    if ( state < state_t::RESPONDING ) {
        return;
    }

    QStringList arguments;
    arguments << "-2" << "-i" << *argPublickeyPathUser << "-l" << "32768" << "-r"
              << argFileToBeam << QString{ *argUserNameOnClients + "@" + ip + ":media4ztree" };

    // Start the process
    QProcess beamFileProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    beamFileProcess.setProcessEnvironment( env );
    beamFileProcess.startDetached( settings->scpCmd, arguments );

    qDebug() << settings->scpCmd << arguments.join( " " );
}

void lc::Client::Boot( const QString &argNetworkBroadcastAddress ) {
    QStringList arguments{ QStringList{} << "-i" << argNetworkBroadcastAddress << mac };

    // Start the process
    QProcess wakeonlanProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    wakeonlanProcess.setProcessEnvironment( env );
    wakeonlanProcess.startDetached( settings->wakeonlanCmd, arguments );

    // Output message via the debug messages tab
    qDebug() << settings->wakeonlanCmd << arguments.join( " " );

    pingTimer->start( 3000 );

    protectedCycles = 7;
    GotStatusChanged( state_t::BOOTING );
}

void lc::Client::DeactiveScreensaver( const QString &argPublickeyPathUser,
                                      const QString &argUserNameOnClients ) {
    QStringList arguments;
    arguments << "-i" << argPublickeyPathUser << QString{ argUserNameOnClients + "@" + ip }
              << settings->xsetCmd << "-display" << ":0.0" << "dpms" << "force" <<  "on";

    // Start the process
    QProcess deactiveScreensaverProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    deactiveScreensaverProcess.setProcessEnvironment( env );
    deactiveScreensaverProcess.startDetached( settings->sshCmd, arguments );

    // Output message via the debug messages tab
    qDebug() << settings->sshCmd << arguments.join( " " );
}

// void Client::display_ping_string(QString *ping_string) {
//     debug_messages_text_edit->appendPlainText("[DEBUG] " + *ping_string);
//     delete ping_string;
// }

void lc::Client::GotStatusChanged( state_t argState ) {
    if ( ( protectedCycles > 0 ) && ( state == state_t::BOOTING ) && ( argState != state_t::RESPONDING ) ) {
        return;
    }
    if ( ( protectedCycles > 0 ) && ( state == state_t::SHUTTING_DOWN ) && argState != state_t::NOT_RESPONDING ) {
        return;
    }
    state = argState;
    qDebug() << name << "status changed to:" << static_cast< unsigned short int >( argState );
}

void lc::Client::KillZLeaf( const QString &argPublickeyPathUser,
                            const QString &argUserNameOnClients ) {
    QStringList arguments;
    arguments << "-i" << argPublickeyPathUser << QString{ argUserNameOnClients + "@" + ip }
              << QString{ settings->lcInstDir + "/scripts/kill_zLeaf_labcontrol2.sh" };

    // Start the process
    QProcess killZLeafProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    killZLeafProcess.setProcessEnvironment( env );
    killZLeafProcess.startDetached( settings->sshCmd, arguments );

    // Output message via the debug messages tab
    qDebug() << settings->sshCmd << arguments.join( " " );

    // Restart the ping_timer, because it is stopped when a zLeaf is started
    pingTimer->start( 3000 );
}

void lc::Client::OpenFilesystem( const QString * const argUserToBeUsed ) {
    if ( state < state_t::RESPONDING ) {
        return;
    }
    QStringList arguments = QStringList{} << QString{ "sftp://" + *argUserToBeUsed + "@" + ip };

    QProcess openFilesystemProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    openFilesystemProcess.setProcessEnvironment( env );
    openFilesystemProcess.startDetached( settings->fileMngr, arguments );
    qDebug() << settings->fileMngr << arguments.join( " " );
}

void lc::Client::OpenTerminal( const QString &argCommand, const bool &argOpenAsRoot,
                               const QString & argPublickeyPathUser,
                               const QString &argUserNameOnClients ) {
    if ( !settings->termEmulCmd.isEmpty() ) {
        if ( state < state_t::RESPONDING ) {
            return;
        }

        QStringList *arguments = nullptr;
        arguments = new QStringList;
        if ( !argOpenAsRoot ) {
            *arguments << "--title" << name << "-e" <<
                          QString{ settings->sshCmd + " -i " + argPublickeyPathUser + " "
                                   + argUserNameOnClients + "@" + ip };
        } else {
            *arguments << "--title" << name << "-e" <<
                          QString{ settings->sshCmd + " -i " + argPublickeyPathUser + " " + "root@" + ip };
        }

        if ( settings->termEmulCmd.contains( "konsole" ) ) {
            arguments->prepend( "--new-tab" );
            arguments->prepend( "--show-tabbar" );
        } else {
            if ( settings->termEmulCmd.contains( "gnome-terminal" ) ) {
                arguments->prepend( "--tab" );
            }
        }

        if ( !argCommand.isEmpty() ) {
            arguments->last().append( " '" + argCommand + "'" );
        }

        QProcess openTerminalProcess;
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        openTerminalProcess.setProcessEnvironment( env );
        openTerminalProcess.startDetached( settings->termEmulCmd, *arguments );
        qDebug() << settings->termEmulCmd << arguments->join( " " );
        delete arguments;
    }
}

void lc::Client::RequestAPing() {
    if ( protectedCycles > 0 ) {
        --protectedCycles;
    }
    emit PingWanted();
}

void lc::Client::SetStateToZLEAF_RUNNING( QString argClientIP ) {
    if ( argClientIP != ip ) {
        return;
    }
    if ( state != state_t::ZLEAF_RUNNING ) {
        pingTimer->stop();
        // Inform the ClientPinger instance, that zLeaf is now running
        pinger->setStateToZLEAF_RUNNING();
        this->GotStatusChanged( state_t::ZLEAF_RUNNING );
        qDebug() << "Client" << name << "got 'ZLEAF_RUNNING' signal.";
    }
}

void lc::Client::ShowDesktop() {
    QStringList arguments;
    arguments << ip;

    QProcess showDesktopProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    showDesktopProcess.setProcessEnvironment( env );
    showDesktopProcess.startDetached( settings->vncViewer, arguments );

    // Output message via the debug messages tab
    qDebug() << settings->vncViewer << arguments.join( " " );
}

void lc::Client::Shutdown( const QString &argPublickeyPathUser, const QString &argUserNameOnClients ) {
    if ( state == state_t::NOT_RESPONDING || state == state_t::BOOTING || state == state_t::SHUTTING_DOWN ) {
        return;
    }
    QStringList arguments;
    arguments << "-i" << argPublickeyPathUser << QString{ argUserNameOnClients  + "@" + ip } << "sudo shutdown -P now";

    // Start the process
    QProcess shutdownProcess;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    shutdownProcess.setProcessEnvironment( env );
    shutdownProcess.startDetached( settings->sshCmd, arguments );

    // Output message via the debug messages tab
    qDebug() << settings->sshCmd << arguments.join( " " );

    // This additional 'ping_timer' start is needed for the case that the clients are shut down without prior closing of zLeaves
    pingTimer->start( 3000 );

    protectedCycles = 3;
    GotStatusChanged( state_t::SHUTTING_DOWN );
}

void lc::Client::StartZLeaf( const QString &argPublickeyPathUser,
                             const QString &argUserNameOnClients,
                             const QString * const argZTreeVersion,
                             const QString &argServerIP, unsigned short int argPort,
                             const QString * const argFakeName ) {
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
            arguments << "-i" << argPublickeyPathUser << QString{ argUserNameOnClients + "@" + ip }
                      << "DISPLAY=:0.0" << QString{ "/home/" + argUserNameOnClients + "/start_zLeaf_labcontrol2.sh" }
                      << *argZTreeVersion << argServerIP;
        } else {
            if ( argFakeName  == nullptr ) {
                arguments << "-i" << argPublickeyPathUser << QString{ argUserNameOnClients + "@" + ip }
                          << "DISPLAY=:0.0" << QString{ "/home/" + argUserNameOnClients + "/start_zLeaf_labcontrol2.sh" }
                          << *argZTreeVersion << argServerIP << QString::number( static_cast< int >( argPort ) - 7000 );
            } else {
                arguments << "-i" << argPublickeyPathUser << QString{ argUserNameOnClients + "@" + ip }
                          << "DISPLAY=:0.0" << QString{ "/home/" + argUserNameOnClients + "/start_zLeaf_labcontrol2.sh" }
                          << *argZTreeVersion << argServerIP << QString::number( static_cast< int >( argPort ) - 7000 ) << *argFakeName ;
            }
        }

        // Start the process
        QProcess startZLeafProcess;
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        startZLeafProcess.setProcessEnvironment( env );
        startZLeafProcess.startDetached( settings->sshCmd, arguments );

        // Output message via the debug messages tab
        qDebug() << settings->sshCmd << arguments.join( " " );
    }
    delete messageBoxRunningZLeafFound;
}
