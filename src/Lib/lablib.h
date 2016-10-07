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

#ifndef LABLIB_H
#define LABLIB_H

#include <list>
#include <memory>

#include <QDir>
#include <QItemSelectionModel>
#include <QList>
#include <QMainWindow>
#include <QMap>
#include <QModelIndexList>
#include <QPlainTextEdit>
#include <QProcess>
#include <QSettings>
#include <QString>
#include <QTableView>
#include <QVector>
#include <QXmlStreamReader>

#include "client.h"
#include "clienthelpnotificationserver.h"
#include "global.h"
#include "netstatagent.h"
#include "session.h"
#include "sessionsmodel.h"
#include "settings.h"

extern std::unique_ptr< lc::Settings > settings;

namespace lc {

//! This class represents the entire lab and running sessions.
/*!
  This class contains elements and functions needed to represent the lab and running sessions.
*/
class Lablib : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Lablib's constructor
     * \param[in] argParent This 'lcLablib' instance's parent QObject
     */
    Lablib( QObject *argParent = nullptr );
    /** Lablib's destructor
     */
    ~Lablib();
    /*!
     * \brief CheckIfUserIsAdmin checks if the account with the passed user name has administrative rights
     * \param argUserName The account name which shall checked for administrative rights
     * \return True, if the account has administrative rights; false, otherwise
     */
    bool CheckIfUserIsAdmin() const;
    /** Returns a pointer to the clients known to Lablib
     *
     * @return A QVector of pointers to the Client class instances
     */
    QVector< Client* > *GetClients () const { return clients; }
    /** Returns a pointer to the QString storing the default name for local zLeafs
     *
     * @return A pointer to the QString storing the default name for local zLeafs
     */
    QString GetLocalZLeafDefaultName() const { return settings->GetLocalzLeafName(); }
    /** Returns a pointer to a QVector<unsigned int> containing all by sessions occupied ports
     *
     * @return A pointer to a QVector<unsigned int> containing all occupied ports
     */
    QVector<int> *GetOccupiedPorts () const { return occupiedPorts; }
    /** Returns a pointer to the QAbstractTableModel storing the Session instances
     *
     * @return A pointer to the QAbstractTableModel storing the Session instances
     */
    SessionsModel *GetSessionsModel () const { return sessionsModel; }
    /** Returns true if receipts for local clients shall be printed
     *
     * @return True if receipts for local clients shall be printed
     */
    bool GetPrintReceiptsForLocalClients() const { return PrintReceiptsForLocalClients; }
    //! Sets the default name of local zLeaf instances
    /**
     * @param argName   The default name local zLeaf instances shall have
     */
    void SetLocalZLeafDefaultName( const QString &argName );
    void SetPrintReceiptsForLocalClients( const bool &argPrintReceiptsForLocalClients );
    void ShowOrsee();
    void ShowPreprints();

public slots:
    void StartNewZTreeInstance( QString argDataTargetPath, int argPort, QString argzTreeVersion,
                                bool argReceiptsForLocalClients, QString argAnonReceiptPlaceholder,
                                QString argChosenLatexHeader );

signals:
    void ZLEAF_RUNNING( QString argClientIP );

private slots:
    //! Gets the output from NetstatAgent
    void GotNetstatQueryResult( QStringList *argActiveZLeafConnections );

private:
    //! Detects installed zTree version and LaTeX headers
    void DetectInstalledZTreeVersionsAndLaTeXHeaders();
    /** Reads all settings from the QSettings 'labSettings' object.
     */
    void ReadSettings();

    ClientHelpNotificationServer *clientHelpNotificationServer = nullptr;    //! A server to retrieve help requests from the clients
    QMap< QString, Client* > * clientIPsToClientsMap = nullptr;    //! A map container storing ip-client pairs
    QVector<Client*> *clients = nullptr;            //! A QVector storing pointers to all Client instances
    QSettings labSettings;
    NetstatAgent *netstatAgent = nullptr;           //! Tries to detect active zLeaf connections from the clients
    QThread netstatThread;
    QTimer *netstatTimer = nullptr;                //! A timer for regular execution of the NetstatAgent instance's request mechanism
    QVector< int > *occupiedPorts = nullptr;
    bool PrintReceiptsForLocalClients = true;
    SessionsModel *sessionsModel = nullptr;        //! A derivation from QAbstractTableModel used to store the single Session instances
};

}

#endif // LABLIB_H
