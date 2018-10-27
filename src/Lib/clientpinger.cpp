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

lc::ClientPinger::ClientPinger(const QString &argIP,
                               const QString &argPingCommand,
                               QObject *argParent) :
    QObject{argParent},
    // Arguments: -c 1 (send 1 ECHO_REQUEST packet) -w 1 (timeout after 1 second) -q (quiet output)
    pingArguments{"-c", "1", "-w", "1", "-q", argIP},
    pingCommand{argPingCommand},
    pingProcess{std::make_unique<QProcess>()}
{
    pingProcess->setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    // emit ping_string(new QString(*ping_command + " " + ping_arguments->join(" ")));
}

void lc::ClientPinger::doPing()
{
    // Initialize the new state to be queried
    Client::EState newState = Client::EState::UNINITIALIZED;

    // Query the current state of the client
    pingProcess->start(pingCommand, pingArguments);
    if (!pingProcess->waitForFinished(2500)
            || pingProcess->exitStatus() != QProcess::NormalExit)
        newState = Client::EState::ERROR;
    else {
        if (pingProcess->exitCode() == 0) {
            newState = Client::EState::RESPONDING;
        } else {
            newState = Client::EState::NOT_RESPONDING;
        }
    }

    if (newState != state) {
        state = newState;
        emit PingFinished(state);
    }
}

void lc::ClientPinger::setStateToZLEAF_RUNNING()
{
    state = Client::EState::ZLEAF_RUNNING;
}
