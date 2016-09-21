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

#include "ztree.h"

lc::ZTree::ZTree( QPlainTextEdit *argDebugMessagesTextEdit,
                  const QString &argZTreeDataTargetPath, const int &argZTreePort,
                  const QString &argZTreeVersionPath,
                  const QVector<QString*> * const argSettingsItems ) {
    QString program{ *( *argSettingsItems )[ ( int )settingsItems_t::LABCONTROL_INSTALLATION_DIRECTORY ] + "/scripts/start_zTree_labcontrol2.sh" };
    QStringList arguments;
    arguments << *( *argSettingsItems )[ ( int )settingsItems_t::ZTREE_INSTALLATION_DIRECTORY ] << argZTreeVersionPath << argZTreeDataTargetPath << QString::number( static_cast<int>( argZTreePort ) - 7000 );

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    zTreeInstance.setProcessEnvironment( env );
    zTreeInstance.startDetached( program, arguments, QDir::currentPath(), &pid );
    connect( &zTreeInstance, SIGNAL( finished( int ) ), SLOT( ZTreeInstanceClosed() ) );

    // Output message via the debug messages tab
    argDebugMessagesTextEdit->appendPlainText( "[DEBUG] " + program + " " + arguments.join( " " ) );
}

void lc::ZTree::ZTreeInstanceClosed() {
    emit ZTreeClosed();
}
