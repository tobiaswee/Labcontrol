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

#ifndef SESSION_H
#define SESSION_H

#include <QFileSystemWatcher>
#include <QTimer>

#include "receipts_handler.h"
#include "ztree.h"

namespace lc {

//! A class containing an entire session.
/*!
  This class represents an entire session with its zTree instance and the corresponding lcReceiptsHandler instance.
*/
class Session : public QObject {
    Q_OBJECT

public:
    const int zTreePort = 7000;                             //! The port this session's zTree instance is running on

    Session( const QString &argZTreeDataTargetPath, const quint16 argZTreePort,
             const QString &argZTreeVersionPath, bool argPrintReceiptsForLocalClients,
             const QString &argAnonymousReceiptsPlaceholder,
             const QString &argLatexHeaderName, QObject *argParent = nullptr );
    ~Session();

    /*! Returns the data item with the given index
     *
     * @param argIndex      The index of the desired item
     */
    QVariant GetDataItem( int argIndex );

    //! This gets thrown as an exception if the chosen data target path could not be created.
    class lcDataTargetPathCreationFailed {};

signals:
    void SessionFinished( Session *argSession );

private slots:
    /*! Starts the session by creating instances of the relevant classes
     */
    void InitializeClasses();
    void OnzTreeClosed( int argExitCode );
    /*! Changes zTree's window title to contain its port number to make zTree windows distinguishable
     */
    void RenameWindow();

private:
    const QString anonymousReceiptsPlaceholder;                 //! Placeholder which shall be inserted for participant names if anonymous printing is desired (QString != "")
    const QString latexHeaderName;                              //! The name of the chosen LaTeX header
    const bool printReceiptsForLocalClients = true;             //! True if receipts shall be printed for local clients
    ReceiptsHandler *receiptsHandler = nullptr;                 //! For automatic creation and printing of the receipts
    QString zTreeDataTargetPath;                                //! The path were the data of this zTree instance's session will be saved
    ZTree *zTreeInstance= nullptr;                              //! The session's zTree instance
    const QString zTreeVersionPath;                             //! The path to the version of zTree used by this session's instance
};

}

#endif // SESSION_H
