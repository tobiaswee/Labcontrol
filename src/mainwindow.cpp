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

#include <memory>

#include <QtGlobal>
#include <QDebug>
#include <QInputDialog>

#include "localzleafstarter.h"
#include "mainwindow.h"
#include "manualprintingsetup.h"
#include "Lib/settings.h"

extern std::unique_ptr< lc::Settings > settings;

lc::MainWindow::MainWindow( QWidget *argParent ) :
    QMainWindow{ argParent },
    icons( static_cast< int >( icons_t::ICON_QUANTITY ) ),
    ui{ new Ui::MainWindow }
{
    ui->setupUi( this );
    lablib = new Lablib{ this };

    LoadIconPixmaps();

    SetupWidgets();
    if ( valid_items ) {
        gui_update_timer = new QTimer{ this };
        connect( gui_update_timer, &QTimer::timeout,
                 this, &MainWindow::UpdateClientsTableView );
        gui_update_timer->start( 500 );
    }
}

lc::MainWindow::~MainWindow() {
    delete ui;
    delete valid_items;
}

bool lc::MainWindow::CheckIfUserIsAdmin() {
    if ( settings->localUserName.isEmpty() ) {
        QMessageBox messageBox{ QMessageBox::Warning, tr( "User not detectable" ),
                                tr( "Your user name could not be queryed. The admin tab will be"
                                    " disabled. You won't be able to perform administrative"
                                    " actions but can conduct experiments normally." ),
                                QMessageBox::Ok };
        messageBox.exec();
        return false;
    }

    qDebug() << "The user's name is:" << settings->localUserName;

    return lablib->CheckIfUserIsAdmin();
}

