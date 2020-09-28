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
#include <QButtonGroup>
#include "localzleafstarter.h"
#include "mainwindow.h"
#include "manualprintingsetup.h"
#include "Lib/settings.h"

// for FreeBSD version detection
#include <sys/param.h>

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

   /* session actions */

    // Add z-Tree versions to the corresponding combo box
    ui->CBzTreeVersion->addItem( tr( "Please choose a version:" ) );
    ui->CBzTreeVersion->addItems( settings->installedZTreeVersions );

    // Add default path to the corresponding combo box
    ui->CBDataTargetPath->addItem( tr( "Set a new path HERE" ) );
    ui->CBDataTargetPath->addItem( QDir::homePath() );
    ui->CBDataTargetPath->addItem( QDir::homePath() + "/zTreeData" );
    ui->CBDataTargetPath->setCurrentIndex( 2 );
    connect( this, &MainWindow::RequestNewDataTargetPath,
             this, &MainWindow::GetNewDataTargetPath );

    if ( settings->dvipsCmd.isEmpty() || settings->latexCmd.isEmpty()
         || settings->lcDataDir.isEmpty() || settings->lprCmd.isEmpty()
         || settings->postscriptViewer.isEmpty() || settings->ps2pdfCmd.isEmpty()
         || settings->rmCmd.isEmpty() || settings->vncViewer.isEmpty() ) {
        QMessageBox::information( this, tr( "Receipts printing will not work" ),
                                  tr( "Some component essential for receipts creation and"
                                      " printing is missing. No receipts will be created or"
                                      " printed." ), QMessageBox::Ok );
    } else {
        ui->CBReceiptsHeader->addItems( settings->installedLaTeXHeaders );

        if ( settings->defaultReceiptIndex
             && settings->defaultReceiptIndex < ui->CBReceiptsHeader->count() ) {
            ui->CBReceiptsHeader->setCurrentIndex( settings->defaultReceiptIndex );
        }
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
    const QStringList &zTreeEntries = settings->installedZTreeVersions;
    if ( zTreeEntries.isEmpty() ) {
        ui->CBClientNames->setEnabled( false );
        //ui->GBzTree->setEnabled( false );
        ui->L_FakeName->setEnabled( false );
        ui->PBRunzLeaf->setEnabled( false );
        ui->PBStartLocalzLeaf->setEnabled( false );
        ui->PBStartzLeaf->setEnabled( false );
    }

    // Disable all z-Leaf killing related buttons if the 'killall' command is not available
    if ( settings->killallCmd.isEmpty() ) {
        ui->PBKillLocalzLeaf->setEnabled( false );
        ui->PBKillzLeaf->setEnabled( false );
    }
    // Disable all functions relying on the labcontrol installation directory if it is not available
    if ( settings->lcDataDir.isEmpty() ) {
        ui->CBClientNames->setEnabled( false );
        ui->CBWebcamChooser->setEnabled( false );
        ui->GBClientActions->setEnabled( false );
        ui->LEFilePath->setEnabled( false );
        ui->L_FakeName->setEnabled( false );
        ui->L_WebcamChooser->setEnabled( false );
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
        ui->L_FakeName->setEnabled( false );
        ui->PBBeamFile->setEnabled( false );
        ui->PBChooseFile->setEnabled( false );
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
        ui->L_FakeName->setEnabled( false );
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
        ui->L_FakeName->setEnabled( false );
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
        ui->PBViewDesktopViewOnly->setEnabled( false );
        ui->PBViewDesktopFullControl->setEnabled( false );
    }

    // Disable 'PBBoot' if 'wakeonlan_command' was not set
    if ( settings->wakeonlanCmd.isEmpty() ) {
        ui->PBBoot->setEnabled( false );
    }

    // Deactivate the webcam choosing interface if no webcams are available or the viewer is missing
    if ( settings->webcamDisplayCmd.isEmpty()
         || settings->webcams.isEmpty() ) {
        ui->CBWebcamChooser->setEnabled( false );
        ui->L_WebcamChooser->setEnabled( false );
    }

    if ( settings->wineCmd.isEmpty() ) {
        ui->CBClientNames->setEnabled( false );
        ui->L_FakeName->setEnabled( false );
        ui->PBRunzLeaf->setEnabled( false );
        //ui->PBStartSession->setEnabled( false );
        ui->PBStartLocalzLeaf->setEnabled( false );
        ui->PBStartzLeaf->setEnabled( false );
    }

    if ( settings->zTreeInstDir.isEmpty() ) {
        ui->CBClientNames->setEnabled( false );
        ui->L_FakeName->setEnabled( false );
        ui->PBRunzLeaf->setEnabled( false );
        ui->PBStartSession->setEnabled( false );
        ui->PBStartLocalzLeaf->setEnabled( false );
        ui->PBStartzLeaf->setEnabled( false );
    }
}

