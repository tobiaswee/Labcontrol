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

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QMetaType>

//! Opens a terminal for the client
enum class state_t : unsigned short int  {
    //! The client is booting but not yet responding
    BOOTING = 1 << 0,
    //! An error occurred determining the client's state
    ERROR = 1 << 1,
    //! The client is not responding to pings
    NOT_RESPONDING = 1 << 2,
    //! The client is shutting down but not yet stopped responding
    SHUTTING_DOWN = 1 << 3,
    //! The client's state is not yet defined (should only occur directly after client creation)
    UNINITIALIZED = 1 << 4,
    //! The client is responding to pings
    RESPONDING = 1 << 5,
    //! The client is running a zLeaf
    ZLEAF_RUNNING = 1 << 6,
};
Q_DECLARE_METATYPE(state_t)

#endif // GLOBAL_H