void lc::MainWindow::DisableDisfunctionalWidgets() {
    // Disable all z-Leaf killing related buttons if the 'killall' command is not available
    if ( settings->killallCmd.isEmpty() ) {
        ui->PBKillLocalzLeaf->setEnabled( false );
        ui->PBKillzLeaf->setEnabled( false );
    }
    // Disable all functions relying on the labcontrol installation directory if it is not available
    if ( settings->lcInstDir.isEmpty() ) {
        ui->CBClientNames->setEnabled( false );
        ui->CBWebcamChooser->setEnabled( false );
        ui->GBClientActions->setEnabled( false );
        ui->GBzTree->setEnabled( false );
        ui->LEFilePath->setEnabled( false );
        ui->LFakeName->setEnabled( false );
        ui->LWebcamChooser->setEnabled( false );
        ui->PBBeamFile->setEnabled( false );
        ui->PBChooseFile->setEnabled( false );
        ui->PBKillLocalzLeaf->setEnabled( false );
        ui->PBRunzLeaf->setEnabled( false );
        ui->PBShowPreprints->setEnabled( false );
        ui->PBStartLocalzLeaf->setEnabled( false );
    }

    // Disable 'PBShowPreprints', if 'file_manager' was not set
    if ( settings->fileMngr.isEmpty() ) {
        ui->PBShowPreprints->setEnabled( false );
    }

    // Disable 'PBBoot', if 'network_broadcast_address' was not set
    if ( settings->netwBrdAddr.isEmpty() ) {
        ui->PBBoot->setEnabled( false );
    }

    // Disable 'PBShowORSEE', if 'orsee_command' was not set
    if ( settings->browserCmd.isEmpty()
         || settings->orseeUrl.isEmpty() ) {
        ui->PBShowORSEE->setEnabled( false );
    }

    // Disable all widgets needless if 'public_key_path_user' or 'user_name_on_clients' was not set
    if ( settings->pkeyPathUser.isEmpty()
         || settings->userNameOnClients.isEmpty() ) {
        ui->CBClientNames->setEnabled( false );
        ui->LEFilePath->setEnabled( false );
        ui->LFakeName->setEnabled( false );
        ui->PBBeamFile->setEnabled( false );
        ui->PBChooseFile->setEnabled( false );
        ui->PBDeactivateScreensaver->setEnabled( false );
        ui->PBKillzLeaf->setEnabled( false );
        ui->PBRunzLeaf->setEnabled( false );
        ui->PBShutdown->setEnabled( false );
        ui->PBStartzLeaf->setEnabled( false );
        ui->RBUseUserRoot->click();
        ui->RBUseLocalUser->setEnabled( false );
    }

    // Disable  widgets needless if 'public_key_path_root' was not set
    if ( settings->pkeyPathRoot.isEmpty() ) {
        ui->RBUseUserRoot->setEnabled( false );
        ui->RBUseLocalUser->click();
    }

    if ( settings->pkeyPathRoot.isEmpty()
         && settings->pkeyPathUser.isEmpty() ) {
        ui->GBExecuteOnEveryClient->setEnabled( false );
        ui->GBOptionsForAdminActions->setEnabled( false );
        ui->PBOpenTerminal->setEnabled( false );
    }

    // Disable beam functionality if 'rcp_command' was not set
    if ( settings->scpCmd.isEmpty() ) {
        ui->LEFilePath->setEnabled( false );
        ui->PBBeamFile->setEnabled( false );
        ui->PBChooseFile->setEnabled( false );
    }

    // Disable 'PBRunzLeaf' and 'PBStartzLeaf' if 'server_ip' was not set
    if ( settings->serverIP.isEmpty() ) {
        ui->PBRunzLeaf->setEnabled( false );
        ui->PBStartzLeaf->setEnabled( false );
    }

    // Disable any actions concerning the clients if 'ssh_command' was not set
    if ( settings->sshCmd.isEmpty() ) {
        ui->CBClientNames->setEnabled( false );
        ui->GBClientActions->setEnabled( false );
        ui->LFakeName->setEnabled( false );
        ui->LEFilePath->setEnabled( false );
        ui->PBBeamFile->setEnabled( false );
        ui->PBChooseFile->setEnabled( false );
        ui->PBKillzLeaf->setEnabled( false );
        ui->PBRunzLeaf->setEnabled( false );
        ui->PBShutdown->setEnabled( false );
        ui->PBStartzLeaf->setEnabled( false );
        ui->TAdminActions->setEnabled( false );
    }

    if ( settings->tasksetCmd.isEmpty() ) {
        ui->CBClientNames->setEnabled( false );
        ui->LFakeName->setEnabled( false );
        ui->PBRunzLeaf->setEnabled( false );
        ui->PBStartSession->setEnabled( false );
        ui->PBStartLocalzLeaf->setEnabled( false );
        ui->PBStartzLeaf->setEnabled( false );
    }

    // Disable 'PBOpenTerminal' if 'terminal_emulator_command' was not set
    if ( settings->termEmulCmd.isEmpty() ) {
        ui->GBExecuteOnEveryClient->setEnabled( false );
        ui->PBOpenTerminal->setEnabled( false );
    }

    // Disable 'PBViewDesktop' if 'vnc_viewer' was not set
    if ( settings->vncViewer.isEmpty() ) {
        ui->PBViewDesktop->setEnabled( false );
    }

    // Disable 'PBBoot' if 'wakeonlan_command' was not set
    if ( settings->wakeonlanCmd.isEmpty() ) {
        ui->PBBoot->setEnabled( false );
    }

    // Deactivate the webcam choosing interface if no webcams are available or the viewer is missing
    if ( settings->webcamDisplayCmd.isEmpty()
         || settings->webcams.isEmpty() ) {
        ui->CBWebcamChooser->setEnabled( false );
        ui->LWebcamChooser->setEnabled( false );
    }

    if ( settings->wineCmd.isEmpty() ) {
        ui->CBClientNames->setEnabled( false );
        ui->LFakeName->setEnabled( false );
        ui->PBRunzLeaf->setEnabled( false );
        ui->PBStartSession->setEnabled( false );
        ui->PBStartLocalzLeaf->setEnabled( false );
        ui->PBStartzLeaf->setEnabled( false );
    }

    // Disable the disable screensaver function if the 'xset_command' was not set
    if ( settings->xsetCmd.isEmpty() ) {
        ui->PBDeactivateScreensaver->setEnabled( false );
    }

    if ( settings->zTreeInstDir.isEmpty() ) {
        ui->CBClientNames->setEnabled( false );
        ui->LFakeName->setEnabled( false );
        ui->PBRunzLeaf->setEnabled( false );
        ui->PBStartSession->setEnabled( false );
        ui->PBStartLocalzLeaf->setEnabled( false );
        ui->PBStartzLeaf->setEnabled( false );
    }
}