void lc::MainWindow::LoadIconPixmaps() {
    if ( settings->lcDataDir.isEmpty() ) {
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
        if ( !icons[ i ].load( settings->lcDataDir + "/icons/" + iconNames[ i ] ) ) {
            QMessageBox::information( this, tr( "Could not load icon '%1'" ).arg( iconNames[ i ] ),
                                      tr( "The icon in '%1/icons/%2' could not be loaded." )
                                      .arg( settings->lcDataDir ).arg( iconNames[ i ] ), QMessageBox::Ok );
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
                    tr( "The creation of the clients view failed. Please check the file '/etc/xdg/Labcontrol/Labcontrol.conf'." ), QMessageBox::Ok, this };
        messageBox.exec();
        ui->CBClientNames->setEnabled( false );
        ui->GBClientActions->setEnabled( false );
        ui->LEFilePath->setEnabled( false );
        ui->L_FakeName->setEnabled( false );
        ui->PBBeamFile->setEnabled( false );
        ui->PBChooseFile->setEnabled( false );
        ui->PBRunzLeaf->setEnabled( false );
        ui->TAdminActions->setEnabled( false );
        ui->TVClients->setEnabled( false );
    }

    // Fill the 'CBWebcamChooser' with all available network webcams
    if ( !settings->webcams.isEmpty() ) {
        for ( const auto &s : settings->webcams_names )
            ui->CBWebcamChooser->addItem( s );
    }

    // Disable the admin tab if the user has no administrative rights and set it up
    if ( CheckIfUserIsAdmin() ) {
        ui->TAdminActions->setEnabled( true );
        ui->L_AdministrativeRights->setText( tr( "You have administrative rights." ) );
    } else {
        ui->L_AdministrativeRights->setText( tr( "You don't have administrative rights." ) );
    }
    ui->L_UserName->setText( tr( "You are user %1" ).arg( settings->localUserName ) );
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

    // Disable buttons which are not configured
    //DisableDisfunctionalWidgets();

    /*// Set the info text in LInfo on the TInfo tab
    ui->LInfo->setText( "This is Labcontrol version 2.1.6\n\n\n\n\n\n"
                        "Developers\n\n"
                        "0day-2016 Henning Prömpers\n"
                        "2014-2016 Markus Prasser\n"
                        "2016 - now WiwilabHiwiOrgaization\n\n\n"
                        "Copyright\n\n"
                        "Labcontrol is free software: you can redistribute it and/or modify\n"
                        "it under the terms of the GNU General Public License as published by\n"
                        "the Free Software Foundation, either version 3 of the License, or\n"
                        "(at your option) any later version.\n\n"
                        "Labcontrol is distributed in the hope that it will be useful,\n"
                        "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
                        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
                        "See the GNU General Public License for more details.\n\n"
                        "You should have received a copy of the GNU General Public License\n"
                        "along with Labcontrol. If not, see <http://www.gnu.org/licenses/>.\n\n\n" ); */ // TODO - version number into window bar
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

/* Experiment tab functions */

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

void lc::MainWindow::on_PBBeamFile_clicked() {
    QModelIndexList activatedItems = ui->TVClients->selectionModel()->selectedIndexes();
    const QString fileToBeam{ ui->LEFilePath->text() };
    if(fileToBeam == ""){
        QMessageBox::information(this, "Upload failed", "You didn't choose any folder to upload.");
    } else {
    //Iterate over the selected clients to upload the file
    for ( QModelIndexList::ConstIterator it = activatedItems.cbegin(); it != activatedItems.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            Client *client = static_cast< Client* >( ( *it ).data( Qt::UserRole ).value< void * >() );
            client->BeamFile( fileToBeam, &settings->pkeyPathUser, &settings->userNameOnClients );
        }
    }
    // Inform the user about the path
    QMessageBox::information(this, "Upload completed", "The folder was copied to all selected clients.\nThe path on every client is /home/ewfuser/media4ztree" + fileToBeam.mid(fileToBeam.lastIndexOf('/')) +".\nDon't forget to adjust the media path within zTree!");
    }
}

