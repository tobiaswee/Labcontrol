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

#ifndef CLIENTPINGER_H
#define CLIENTPINGER_H

#include <QObject>
#include <QProcess>
#include <QThread>

#include "global.h"

namespace lc {

//! The ClientPinger class is used to do repetitive pings of the owning Client
//! instance's client.
/*!
  This class is just used for executing repetitive pings.
*/
class ClientPinger : public QObject {
  Q_OBJECT

public:
  //! ClientPinger's constructor
  /*!
    @param argIP              A reference to the owning Client instance's IP
    address
    @param argPingCommand     The path were the command to be executed for pings
    resides
    @param argParent          The ClientPinger's parent owning this instance of
    it
  */
  explicit ClientPinger(const QString &argIP, const QString &argPingCommand,
                        QObject *argParent = nullptr);
  //! ClientPinger's destructor
  ~ClientPinger();

public slots:
  //! This slot executes a ping when called.
  void doPing();
  //! Retrieves the information, that zLeaf is running (otherwise restarting
  //! will not result in status updates)
  void setStateToZLEAF_RUNNING();

private:
  const QStringList pingArguments; //! The arguments for the 'ping' command
  const QString pingCommand;       //! The 'ping' command itself
  QProcess *pingProcess; //! The 'ping' process which will be executed on every
                         //! call of 'do_ping()'
  state_t state;         //! Stores the current state of the client

signals:
  //! This signal was just implemented for testing purposes
  //! This signal is emitted if the ping finished and the state of the client
  //! changed
  void PingFinished(state_t state);
};

} // namespace lc

#endif // CLIENTPINGER_H