void lc::MainWindow::LoadIconPixmaps() {
    if ( settings->lcInstDir.isEmpty() ) {
        return;
    }

    const QStringList iconNames{ QStringList{}
                         << "unknown.png"
                         << "off.png"
                         << "down.png"
                         << "boot.png"
                         << "on.png"
                         << "zLeaf.png" };

    for ( int i = 0; i < ( int )icons_t::ICON_QUANTITY; i++ ) {
        if ( !icons[ i ].load( settings->lcInstDir + "/icons/" + iconNames[ i ] ) ) {
            QMessageBox::information( this, tr( "Could not load icon '%1'" ).arg( iconNames[ i ] ),
                                      tr( "The icon in '%1/icons/%2' could not be loaded." )
                                      .arg( settings->lcInstDir ).arg( iconNames[ i ] ), QMessageBox::Ok );
        }
    }
}

void lc::MainWindow::on_CBWebcamChooser_activated( int argIndex ) {
    if (  argIndex != 0 ) {
        QString program{ settings->webcamDisplayCmd };
        QStringList arguments;
        arguments << ui->CBWebcamChooser->currentText();

        QProcess showWebcamProcess;
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        showWebcamProcess.setProcessEnvironment( env );
        showWebcamProcess.startDetached( program, arguments );
    }
}

void lc::MainWindow::on_PBBeamFile_clicked() {
    QModelIndexList activatedItems = ui->TVClients->selectionModel()->selectedIndexes();
    const QString fileToBeam{ ui->LEFilePath->text() };
    for ( QModelIndexList::ConstIterator it = activatedItems.cbegin(); it != activatedItems.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            Client *client = static_cast< Client* >( ( *it ).data( Qt::UserRole ).value< void * >() );
            client->BeamFile( fileToBeam, &settings->pkeyPathUser, &settings->userNameOnClients );
        }
    }
}

void lc::MainWindow::on_PBBoot_clicked() {
    QModelIndexList activatedItems = ui->TVClients->selectionModel()->selectedIndexes();
    for ( QModelIndexList::ConstIterator it = activatedItems.cbegin(); it != activatedItems.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            Client *client = static_cast< Client* >( ( *it ).data( Qt::UserRole ).value< void * >() );
            client->Boot();
        }
    }
}

void lc::MainWindow::on_PBChooseFile_clicked() {
    QFileDialog *file_dialog = new QFileDialog{ this, tr( "Choose a file to beam" ), QDir::homePath() };
    file_dialog->setFileMode( QFileDialog::Directory );
    file_dialog->setOption( QFileDialog::DontUseNativeDialog, true );
    file_dialog->setOption( QFileDialog::ReadOnly, true );
    file_dialog->setOption( QFileDialog::ShowDirsOnly, true );

    if(file_dialog->exec()) {
        ui->LEFilePath->setText(file_dialog->selectedFiles().at(0));
        qDebug() << "Chose file" << ui->LEFilePath->text() << "for beaming.";
    }
    else {
        ui->LEFilePath->setText( tr( "File choosing cancelled" ) );
        qDebug() << "File choosing cancelled";
    }
    delete file_dialog;
}

void lc::MainWindow::on_PBDeactivateScreensaver_clicked() {
    for ( auto s : settings->GetClients() ) {
        if ( s->GetClientState() >= state_t::RESPONDING )
            s->DeactiveScreensaver();
    }
}

