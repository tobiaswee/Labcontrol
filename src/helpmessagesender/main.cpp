/*
 * Copyright 2018 Markus Prasser
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
#include <QSettings>

#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app{argc, argv};

    QSettings labSettings{"Economic Laboratory", "Labcontrol"};

    bool convSuccess = false;
    const auto portNum{
        static_cast<quint16>(labSettings.value("client_help_server_port", "0")
                             .toUInt(&convSuccess))};
    if (convSuccess == false) {
        std::cerr << "Failed to convert \"client_help_server_port\" setting\n";
        return 1;
    }

    lcHelpMessageWindow w{labSettings.value("server_ip", "127.0.0.1").toString(),
                          portNum};
    w.show();

    return app.exec();
}
