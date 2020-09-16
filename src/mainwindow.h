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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

enum class icons_t : unsigned short int { UNKNOWN, OFF, DOWN, BOOT, ON, ZLEAF, ICON_QUANTITY };

#include "Lib/client.h"
#include "Lib/lablib.h"
#include "ui_mainwindow.h"

#include <cmath>

#include <QDir>
#include <QFileDialog>
#include <QMainWindow>
#include <QSettings>
#include <QStandardItemModel>
#include <QThread>
#include <QTimer>
#include <QVector>

namespace lc {

namespace Ui {
class MainWindow;
}

//! The class containing the graphical user interface.
/*!
  This class represents the graphical user interface and all connected functionality.
*/
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow( QWidget *argParent = nullptr );
    ~MainWindow();

private slots:
    void on_CBWebcamChooser_activated(int index);
    void on_PBBeamFile_clicked();
    void on_PBBoot_clicked();
    void on_PBChooseFile_clicked();
    void on_PBExecute_clicked();
    void on_PBKillLocalzLeaf_clicked();
    void on_PBOpenFilesystem_clicked();
    void on_PBOpenTerminal_clicked();
    void on_PBPrintPaymentFileManually_clicked();
    void on_PBRunzLeaf_clicked();
    void on_PBShowORSEE_clicked();
    void on_PBShowPreprints_clicked();
    void on_PBShutdown_clicked();
    void on_PBStartLocalzLeaf_clicked();
    void on_PBStartSession_clicked();
    void on_PBStartzLeaf_clicked();
    void on_PBViewDesktopViewOnly_clicked();
    void on_PBViewDesktopFullControl_clicked();
    void on_RBUseLocalUser_toggled(bool checked);
    void StartLocalzLeaf( QString argzLeafName, QString argzLeafVersion, int argzTreePort );
    //! Updates the icons of the QTableView displaying the clients' states
    /*!
     * This function iterates over all valid items of the 'TVClients' and sets their states
     * by querying the represented 'lcClient' instances.
     */
    void UpdateClientsTableView();

signals:
    /*Session actions*/
    void RequestNewDataTargetPath();
    void RequestNewSession( QVector< Client* > argAssocCl, QString argParticipNameReplacement,
                            bool argPrintLocalReceipts, QString argReceiptsHeader,
                            QString argzTreeDataTargetPath, quint16 argzTreePort,
                            QString argzTreeVersion );

private:
    //! Checks, if the user has administrative rights
    /*!
      @return Returns true, if the user has administrative rights (If the user is in the 'sudo' group)
    */
    bool CheckIfUserIsAdmin();
    //! Disables widgets for functions not available due to lacking devices or settings
    void DisableDisfunctionalWidgets();
    //! Loads all needed client icon QPixmaps
    void LoadIconPixmaps();
    //! Sets up all used widgets
    void SetupWidgets();

    QStandardItemModel *clients_view_model = nullptr;   //! The view storing all clients data
    QTimer *gui_update_timer = nullptr;         //! A QTimer triggering updates of the graphical user interface
    QVector< QPixmap > icons;                   //! Vector of pixmaps storing the icons indicating the clients' statuses
    Lablib *lablib = nullptr;                   //! Accumulator of all program logic being accessed by the GUI
    bool localzLeavesAreRunning = false;        //! Stores if a local z-Leaf instance is running on the server ('true' if local z-Leaf exists)
    QButtonGroup *userChooseButtonGroup = nullptr;      //! Used to group the radio buttons choosing which user shall be used for administrative client actions
    Ui::MainWindow *ui = nullptr;               //! Pointer storing all GUI items
    QVector<QStandardItem *> *valid_items = nullptr;    //! Stores all valid Client instances displayed by the table view, its main use is as iterable object for 'update_clients_table_view()'

private slots:
    void StartReceiptsHandler( QString argzTreeDataTargetPath,
                               bool argReceiptsForLocalClients,
                               QString argAnonymousReceiptsPlaceholder,
                               QString argLatexHeaderName,
                               QString argDateString );
    void on_PBstartBrowser_clicked();
    void on_PBstopBrowser_clicked();

    /* Session actions */
    void on_PBStopZtree_clicked();
    void on_PBRecoverCrashedSession_clicked();
    void GetNewDataTargetPath();
    void on_CBDataTargetPath_activated( int argIndex );
    void on_CBReceiptsHeader_activated(int argIndex);
    void on_ChBPrintanonymousreceipts_clicked();
    void on_PBKillzLeaf_clicked();
    void on_PBEnableRMB_clicked();
    void on_PBDisableRMB_clicked();
};

}

#endif // MAINWINDOW_H
