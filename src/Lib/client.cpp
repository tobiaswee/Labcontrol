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

#include "clientpinger.h"
#include "lablib.h"

#include <memory>

extern std::unique_ptr<lc::Settings> settings;
extern std::unique_ptr<lc::Lablib> lablib;

lc::Client::Client(const QString &argIP, const QString &argMAC, const QString &argName,
                   const unsigned short int argXPosition,
                   const unsigned short int argYPosition,
                   const QString &argPingCmd):
    ip{argIP},
    mac{argMAC},
    name{argName},
    xPosition{argXPosition},
    yPosition{argYPosition},
    protectedCycles{0}
{
    qRegisterMetaType<state_t>("state_t");

    if (!argPingCmd.isEmpty()) {
        pinger = new ClientPinger{ip, argPingCmd};
        pinger->moveToThread(&pingerThread);
        connect(&pingerThread, &QThread::finished,
                pinger, &QObject::deleteLater);
        connect(this, &Client::PingWanted,
                pinger, &ClientPinger::doPing);
        connect(pinger, &ClientPinger::PingFinished,
                this, &Client::GotStatusChanged);
        pingerThread.start();

        pingTimer = new QTimer{this};
        connect(pingTimer, &QTimer::timeout,
                this, &Client::RequestAPing) ;
        pingTimer->start(3000);
    }

    qDebug() << "Created client" << name << "with MAC" << mac << "and IP" << ip
             << "at position" << QString{QString::number(xPosition) + "x"
                                         + QString::number(yPosition)};
}

lc::Client::~Client()
{
    if (pingTimer) {
        pingTimer->stop();
    }
    delete pingTimer;
    pingerThread.quit();
    pingerThread.wait();
}

void lc::Client::BeamFile(const QString &argFileToBeam,
                          const QString *const argPublickeyPathUser,
                          const QString *const argUserNameOnClients)
{
    if (state < state_t::RESPONDING) {
        return;
    }

    const QStringList arguments{"-2", "-i", *argPublickeyPathUser, "-l",
        "32768", "-r", argFileToBeam,
        QString{*argUserNameOnClients + "@" + ip + ":media4ztree"}};

    // Start the process
    QProcess beamFileProcess;
    beamFileProcess.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    beamFileProcess.startDetached(settings->scpCmd, arguments);

    qDebug() << settings->scpCmd << arguments.join(" ");
}

void lc::Client::Boot()
{
    const QStringList arguments{"-i", settings->netwBrdAddr, mac};

    // Start the process
    QProcess wakeonlanProcess;
    wakeonlanProcess.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    wakeonlanProcess.startDetached(settings->wakeonlanCmd, arguments);

    // Output message via the debug messages tab
    qDebug() << settings->wakeonlanCmd << arguments.join(" ");

    pingTimer->start(3000);

    protectedCycles = 7;
    GotStatusChanged(state_t::BOOTING);
}

void lc::Client::GotStatusChanged(state_t argState)
{
    if ((protectedCycles > 0) && (state == state_t::BOOTING)
            && (argState != state_t::RESPONDING)) {
        return;
    }
    if ((protectedCycles > 0) && (state == state_t::SHUTTING_DOWN)
            && argState != state_t::NOT_RESPONDING) {
        return;
    }
    state = argState;
    qDebug() << name << "status changed to:"
             << static_cast<unsigned short int>(argState);
}

void lc::Client::KillZLeaf()
{
    const QStringList arguments{"-i", settings->pkeyPathUser,
        QString{settings->userNameOnClients + "@" + ip},
        settings->killallCmd, "-I", "-q", "zleaf.exe"};

    // Start the process
    QProcess killZLeafProcess;
    killZLeafProcess.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    killZLeafProcess.startDetached(settings->sshCmd, arguments);

    // Output message via the debug messages tab
    qDebug() << settings->sshCmd << arguments.join(" ");

    // Restart the ping_timer, because it is stopped when a zLeaf is started
    pingTimer->start(3000);
}

void lc::Client::OpenFilesystem(const QString *const argUserToBeUsed)
{
    if (state < state_t::RESPONDING) {
        return;
    }
    const QStringList arguments{QString{ "sftp://" + *argUserToBeUsed + "@" + ip }};

    QProcess openFilesystemProcess;
    openFilesystemProcess.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    openFilesystemProcess.startDetached(settings->fileMngr, arguments);
    qDebug() << settings->fileMngr << arguments.join(" ");
}

void lc::Client::OpenTerminal(const QString &argCommand,
                              const bool argOpenAsRoot)
{
    if (!settings->termEmulCmd.isEmpty()) {
        if (state < state_t::RESPONDING) {
            return;
        }

        QStringList arguments;
        if (!argOpenAsRoot) {
            arguments << "-e"
                      << QString{settings->sshCmd + " -i " + settings->pkeyPathUser + " "
                                 + settings->userNameOnClients + "@" + ip};
        } else {
            arguments << "-e" <<
                      QString{settings->sshCmd + " -i " + settings->pkeyPathRoot
                              + " " + "root@" + ip};
        }

        if (!argCommand.isEmpty()) {
            arguments.last().append(" '" + argCommand + "'");
        }

        QProcess openTerminalProcess;
        openTerminalProcess.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
        openTerminalProcess.startDetached(settings->termEmulCmd, arguments);
        qDebug() << settings->termEmulCmd << arguments.join(" ");
    }
}

void lc::Client::RequestAPing()
{
    if (protectedCycles > 0) {
        --protectedCycles;
    }
    emit PingWanted();
}

