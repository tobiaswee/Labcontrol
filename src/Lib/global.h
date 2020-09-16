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

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QMetaType>

//! Opens a terminal for the client
enum class state_t : unsigned short int  {
    //! The client is booting but not yet responding
    BOOTING,
    //! An error occurred determining the client's state
    ERROR,
    //! The client is not responding to pings
    NOT_RESPONDING,
    //! The client is shutting down but not yet stopped responding
    SHUTTING_DOWN,
    //! The client's state is not yet defined (should only occur directly after client creation)
    UNINITIALIZED,
    //! The client is responding to pings
    RESPONDING,
    //! The client is running a zLeaf
    ZLEAF_RUNNING};
Q_DECLARE_METATYPE( state_t )

#endif // GLOBAL_H