void lc::MainWindow::on_PBExecute_clicked() {
    // This will be set to false, if the command shall be executed only on the chosen clients (that's if not all clients are up)
    bool executeOnEveryClient = true;

    // Cancel, if not all clients are up and running
    for ( auto s: settings->GetClients() ) {
        if ( !( s->name.contains( "backup", Qt::CaseInsensitive ) ) ) {
            if ( s->GetClientState() < state_t::RESPONDING ) {
                QMessageBox messageBox{ QMessageBox::Warning, tr( "Not all clients are running" ),
                            tr( "Not all clients are running. The command could not be executed on every client and should therefore be canceled to keep the clients consistent.\n\nAre you sure you want to continue only with the currently chosen clients?" ), QMessageBox::No | QMessageBox::Yes, this };
                messageBox.setDefaultButton( QMessageBox::No );
                messageBox.exec();
                executeOnEveryClient = false;
                if ( messageBox.clickedButton() == messageBox.button( QMessageBox::No ) ) {
                    return;
                } else {
                    break;
                }
            }
        }
    }

    // Get the command to be executed ...
    QString command = ui->CBCommandToExecute->currentText();

    // Set the correct public key
    QString pkeyPathUser;
    if ( ui->RBUseUserRoot->isChecked() ) {
        pkeyPathUser = settings->pkeyPathRoot;
    } else {
        pkeyPathUser = settings->pkeyPathUser;
    }

    // and execute it
    if ( executeOnEveryClient ) {
        qDebug() << "Executing command" << command << "on every client.";
        for ( auto s: settings->GetClients() ) {
            if ( !( s->name.contains( "backup", Qt::CaseInsensitive ) ) ) {
                s->OpenTerminal( command, ui->RBUseUserRoot->isChecked() );
            }
        }
    } else {
        qDebug() << "Executing command" << command << "only on chosen clients.";
        QModelIndexList activated_items = ui->TVClients->selectionModel()->selectedIndexes();
        for ( QModelIndexList::ConstIterator it = activated_items.cbegin(); it != activated_items.cend(); ++it ) {
            if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
                Client *client = static_cast< Client* >( ( *it ).data( Qt::UserRole ).value< void * >() );
                client->OpenTerminal( command, ui->RBUseUserRoot->isChecked() );
            }
        }
    }
}

void lc::MainWindow::on_PBKillLocalzLeaf_clicked() {
    QString program{ settings->killallCmd };
    QStringList arguments;
    arguments << "-I" << "-q" << "zleaf.exe";

    // Start the process
    QProcess killLocalzLeafProc;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    killLocalzLeafProc.setProcessEnvironment( env );
    killLocalzLeafProc.startDetached( program, arguments );

    localzLeavesAreRunning = false;

    // Output message via the debug messages tab
    qDebug() << program << arguments;
}

void lc::MainWindow::on_PBKillzLeaf_clicked() {
    QModelIndexList activated_items = ui->TVClients->selectionModel()->selectedIndexes();
    for ( QModelIndexList::ConstIterator it = activated_items.cbegin(); it != activated_items.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            Client *client = static_cast< Client* >( ( *it ).data( Qt::UserRole ).value< void * >() );
            client->KillZLeaf();
        }
    }
}

void lc::MainWindow::on_PBOpenFilesystem_clicked() {
    // Determine the correct user to be used
    QString * userToBeUsed = nullptr;
    if ( ui->RBUseUserRoot->isChecked() ) {
        userToBeUsed = new QString{ "root" };
    } else  {
        userToBeUsed = new QString{ settings->userNameOnClients };
    }

    QModelIndexList activated_items = ui->TVClients->selectionModel()->selectedIndexes();
    for ( QModelIndexList::ConstIterator it = activated_items.cbegin(); it != activated_items.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            Client *client = static_cast< Client* >( ( *it ).data( Qt::UserRole ).value< void * >() );
            client->OpenFilesystem( userToBeUsed );
        }
    }
    delete userToBeUsed;
}

