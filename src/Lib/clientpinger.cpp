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

#include "clientpinger.h"

/*!
 * \brief Construct a new ClientPinger instance
 *
 * \param[in] argIP The IP address of the client to be pinged
 * \param[in] argPingCommand The path to the ping command which shall be used
 * \param[in] argParent The instance's parent QObject
 */
lc::ClientPinger::ClientPinger(const QString &argIP,
                               const QString &argPingCommand,
                               QObject *const argParent)
    : QObject{argParent},
      // Arguments: -c 1 (send 1 ECHO_REQUEST packet) -w 1 (timeout after 1
      // second) -q (quiet output)
      pingArguments{"-c", "1", "-w", "1", "-q", argIP},
      pingCommand{argPingCommand}, pingProcess{new QProcess{this}} {
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  pingProcess->setProcessEnvironment(env);
}

//! Destroy the ClientPinger instance
lc::ClientPinger::~ClientPinger() { pingProcess->deleteLater(); }

/*!
 * \brief Execute a ping and emit ClientStateChanged if the recognized state
 * changes
 */
void lc::ClientPinger::doPing() {
  // Default-initialize the new state to be queried
  Client::State newState = Client::State::ERROR;

  // Query the current state of the client
  pingProcess->start(pingCommand, pingArguments);
  if (pingProcess->waitForFinished(2500)) {
    if (pingProcess->exitCode() == 0)
      newState = Client::State::RESPONDING;
    else
      newState = Client::State::NOT_RESPONDING;
  }

  if (newState != state) {
    state = newState;
    emit ClientStateChanged(newState);
  }
}

/*!
 * \brief Retrieve the information that a z-Leaf is running
 *
 * This is needed since otherwise restarting will not result in status updates.
 */
void lc::ClientPinger::setStateToZLEAF_RUNNING() noexcept {
  state = Client::State::ZLEAF_RUNNING;
}
