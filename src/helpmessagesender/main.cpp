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

#include "helpmessagewindow.h"

#include <QApplication>
#include <QDebug>
#include <QSettings>

int main(int argc, char *argv[]) {
  QApplication a{argc, argv};
  QSettings labSettings{"Labcontrol", "Labclient"};

  const auto serverIP{labSettings.value("server_ip").toString()};
  if (serverIP.isEmpty()) {
    qDebug() << "Invalid laboratory server ip \"" + serverIP + "\" given";
    return 1;
  }

  const auto serverPortStr{labSettings.value("server_port").toString()};
  bool convSuccess = false;
  const auto serverPort = serverPortStr.toUInt(&convSuccess);
  if ((false == convSuccess) || ((serverPort < 1) || (serverPort > 65535))) {
    qDebug() << "Invalid laboratory server port \"" + serverPortStr +
                    "\" given";
    return 2;
  }

  lc::HelpMessageWindow w{serverIP, static_cast<uint16_t>(serverPort)};
  w.show();

  return a.exec();
}