void lc::MainWindow::on_PBOpenTerminal_clicked() {
    QString pkeyPathUser;
    if ( ui->RBUseUserRoot->isChecked() ) {
        pkeyPathUser = settings->pkeyPathRoot;
    } else {
        pkeyPathUser = settings->pkeyPathUser;
    }
    QModelIndexList activated_items = ui->TVClients->selectionModel()->selectedIndexes();
    for ( QModelIndexList::ConstIterator it = activated_items.cbegin(); it != activated_items.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            Client *client = static_cast< Client* >( ( *it ).data( Qt::UserRole ).value< void * >() );
            client->OpenTerminal( QString{}, ui->RBUseUserRoot->isChecked() );
        }
    }
}

void lc::MainWindow::on_PBPrintPaymentFileManually_clicked() {
    ManualPrintingSetup *manPrint = new ManualPrintingSetup{ this };
    manPrint->setWindowFlags( Qt::Window );
    manPrint->show();
    connect( manPrint, SIGNAL( destroyed( QObject* ) ),
             manPrint, SLOT( deleteLater() ) );
    connect( manPrint, &ManualPrintingSetup::RequestReceiptsHandler,
             this, &MainWindow::StartReceiptsHandler );
}

void lc::MainWindow::on_PBRunzLeaf_clicked() {
    // Check if more than one client is selected and issue a warning message if so
    unsigned short int numberOfSelectedClients = 0;
    QModelIndexList activatedItems = ui->TVClients->selectionModel()->selectedIndexes();
    for ( QModelIndexList::ConstIterator it = activatedItems.cbegin(); it != activatedItems.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            ++numberOfSelectedClients;
        }
    }
    qDebug() << numberOfSelectedClients << "clients are selected.";
    if ( numberOfSelectedClients > 1 ) {
        QMessageBox messageBox{ QMessageBox::Information, tr( "Too many clients selected" ), tr( "There are too many clients selected in the table view on the left. Please select only one." ), QMessageBox::Ok, this };
        messageBox.exec();
    } else {
        const QString * const fakeName = new QString{ ui->CBClientNames->currentText() };
        for ( QModelIndexList::ConstIterator it = activatedItems.cbegin(); it != activatedItems.cend(); ++it ) {
            if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
                Client *client = static_cast< Client* >( ( *it ).data( Qt::UserRole ).value< void * >() );
                client->StartZLeaf( fakeName );
            }
        }
        delete fakeName;
    }
}

void lc::MainWindow::on_PBShowORSEE_clicked() {
    lablib->ShowOrsee();
}

void lc::MainWindow::on_PBShowPreprints_clicked() {
    lablib->ShowPreprints();
}

void lc::MainWindow::on_PBShowSessions_clicked() {
    QWidget *sessionDisplay = new SessionDisplay{ lablib->GetSessionsModel(), this };
    sessionDisplay->setWindowFlags( Qt::Window );
    sessionDisplay->show();
    connect( sessionDisplay, &SessionDisplay::destroyed,
             sessionDisplay, &SessionDisplay::deleteLater );
}

void lc::MainWindow::on_PBShutdown_clicked() {
    QModelIndexList activatedItems = ui->TVClients->selectionModel()->selectedIndexes();
    for ( QModelIndexList::ConstIterator it = activatedItems.cbegin(); it != activatedItems.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            Client *client = static_cast< Client* >( ( *it ).data( Qt::UserRole ).value< void * >() );
            // Do not shut down the server itself
            if ( client->name == "self"){
                QMessageBox::information(NULL, "Shutdown canceled", "It is not allowed to shutdown the server itself via labcontrol!");
            } else {
                client->Shutdown();
            }
        }
    }
}

void lc::MainWindow::on_PBStartLocalzLeaf_clicked() {
    LocalzLeafStarter *localzLeafStarter = new LocalzLeafStarter{ this };
    localzLeafStarter->setWindowFlags( Qt::Window );
    localzLeafStarter->show();
    connect( localzLeafStarter, &LocalzLeafStarter::LocalzLeafRequested,
             this, &MainWindow::StartLocalzLeaf );
    connect( localzLeafStarter, SIGNAL( LocalzLeafRequested( QString, QString, int ) ),
             localzLeafStarter, SLOT( deleteLater() ) );
}

