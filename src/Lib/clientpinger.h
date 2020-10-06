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

#include "client.h"

class QProcess;

namespace lc {

/*!
 * \brief Repetitively ping the client represented by the owning Client instance
 */
class ClientPinger : public QObject {
  Q_OBJECT

public:
  explicit ClientPinger(const QString &argIP, const QString &argPingCommand,
                        QObject *argParent = nullptr);
  ~ClientPinger() override;

public slots:
  void doPing();
  void setStateToZLEAF_RUNNING() noexcept;

private:
  //! The arguments passed to the "ping" command
  const QStringList pingArguments;
  //! The utilized "ping" command itself
  const QString pingCommand;
  //! The "ping" process which will be executed on every call of "doPing()"
  QProcess *const pingProcess = nullptr;
  //! Stores the currently assumed state of the client
  Client::State state = Client::State::UNINITIALIZED;

signals:
  /*!
   * \brief Signal which is being emitted if a client's state seems to have
   * changed
   *
   * This is emitted if a ping triggered by "doPing()" finished and the detected
   * change of the represented client seems to have changed.
   *
   * \param state The new state the client seems to have
   */
  void ClientStateChanged(Client::State state);
};

} // namespace lc

#endif // CLIENTPINGER_H
