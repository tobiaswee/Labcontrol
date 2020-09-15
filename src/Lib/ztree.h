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

#ifndef ZTREE_H
#define ZTREE_H

#include <QObject>

class QProcess;

namespace lc {

/*!
 * \brief A class to contain running z-Tree instances.
 *
 * This class is element of every Session instance and is used to handle all
 * z-Tree related data as well as the execution of z-Tree itself.
 */
class ZTree : public QObject {
  Q_OBJECT

public:
  ZTree(const QString &argZTreeDataTargetPath, const int &argZTreePort,
        const QString &argZTreeVersion, QObject *argParent = nullptr);
  ~ZTree() override;

signals:
  /*!
   * \brief This is emitted if the represented z-Tree instance got closed
   *
   * \param argExitCode The exit code with which the z-Tree instance closed
   */
  void ZTreeClosed(int argExitCode);

private:
  QProcess *const zTreeInstance = nullptr;
};

} // namespace lc

#endif // ZTREE_H
