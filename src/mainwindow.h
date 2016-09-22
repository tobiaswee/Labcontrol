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
#include "sessionstarter.h"
#include "sessiondisplay.h"
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
    void on_PBDeactivateScreensaver_clicked();
    void on_PBExecute_clicked();
    void on_PBKillLocalzLeaf_clicked();
    void on_PBKillzLeaf_clicked();
    void on_PBOpenFilesystem_clicked();
    void on_PBOpenTerminal_clicked();
    void on_PBPrintPaymentFileManually_clicked();
    void on_PBRunzLeaf_clicked();
    void on_PBShowORSEE_clicked();
    void on_PBShowPreprints_clicked();
    void on_PBShowSessions_clicked();
    void on_PBShutdown_clicked();
    void on_PBStartLocalzLeaf_clicked();
    void on_PBStartzLeaf_clicked();
    void on_PBStartzTree_clicked();
    void on_PBViewDesktop_clicked();
    void on_RBUseLocalUser_toggled(bool checked);
    //! Updates the icons of the QTableView displaying the clients' states
    /*!
     * This function iterates over all valid items of the 'TVClients' and sets their states
     * by querying the represented 'lcClient' instances.
     */
    void UpdateClientsTableView();

signals:

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
    bool local_zLeaves_are_running = false;     //! Stores if a local zLeaf instance is running on the server ('true' if local zLeaf exists)
    QButtonGroup *userChooseButtonGroup = nullptr;      //! Used to group the radio buttons choosing which user shall be used for administrative client actions
    Ui::MainWindow *ui = nullptr;               //! Pointer storing all GUI items
    QVector<QStandardItem *> *valid_items = nullptr;    //! Stores all valid Client instances displayed by the table view, its main use is as iterable object for 'update_clients_table_view()'
};

}

#endif // MAINWINDOW_H