void lc::MainWindow::on_PBStartSession_clicked() {
    SessionStarter *sessionStarter = new SessionStarter{ lablib->GetOccupiedPorts(), this };
    sessionStarter->setWindowFlags( Qt::Window );
    sessionStarter->show();
    connect( sessionStarter, &SessionStarter::RequestNewSession,
             lablib, &Lablib::StartNewSession );
    connect( sessionStarter, &SessionStarter::destroyed,
             sessionStarter, &SessionStarter::deleteLater );
//    // Show an error message, if no zTree version was chosen yet
//    if (ui->CBzTreeVersion->currentIndex() == 0) {
//        QMessageBox messageBox{ QMessageBox::Warning, tr("Unset zTree version"), tr("There is no zTree version chosen yet. Please choose one."), QMessageBox::Ok, this };
//        messageBox.exec();
//        return;
//    }

//    // Ask a second time, if no valid LaTeX header was set
//    if (ui->CBReceiptsHeader->currentText() == "None found") {
//        QMessageBox messageBox{ QMessageBox::Information, tr("No valid LaTeX header chosen"),
//                    tr("No valid LaTeX header was chosen. Receipts creation and printing will not work. Shall a new zTree instance be started nonetheless?"), QMessageBox::Yes | QMessageBox::No, this };
//        messageBox.exec();
//        if (messageBox.clickedButton() == messageBox.button(QMessageBox::No))
//            return;
//    }

//    ui->CBDataTargetPath->setStyleSheet( "" );
//    ui->CBPrintanonymousreceipts->setStyleSheet( "" );
//    ui->CBReceiptsHeader->setStyleSheet( "" );
//    ui->CBReceiptsforLocalClients->setStyleSheet( "" );
//    ui->SBPort->setStyleSheet( "" );

//    lablib->StartNewZTreeInstance();
}

void lc::MainWindow::on_PBStartzLeaf_clicked() {
    QModelIndexList activated_items = ui->TVClients->selectionModel()->selectedIndexes();
    for ( QModelIndexList::ConstIterator it = activated_items.cbegin(); it != activated_items.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            Client *client = static_cast< Client* >( ( *it ).data( Qt::UserRole ).value< void * >() );
            client->StartZLeaf( nullptr );
        }
    }
}

void lc::MainWindow::on_PBViewDesktop_clicked() {
    QModelIndexList activatedItems = ui->TVClients->selectionModel()->selectedIndexes();
    for ( QModelIndexList::ConstIterator it = activatedItems.cbegin(); it != activatedItems.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            Client *client = static_cast< Client* >( ( *it ).data( Qt::UserRole ).value< void * >() );
            client->ShowDesktop();
        }
    }
}

void lc::MainWindow::on_RBUseLocalUser_toggled(bool checked) {
    if ( checked ) {
        qDebug() << "'RBUseLocalUser' got toggled.";
    }
}

