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

#include "clientpinger.h"

lcClientPinger::lcClientPinger( const QString * const argIP, const QString * const argPingCommand, QObject *argParent ) :
    QObject{ argParent },
    ip{ argIP },
    // Arguments: -c 1 (send 1 ECHO_REQUEST packet) -w 1 (timeout after 1 second) -q (quiet output)
    pingArguments{ new QStringList{ QStringList{} << "-c" << "1" << "-w" << "1" << "-q" << *ip } },
    pingCommand{ argPingCommand },
    pingProcess{ new QProcess{ this } },
    state{ state_t::UNINITIALIZED }
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    pingProcess->setProcessEnvironment(env);
    // emit ping_string(new QString(*ping_command + " " + ping_arguments->join(" ")));
}

lcClientPinger::~lcClientPinger() {
    delete pingProcess;
    delete pingArguments;
}

void lcClientPinger::doPing() {
    // Initialize the new state to be queried
    state_t newState = state_t::UNINITIALIZED;

    // Query the current state of the client
    pingProcess->start( *pingCommand, *pingArguments );
    if ( !pingProcess->waitForFinished( 2500 ) )
        newState = state_t::ERROR;
    else {
        if ( pingProcess->exitCode() == 0 )
            newState = state_t::RESPONDING;
        else
            newState = state_t::NOT_RESPONDING;
    }

    if ( newState != state ) {
        state = newState;
        // emit ping_string(new QString(name + *ping_command + " " + ping_arguments->join(" ")));
        emit PingFinished( newState );
    }
}

void lcClientPinger::setStateToZLEAF_RUNNING() {
    state = state_t::ZLEAF_RUNNING;
}
