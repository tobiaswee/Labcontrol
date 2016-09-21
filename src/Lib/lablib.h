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

//! This class represents the entire lab and running sessions.
/*!
  This class contains elements and functions needed to represent the lab and running sessions.
*/
class lcLablib : public QObject
{
    Q_OBJECT
public:
    /** Lablib's constructor
     *
     * @param argDebugMessagesTextEdit      A pointer to the debug messages text edit for verbose output
     * @param argParent                     This 'lcLablib' instance's parent QObject
     */
    lcLablib( QPlainTextEdit *argDebugMessagesTextEdit, QObject *argParent = nullptr );
    /** Lablib's destructor
     */
    ~lcLablib();
    /*! Returns the users who have administrative rights
     *
     * @return The users with administrative rights
     */
    QStringList *GetAdminUsers() const { return adminUsers; }
    /*! Returns the placeholder for anonymous receipts
     *
     * @return The placeholder for anonymous receipts
     */
    QString GetAnonymousReceiptsPlaceholder() const { return anonymousReceiptsPlaceholder; }
    /*! Returns the currently set port number of zTree
     *
     * @return The currently set port number for zTree
     */
    int GetChosenZTreePort() const { return chosenZTreePort; }
    /** Returns a pointer to the clients known to Lablib
     *
     * @return A QVector of pointers to the Client class instances
     */
    QVector<lcClient*> *GetClients () const { return clients; }
    /** Returns the default receipt index for the 'CBReceipts' combobox
     *
     * @return The default receipt index for the 'CBReceipts' combobox
     */
    int GetDefaultReceiptIndex () const { return defaultReceiptIndex; }
    /** Returns a pointer to the QString storing the default name for local zLeafs
     *
     * @return A pointer to the QString storing the default name for local zLeafs
     */
    QString *GetLocalZLeafDefaultName() const { return ( *settingsItems )[ ( int )settingsItems_t::LOCAL_ZLEAF_NAME ]; }
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
    /** Returns a pointer to a specified QString stored in 'settings_items' storing all command QString pointers
     *
     * @return A pointer to to a specified QString stored in 'settings_items' storing all command QString pointers
     */
    QString *GetSettingsItem( settingsItems_t argItem ) const { return ( *settingsItems )[ ( int )argItem ]; }
    /** Returns a pointer to the 'settingsItems' storing all command QString pointers
     *
     * @return A pointer to the 'settingsItems' storing all command QString pointers
     */
    QVector<QString*> *GetSettingsItems() const { return settingsItems; }
    /** Returns a pointer to server's user's name
     *
     * @return A pointer to the server's user's name
     */
    QString GetUserNameOnServer () const { return userNameOnServer; }
    /** Returns a QStringList containing all available LaTeX headers of this system
     *
     * @return A pointer to a QStringList containing all available LaTeX headers
     */
    QStringList *GetInstalledLaTeXHeaders () const {return installedLaTeXHeaders; }
    /** Returns a QStringList containing all available zTree versions of this system
     *
     * @return A pointer to a QStringList containing all available zTree versions
     */
    QStringList *GetInstalledZTreeVersions () const { return InstalledZTreeVersions; }
    /** Returns a pointer to a QStringList containing all available webcams
     *
     * @return A pointer to a QStringList containing all available webcams
     */
    QStringList *GetWebcams () const { return webcams; }
    void SetAnonymousReceiptsPlaceholder( const QString &argPlaceHolder );
    void SetChosenLaTeXHeader( const QString &argLatexHeader );
    void SetChosenZTreeDataTargetPath( const QString &argZTreeDataTargetPath );
    void SetChosenZTreePort( const int &argPort );
    void SetChosenZTreeVersion( const QString &argZTreeVersion );
    //! Sets the default name of local zLeaf instances
    /**
     * @param argName   The default name local zLeaf instances shall have
     */
    void SetLocalZLeafDefaultName( const QString &argName );
    void SetPrintReceiptsForLocalClients( const bool &argPrintReceiptsForLocalClients );
    void SetUserNameOnServer( const QString &argUserName );
    void ShowOrsee();
    void ShowPreprints();
    void StartNewZTreeInstance();

signals:
    void ZLEAF_RUNNING( QString argClientIP );

private slots:
    //! Gets the output from NetstatAgent
    void GotNetstatQueryResult( QStringList *argActiveZLeafConnections );

private:
    //! Checks if the specified path exists and complains if not so
    /**
     * @param argPath           The path which shall be check for existance
     * @param argVariableName   The name of the QSettings variable referring the path
     * @param argComplaint      The complaint which shall be added to debugging output
     * @return                  True, if path exists, false if not
     */
    bool CheckPathAndComplain( const QString * const argPath, const QString &argVariableName, const QString &argComplaint );
    //! Detects installed zTree version and LaTeX headers
    void DetectInstalledZTreeVersionsAndLaTeXHeaders();
    /** Reads all settings from the QSettings 'labSettings' object.
     */
    void ReadSettings();
    //! Loads a single settings item from the globally accessible QSettings instance
    /**
     * @param arg_variable_name The name of the variable to load
     * @param arg_complaint The complaint to display if the variable could not be loaded
     * @param arg_is_file If the existance of the file referenced by the setting's value shall be checked
     * @return The settings data or a nullptr on failure
     */
    QString *ReadSettingsItem( const QString &argVariableName, const QString &argComplaint, bool argItemIsFile = false );

    QStringList *adminUsers = nullptr;         //! Stores all users with administrative rights
    QString anonymousReceiptsPlaceholder;    //! The placeholder which shall substitute the client names on anonymous receipts (if != "")
    QString chosenLaTeXHeader;
    QString chosenZTreeDataTargetPath;
    int chosenZTreePort = 7000;               //! Stores the currently chosen port for new zTree instances
    QString chosenZTreeVersion;
    lcClientHelpNotificationServer *clientHelpNotificationServer = nullptr;    //! A server to retrieve help requests from the clients
    unsigned short int clientHelpNotificationServerPort = 0;        //! The port the help requests shall be received on
    QMap< QString, lcClient* > * clientIPsToClientsMap = nullptr;    //! A map container storing ip-client pairs
    QVector<lcClient*> *clients = nullptr;            //! A QVector storing pointers to all Client instances
    QPlainTextEdit *debugMessagesTextEdit = nullptr;       //! This stores a pointer to the text edit in the debug tab to be able to write to it
    int defaultReceiptIndex = 0;                  //! Stores the index of the LaTeX header to be displayed by default
    QStringList *installedLaTeXHeaders = nullptr;
    QStringList *InstalledZTreeVersions = nullptr;
    QSettings labSettings;
    lcNetstatAgent *netstatAgent = nullptr;         //! Tries to detect active zLeaf connections from the clients
    QThread netstatThread;
    QTimer *netstatTimer = nullptr;                //! A timer for regular execution of the NetstatAgent instance's request mechanism
    QVector< int > *occupiedPorts = nullptr;
    bool PrintReceiptsForLocalClients = true;
    SessionsModel *sessionsModel = nullptr;        //! A derivation from QAbstractTableModel used to store the single Session instances
    QVector<QString*> *settingsItems = nullptr;    //! A QVector storing all simple settings
    QString userNameOnServer;         //! The user name on the server as extracted from the environment variables
    QStringList *webcams = nullptr;                 //! A QStringList containing all available stationary webcams in the laboratory
};

#endif // LABLIB_H