void lc::MainWindow::SetupWidgets() {
    // Fill the 'CBClientNames' with possible client names and the 'TVClients' with the clients
    if ( !settings->GetClients().isEmpty() ) {
        valid_items = new QVector< QStandardItem * >;
        valid_items->reserve( settings->GetClients().size() );
        clients_view_model = new QStandardItemModel{ this };
        for ( auto *s : settings->GetClients() ) {
            int temp_xpos = s->xPosition - 1, temp_ypos = s->yPosition - 1;

            // Check if a client already exists at the given position and skip, if so
            if ( clients_view_model->item( temp_ypos, temp_xpos ) ) {
                QMessageBox::information( this, tr( "Double assignment to one position" ),
                                          tr( "Two clients where set for the same position, '%1' will be dropped." ).arg( s->name ) );
                continue;
            }

            // Work to be done for the 'CBClientNames'
            ui->CBClientNames->addItem( s->name );

            // Work to be done for the 'TVClients'
            QStandardItem *item = new QStandardItem( s->name );
            item->setBackground( QBrush( QColor( 127, 255, 127, 255 ) ) );
            QVariant v = qVariantFromValue( static_cast< void * >( s ) );
            item->setData( v, Qt::UserRole );
            item->setIcon( icons[ ( int )icons_t::UNKNOWN ] );
            clients_view_model->setItem( temp_ypos, temp_xpos, item );
            valid_items->append( item );
        }
        ui->TVClients->setModel( clients_view_model );
        valid_items->squeeze();
    } else {
        QMessageBox messageBox{ QMessageBox::Warning, tr( "Could not construct clients view" ),
                    tr( "The creation of the clients view failed. Please check the file '/etc/xdg/Economic Laboratory/Labcontrol.conf'." ), QMessageBox::Ok, this };
        messageBox.exec();
        ui->CBClientNames->setEnabled( false );
        ui->GBClientActions->setEnabled( false );
        ui->LEFilePath->setEnabled( false );
        ui->LFakeName->setEnabled( false );
        ui->PBBeamFile->setEnabled( false );
        ui->PBChooseFile->setEnabled( false );
        ui->PBRunzLeaf->setEnabled( false );
        ui->TAdminActions->setEnabled( false );
        ui->TVClients->setEnabled( false );
    }

    // Fill the 'CBWebcamChooser' with all available network webcams
    if ( !settings->webcams.isEmpty() ) {
        for ( const auto &s : settings->webcams )
            ui->CBWebcamChooser->addItem( s );
    }

    const QStringList &zTreeEntries = settings->installedZTreeVersions;
    if ( zTreeEntries.isEmpty() ) {
        ui->CBClientNames->setEnabled( false );
        ui->GBzTree->setEnabled( false );
        ui->LFakeName->setEnabled( false );
        ui->PBRunzLeaf->setEnabled( false );
        ui->PBStartLocalzLeaf->setEnabled( false );
        ui->PBStartzLeaf->setEnabled( false );
    }

    // Disable the admin tab if the user has no administrative rights and set it up
    if ( CheckIfUserIsAdmin() ) {
        ui->TAdminActions->setEnabled( true );
        ui->LAdministrativeRights->setText( tr( "You have administrative rights." ) );
    } else {
        ui->LAdministrativeRights->setText( tr( "You don't have administrative rights." ) );
    }
    ui->LUserName->setText( tr( "You are user %1" ).arg( settings->localUserName ) );
    if ( !settings->userNameOnClients.isEmpty() ) {
        ui->RBUseLocalUser->setText( settings->userNameOnClients );
    } else {
        ui->RBUseUserRoot->click();
    }

    // Match the QRadioButtons correct groups
    userChooseButtonGroup = new QButtonGroup{ this };
    userChooseButtonGroup->setExclusive( true );
    userChooseButtonGroup->addButton( ui->RBUseLocalUser );
    userChooseButtonGroup->addButton( ui->RBUseUserRoot );
    ui->RBUseLocalUser->click();

    // Fill the CBCommandToExecute QComboBox
    if ( !settings->userNameOnClients.isEmpty() ) {
        ui->CBCommandToExecute->addItems( QStringList{} << "" << "apt update"
                                          << "apt full-upgrade -y" << "reboot"
                                          << "rm -rfv /home/" + settings->userNameOnClients + "/.mozilla"
                                          << "uname -a" );
    } else {
        ui->CBCommandToExecute->addItems( QStringList{} << "" << "apt update"
                                          << "apt full-upgrade -y" << "reboot" << "uname -a" );
    }

    DisableDisfunctionalWidgets();

    // Set the info text in LInfo on the TInfo tab
    ui->LInfo->setText( "This is Labcontrol.\n\nCopyright 2014-2016 Markus Prasser\n\n\n"
                        "Labcontrol is free software: you can redistribute it and/or modify\n"
                        "it under the terms of the GNU General Public License as published by\n"
                        "the Free Software Foundation, either version 3 of the License, or\n"
                        "(at your option) any later version.\n\n"
                        "Labcontrol is distributed in the hope that it will be useful,\n"
                        "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
                        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
                        "See the GNU General Public License for more details.\n\n"
                        "You should have received a copy of the GNU General Public License\n"
                        "along with Labcontrol. If not, see <http://www.gnu.org/licenses/>.\n\n\n" );
}