void lc::Client::SetStateToZLEAF_RUNNING(const QString &argClientIP)
{
    if ( argClientIP != ip ) {
        return;
    }
    if (state != state_t::ZLEAF_RUNNING) {
        pingTimer->stop();
        // Inform the ClientPinger instance, that zLeaf is now running
        pinger->setStateToZLEAF_RUNNING();
        this->GotStatusChanged(state_t::ZLEAF_RUNNING);
        qDebug() << "Client" << name << "got 'ZLEAF_RUNNING' signal.";
    }
}

void lc::Client::ShowDesktopViewOnly()
{
    QStringList arguments{ip};

    QProcess showDesktopProcess;
    showDesktopProcess.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    showDesktopProcess.startDetached( settings->vncViewer, QStringList{ip});

    // Output message via the debug messages tab
    qDebug() << settings->vncViewer << arguments.join( " " );
}

void lc::Client::ShowDesktopFullControl()
{
    const QStringList arguments{QString{ip + ":5901"}};

    QProcess showDesktopProcess;
    showDesktopProcess.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    showDesktopProcess.startDetached(settings->vncViewer, arguments);

    // Output message via the debug messages tab
    qDebug() << settings->vncViewer << arguments.join(" ");
}

void lc::Client::Shutdown()
{
    if (state == state_t::NOT_RESPONDING || state == state_t::BOOTING
            || state == state_t::SHUTTING_DOWN) {
        return;
    }
    const QStringList arguments{"-i", settings->pkeyPathUser,
        QString{settings->userNameOnClients  + "@" + ip},
        "sudo shutdown -P now"};

    // Start the process
    QProcess shutdownProcess;
    shutdownProcess.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    shutdownProcess.startDetached(settings->sshCmd, arguments);

    // Output message via the debug messages tab
    qDebug() << settings->sshCmd << arguments.join(" ");

    // This additional 'ping_timer' start is needed for the case that the clients are shut down without prior closing of zLeaves
    pingTimer->start(3000);

    protectedCycles = 3;
    GotStatusChanged(state_t::SHUTTING_DOWN);
}

void lc::Client::StartZLeaf(const QString *argFakeName, QString cmd)
{
    if (state < state_t::RESPONDING
            || zLeafVersion.isEmpty()
            || GetSessionPort() < 7000) {
        return;
    }

    // Create a QMessageBox for user interaction if there is already a zLeaf running
    std::unique_ptr<QMessageBox> messageBoxRunningZLeafFound;
    if (state == state_t::ZLEAF_RUNNING) {
        messageBoxRunningZLeafFound.reset(new QMessageBox{QMessageBox::Warning, "Running zLeaf found",
                                                          QString{"There is already a zLeaf running on " + name + "."},
                                                          QMessageBox::No | QMessageBox::Yes});
        messageBoxRunningZLeafFound->setInformativeText("Do you want to start a zLeaf on client "
                                                        + name + " nonetheless?");
        messageBoxRunningZLeafFound->setDefaultButton(QMessageBox::No);
        messageBoxRunningZLeafFound->exec();
    }

    if ((messageBoxRunningZLeafFound.get() != nullptr
            && messageBoxRunningZLeafFound->clickedButton()
            == messageBoxRunningZLeafFound->button(QMessageBox::Yes))
            || state != state_t::ZLEAF_RUNNING) {
        QStringList arguments;
        if (argFakeName  == nullptr) {
            arguments << "-i" << settings->pkeyPathUser
                      << QString{settings->userNameOnClients + "@" + ip}
                      << cmd;
        } else {
            arguments << "-i" << settings->pkeyPathUser
                      << QString{settings->userNameOnClients + "@" + ip}
                      << cmd
                      << "/name" << *argFakeName;
        }

        // Start the process
        QProcess startZLeafProcess;
        startZLeafProcess.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
        startZLeafProcess.startDetached(settings->sshCmd, arguments);

        // Output message via the debug messages tab
        qDebug() << settings->sshCmd << arguments.join(" ");
    }
}

void lc::Client::StartClientBrowser(const QString *const argURL,
                                    const bool *const argFullscreen)
{
    //Declarations
    QStringList arguments;

    // Build arguments list for SSH command
    arguments << "-i" << settings->pkeyPathUser
              << QString{settings->userNameOnClients + "@" + ip}
              << "DISPLAY=:0.0"
              << settings->clientBrowserCmd
              << *argURL;

    // Add fullscreen toggle if checked
    if (*argFullscreen == true) {
        arguments << "& sleep 3 && DISPLAY=:0.0 xdotool key --clearmodifiers F11";
    }

    // Start the process
    QProcess startClientBrowserProcess;
    startClientBrowserProcess.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    startClientBrowserProcess.startDetached(settings->sshCmd, arguments);

    // Output message via the debug messages tab
    qDebug() << settings->sshCmd << arguments.join(" ");
}

void lc::Client::StopClientBrowser()
{
    //Declarations
    QStringList arguments{"-i", settings->pkeyPathUser,
                          QString{settings->userNameOnClients + "@" + ip},
                          "killall", settings->clientBrowserCmd,
                          "& sleep 1 && rm -R /home/ewfuser/.mozilla/firefox/*"};

    // Start the process
    QProcess startClientBrowserProcess;
    startClientBrowserProcess.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    startClientBrowserProcess.startDetached(settings->sshCmd, arguments);

    // Output message via the debug messages tab
    qDebug() << settings->sshCmd << arguments.join(" ");
}