void lc::MainWindow::on_PBShowORSEE_clicked() {
    lablib->ShowOrsee();
}

void lc::MainWindow::on_PBShowPreprints_clicked() {
    lablib->ShowPreprints();
}

void lc::MainWindow::on_PBShutdown_clicked() {
    // Confirmation dialog
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm", "Really shutdown the selected clients?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
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
    } else {
            qDebug() << "Canceled shutting down the selected clients";
    }
}

void lc::MainWindow::on_CBWebcamChooser_activated( int argIndex ) {
    if (  argIndex != 0 ) {
        QString program{ settings->webcamDisplayCmd };
        QStringList arguments;

        // Attention argIndex is NOT 0-based
        arguments << settings->webcams[argIndex-1];
        qDebug() << "Webcam" << arguments << "will be opened";

        QProcess showWebcamProcess;
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        showWebcamProcess.setProcessEnvironment( env );
        showWebcamProcess.startDetached( program, arguments );
    }
}

void lc::MainWindow::on_PBstartBrowser_clicked()
{
    QString argURL = ui->LEURL->text();
    bool argFullscreen = ui->CBFullscreen->checkState();
    QModelIndexList activated_items = ui->TVClients->selectionModel()->selectedIndexes();
    for ( QModelIndexList::ConstIterator it = activated_items.cbegin(); it != activated_items.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            Client *client = static_cast< Client* >( ( *it ).data( Qt::UserRole ).value< void * >() );
            client->StartClientBrowser( &argURL, &argFullscreen );
        }
    }
}

void lc::MainWindow::on_PBstopBrowser_clicked()
{
    // Confirmation dialog
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm", "Really kill all selected browser instances?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QModelIndexList activated_items = ui->TVClients->selectionModel()->selectedIndexes();
        for ( QModelIndexList::ConstIterator it = activated_items.cbegin(); it != activated_items.cend(); ++it ) {
            if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
                Client *client = static_cast< Client* >( ( *it ).data( Qt::UserRole ).value< void * >() );
                client->StopClientBrowser( );
            }
        }
    } else {
        qDebug() << "Canceled stopping all selected browser processes";
    }
}

// View only VNC button
void lc::MainWindow::on_PBViewDesktopViewOnly_clicked()
{
    QModelIndexList activatedItems = ui->TVClients->selectionModel()->selectedIndexes();
    for ( QModelIndexList::ConstIterator it = activatedItems.cbegin(); it != activatedItems.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            Client *client = static_cast< Client* >( ( *it ).data( Qt::UserRole ).value< void * >() );
            client->ShowDesktopViewOnly();
        }
    }
}

// Full control VNC button
void lc::MainWindow::on_PBViewDesktopFullControl_clicked()
{
    QModelIndexList activatedItems = ui->TVClients->selectionModel()->selectedIndexes();
    for ( QModelIndexList::ConstIterator it = activatedItems.cbegin(); it != activatedItems.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            Client *client = static_cast< Client* >( ( *it ).data( Qt::UserRole ).value< void * >() );
            client->ShowDesktopFullControl();
        }
    }
}


