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

#include "ztree.h"
#include "settings.h"

#include <QDir>

#include <memory>

extern std::unique_ptr<lc::Settings> settings;

/*!
 * Setup and start a new z-Tree instance with the given parameters
 *
 * \param[in] argZTreeDataTargetPath The path where the new z-Tree instance
 * shall write its output to
 * \param[in] argZTreePort The port on which the new z-Tree instance shall
 * listen
 * \param[in] argZTreeVersion The version of z-Tree which shall be
 * started
 * \param[in] argParent The instance's parent QObject
 */
lc::ZTree::ZTree(const QString &argZTreeDataTargetPath, const int &argZTreePort,
                 const QString &argZTreeVersion, QObject *const argParent)
    : QObject{argParent}, zTreeInstance{new QProcess{this}} {
  const QStringList arguments{"-c",
                              "0",
                              settings->wineCmd,
                              QString{settings->zTreeInstDir + "/zTree_" +
                                      argZTreeVersion + "/ztree.exe"},
                              "/datadir",
                              QString{"Z:/" + argZTreeDataTargetPath},
                              "/privdir",
                              QString{"Z:/" + argZTreeDataTargetPath},
                              "/gsfdir",
                              QString{"Z:/" + argZTreeDataTargetPath},
                              "/tempdir",
                              QString{"Z:/" + argZTreeDataTargetPath},
                              "/leafdir",
                              QString{"Z:/" + argZTreeDataTargetPath},
                              "/channel",
                              QString::number(argZTreePort - 7000)};

  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  zTreeInstance->setProcessEnvironment(env);
  zTreeInstance->setWorkingDirectory(QDir::homePath());
  zTreeInstance->start(settings->tasksetCmd, arguments, QIODevice::NotOpen);
  connect(zTreeInstance,
          static_cast<void (QProcess::*)(int)>(&QProcess::finished), this,
          &ZTree::ZTreeClosed);

  qDebug() << settings->tasksetCmd << arguments.join(" ");
}

/*!
 * \brief Destroy the z-Tree instance (closes the running z-Tree instance)
 */
lc::ZTree::~ZTree() { zTreeInstance->deleteLater(); }
