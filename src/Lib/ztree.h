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

#ifndef ZTREE_H
#define ZTREE_H

#include "global.h"

#include <QProcess>

namespace lc {

//! A class to contain running zTree instances.
/*!
  This class is element of every session and is used to handle all zTree related data.
*/
class ZTree: public QObject
{
    Q_OBJECT

public:
    ZTree(const QString &argZTreeDataTargetPath,
          const int argZTreePort, const QString &argZTreeVersionPath,
          QObject *argParent = nullptr);

signals:
    void ZTreeClosed(int argExitCode);

private:
    QProcess zTreeInstance;
};

} // namespace lc

#endif // ZTREE_H
