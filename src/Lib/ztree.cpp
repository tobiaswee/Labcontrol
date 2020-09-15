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

#include <memory>

#include <QDebug>

#include "settings.h"
#include "ztree.h"

extern std::unique_ptr< lc::Settings > settings;

lc::ZTree::ZTree( const QString &argZTreeDataTargetPath, const int &argZTreePort,
                  const QString &argZTreeVersionPath, QObject *argParent ) :
    QObject{ argParent }
{
    QStringList arguments{ QStringList{} << "-c" << "0" << settings->wineCmd
                                         << QString{ settings->zTreeInstDir + "/zTree_"
                                            + argZTreeVersionPath + "/ztree.exe" }
                                         << "/datadir" << QString{ "Z:/" + argZTreeDataTargetPath }
                                         << "/privdir" << QString{ "Z:/" + argZTreeDataTargetPath }
                                         << "/gsfdir" << QString{ "Z:/" + argZTreeDataTargetPath }
                                         << "/tempdir" << QString{ "Z:/" + argZTreeDataTargetPath }
                                         << "/leafdir" << QString{ "Z:/" + argZTreeDataTargetPath }
                                         << "/channel" << QString::number( argZTreePort - 7000 ) };

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    zTreeInstance.setProcessEnvironment( env );
    zTreeInstance.setWorkingDirectory( QDir::homePath() );
    zTreeInstance.start( settings->tasksetCmd, arguments, QIODevice::NotOpen );
    connect( &zTreeInstance, SIGNAL( finished( int ) ),
             this, SIGNAL( ZTreeClosed( int ) ) );

    qDebug() << settings->tasksetCmd << arguments.join( " " );
}