/* Session tab functions */
void lc::MainWindow::on_PBStartzLeaf_clicked() {
    QModelIndexList activated_items = ui->TVClients->selectionModel()->selectedIndexes();
    for ( QModelIndexList::ConstIterator it = activated_items.cbegin(); it != activated_items.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            Client *client = static_cast< Client* >( ( *it ).data( Qt::UserRole ).value< void * >() );
            client->StartZLeaf( nullptr, ui->LEzLeafCommandline->text() );
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

void lc::MainWindow::StartLocalzLeaf( QString argzLeafName, QString argzLeafVersion,
                                      int argzTreePort ) {
    if ( settings->tasksetCmd.isEmpty() || settings->wineCmd.isEmpty()
         || settings->zTreeInstDir.isEmpty() ) {
        return;
    }

    QProcess startProc;
    startProc.setProcessEnvironment( QProcessEnvironment::systemEnvironment() );
    QStringList arguments;
#if __FreeBSD__ >= 9
    arguments << QString{ settings->zTreeInstDir + "/zTree_" + argzLeafVersion + "/zleaf.exe" }
              << "/server" << "127.0.0.1" << "/channel"
              << QString::number( argzTreePort - 7000 ) << "/name" << argzLeafName;
    if ( !settings->localzLeafSize.isEmpty() ) {
      arguments << "/size" << QString{ settings->localzLeafSize };
    }

    qDebug() << "Start local zLeaf:" << arguments;
    startProc.startDetached( settings->wineCmd, arguments );
#else
    arguments << "0x00000001" << settings->wineCmd
              << QString{ settings->zTreeInstDir + "/zTree_" + argzLeafVersion + "/zleaf.exe" }
              << "/server" << "127.0.0.1" << "/channel"
              << QString::number( argzTreePort - 7000 ) << "/name" << argzLeafName;
    if ( !settings->localzLeafSize.isEmpty() ) {
      arguments << "/size" << QString{ settings->localzLeafSize };
    }

    qDebug() << "Start local zLeaf:" << arguments;
    startProc.startDetached( settings->tasksetCmd, arguments );
#endif
}

void lc::MainWindow::on_PBStopZtree_clicked()
{
    QString program{ settings->killallCmd };
    QStringList arguments;
    arguments << "-I" << "-q" << "ztree.exe";
    // Confirmation dialog
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm", "Really kill all z-Tree instances?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        // Kill all z-Tree processes
        QProcess killLocalzLeafProc;
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        killLocalzLeafProc.setProcessEnvironment( env );
        killLocalzLeafProc.startDetached( program, arguments );
        // Output message via the debug messages tab
        qDebug() << program << arguments;
    } else {
        qDebug() << "Canceled stopping all z-Tree processes";
    }
}

void lc::MainWindow::on_PBRecoverCrashedSession_clicked()
{
    // TODO: Implement the functionality of the restore session script in here (no zenity script)
        QProcess startProc;
        startProc.setProcessEnvironment( QProcessEnvironment::systemEnvironment() );
        if ( !settings->restartCrashedSessionScript.isEmpty() ) {
            startProc.startDetached( settings->restartCrashedSessionScript);
        }
}

void lc::MainWindow::on_CBDataTargetPath_activated( int argIndex )
{
    if ( !argIndex ) {
        emit RequestNewDataTargetPath();
    }

    ui->CBDataTargetPath->setStyleSheet( "" );
}

// Open a folder chooser dialog for zTree data path
void lc::MainWindow::GetNewDataTargetPath() {
    QFileDialog fileDialog{ this };
    fileDialog.setFileMode( QFileDialog::Directory );
    fileDialog.setDirectory( QDir::homePath() );
    fileDialog.setOption( QFileDialog::ShowDirsOnly, true );
    fileDialog.setOption( QFileDialog::DontUseNativeDialog, true );
    if ( fileDialog.exec() ) {
        const QString fileName = fileDialog.selectedFiles().at( 0 );
        ui->CBDataTargetPath->addItem( fileName );
        ui->CBDataTargetPath->setCurrentText( fileName );
    }
}

// Dummy function for enabling anonymous receipts section in UI
void lc::MainWindow::on_CBReceiptsHeader_activated(int argIndex)
{
    Q_UNUSED( argIndex );
    ui->CBReceiptsHeader->setStyleSheet( "" );
}

// Start session button actions
void lc::MainWindow::on_PBStartSession_clicked() {

    if ( ui->CBzTreeVersion->currentIndex() == 0 ) {
        QMessageBox::information( this, tr( "No z-Tree version chosen" ),
                                  tr( "A z-Tree version was not chosen, yet. This setting is"
                                      " mandatory." ), QMessageBox::Ok );
        return;
    }

    const QModelIndexList activatedItems = ui->TVClients->selectionModel()->selectedIndexes();
    if( !ui->ChBSessionWithoutAttachedClients->isChecked() ) {
        if ( !activatedItems.length() ) {
            QMessageBox::information( this, tr( "Canceled, no clients were chosen" ),
                                      tr( "The start of a new session was canceled.\n"
                                          " Some clients have to be selected first or the"
                                          " creation of sessions without clients must be"
                                          " allowed with the checkbox." ) );
            return;
        }
    }

    QString anonymousReceiptsPlaceholder;
    if ( ui->ChBPrintAnonymousReceipts->isChecked() ) {
        anonymousReceiptsPlaceholder = ui->CBReplaceParticipantNames->currentText();
    }

    QVector< Client* > associatedClients;
    for ( auto cit = activatedItems.cbegin(); cit != activatedItems.cend(); ++cit ) {
        if ( ( *cit ).data( Qt::DisplayRole ).type() != 0 ) {
            Client *client = static_cast< Client* >( ( *cit ).data( Qt::UserRole ).value< void* >() );
            client->SetSessionPort( ui->SBPort->value() );
            client->SetzLeafVersion( ui->CBzTreeVersion->currentText() );
            associatedClients.append( client );
        }
    }

    this->lablib->StartNewSession ( associatedClients, anonymousReceiptsPlaceholder,
                            ui->ChBReceiptsForLocalClients->isChecked(),
                            ui->CBReceiptsHeader->currentText(),
                            ui->CBDataTargetPath->currentText(),
                            static_cast< quint16 >( ui->SBPort->value() ),
                            ui->CBzTreeVersion->currentText() );

    //Display the command line
    QString cmd = this->lablib->getzLeafArgs( ui->SBPort->value(), ui->CBzTreeVersion->currentText()).join(" ");
    ui->LEzLeafCommandline->setText(cmd);

    //Start z-Leaf on selected clients if checkbox is activated
    if( ui->ChBautoStartClientZleaf->isChecked() ) {
        for ( auto cit = activatedItems.cbegin(); cit != activatedItems.cend(); ++cit ) {
            if ( ( *cit ).data( Qt::DisplayRole ).type() != 0 ) {
                Client *client = static_cast< Client* >( ( *cit ).data( Qt::UserRole ).value< void * >() );
                client->StartZLeaf( nullptr, cmd );
            }
        }
    }

    // Set chosen Port
    settings->SetChosenZTreePort(ui->SBPort->text().toInt());
    // Increment port number
    int newPort = ui->SBPort->text().toInt() + 1;
    ui->SBPort->setValue(newPort);
}

void lc::MainWindow::on_PBKillzLeaf_clicked()
{
    QModelIndexList activated_items = ui->TVClients->selectionModel()->selectedIndexes();
    for ( QModelIndexList::ConstIterator it = activated_items.cbegin(); it != activated_items.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            Client *client = static_cast< Client* >( ( *it ).data( Qt::UserRole ).value< void * >() );
            client->KillZLeaf();
        }
    }
}