void lc::MainWindow::StartLocalzLeaf( QString argzLeafName, QString argzLeafVersion,
                                      int argzTreePort ) {
    if ( settings->tasksetCmd.isEmpty() || settings->wineCmd.isEmpty()
         || settings->zTreeInstDir.isEmpty() ) {
        return;
    }

    QProcess startProc;
    startProc.setProcessEnvironment( QProcessEnvironment::systemEnvironment() );
    QStringList arguments;
    arguments << "0x00000001" << settings->wineCmd
              << QString{ settings->zTreeInstDir + "/zTree_" + argzLeafVersion + "/zleaf.exe" }
              << "/server" << "127.0.0.1" << "/channel"
              << QString::number( argzTreePort - 7000 ) << "/name" << argzLeafName;
    if ( !settings->localzLeafSize.isEmpty() ) {
      arguments << "/size" << QString{ settings->localzLeafSize };
    }
	      
    startProc.startDetached( settings->tasksetCmd, arguments );
}

void lc::MainWindow::StartReceiptsHandler( QString argzTreeDataTargetPath,
                                           bool argReceiptsForLocalClients,
                                           QString argAnonymousReceiptsPlaceholder,
                                           QString argLatexHeaderName, QString argDateString) {
    ReceiptsHandler *recHand = new ReceiptsHandler{ argzTreeDataTargetPath,
                                                    argReceiptsForLocalClients,
                                                    argAnonymousReceiptsPlaceholder,
                                                    argLatexHeaderName, argDateString, this };
    connect( recHand, &ReceiptsHandler::PrintingFinished,
             recHand, &ReceiptsHandler::deleteLater );
}

void lc::MainWindow::UpdateClientsTableView() {
    for ( auto s : *valid_items ) {
        state_t state = static_cast< Client* >( s->data( Qt::UserRole ).value<void *>() )->GetClientState();
        switch ( state ) {
        case state_t::RESPONDING:
            s->setBackground( QBrush( QColor( 128, 255, 128, 255 ) ) );
            s->setIcon( icons[ ( int )icons_t::ON ] );
            break;
        case state_t::NOT_RESPONDING:
            s->setBackground( QBrush( QColor( 255, 255, 128, 255 ) ) );
            s->setIcon( icons[ ( int )icons_t::OFF ] );
            break;
        case state_t::BOOTING:
            s->setBackground( QBrush( QColor( 128, 128, 255, 255 ) ) );
            s->setIcon( icons[ ( int )icons_t::BOOT ] );
            break;
        case state_t::SHUTTING_DOWN:
            s->setBackground( QBrush( QColor( 128, 128, 255, 255 ) ) );
            s->setIcon( icons[ ( int )icons_t::DOWN ] );
            break;
        case state_t::ZLEAF_RUNNING:
            s->setBackground( QBrush( QColor( 0, 255, 0, 255 ) ) );
            s->setIcon( icons[ ( int )icons_t::ZLEAF ] );
            break;
        case state_t::UNINITIALIZED:
        case state_t::ERROR:
            s->setBackground( QBrush( QColor( 255, 128, 128, 255 ) ) );
            break;
        }
    }
}

// TODO: Implement the functionality of the script in here
void lc::MainWindow::on_PBrestartCrashedSession_clicked() {
    QProcess startProc;
    startProc.setProcessEnvironment( QProcessEnvironment::systemEnvironment() );
    if ( !settings->restartCrashedSessionScript.isEmpty() ) {
        startProc.startDetached( settings->restartCrashedSessionScript);
    }
}