/* Admin tab functions */

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

void lc::MainWindow::on_PBExecute_clicked() {

    // Get the command to be executed ...
    QString command = ui->CBCommandToExecute->currentText();

    // Set the correct public key
    QString pkeyPathUser;
    if ( ui->RBUseUserRoot->isChecked() ) {
        pkeyPathUser = settings->pkeyPathRoot;
    } else {
        pkeyPathUser = settings->pkeyPathUser;
    }

    qDebug() << "Executing command" << command << " on chosen clients.";
    QModelIndexList activated_items = ui->TVClients->selectionModel()->selectedIndexes();
    for ( QModelIndexList::ConstIterator it = activated_items.cbegin(); it != activated_items.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            Client *client = static_cast< Client* >( ( *it ).data( Qt::UserRole ).value< void * >() );
            client->OpenTerminal( command, ui->RBUseUserRoot->isChecked() );
        }
    }

}

// Issue open terminal call
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

// Enable RMB
void lc::MainWindow::on_PBEnableRMB_clicked()
{
    // Confirmation dialog
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm", "Really enable the right mouse button on selected clients?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        qDebug() << "Enabling RMB on chosen clients.";
        QModelIndexList activated_items = ui->TVClients->selectionModel()->selectedIndexes();
        for ( QModelIndexList::ConstIterator it = activated_items.cbegin(); it != activated_items.cend(); ++it ) {
            if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
                Client *client = static_cast< Client* >( ( *it ).data( Qt::UserRole ).value< void * >() );
                client->ControlRMB(true);
            }
        }
    }
}

// Disable RMB
void lc::MainWindow::on_PBDisableRMB_clicked()
{
    // Confirmation dialog
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm", "Really disable the right mouse button on selected clients?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        qDebug() << "Disabling RMB on chosen clients.";
        QModelIndexList activated_items = ui->TVClients->selectionModel()->selectedIndexes();
        for ( QModelIndexList::ConstIterator it = activated_items.cbegin(); it != activated_items.cend(); ++it ) {
            if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
                Client *client = static_cast< Client* >( ( *it ).data( Qt::UserRole ).value< void * >() );
                client->ControlRMB(false);
            }
        }
    }
}

// Anonymous receipients header check box
void lc::MainWindow::on_ChBPrintAnonymousReceipts_clicked()
{
    ui->LReplaceParticipantNames->setEnabled(true);
    ui->CBReplaceParticipantNames->setEnabled(true);
}
