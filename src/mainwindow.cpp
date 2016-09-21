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

#include "mainwindow.h"

#include <QtGlobal>
#include <QInputDialog>

lcMainWindow::lcMainWindow( QWidget *argParent ) :
    QMainWindow{ argParent },
    icons{ new QPixmap[ icons_t::ICON_QUANTITY ] },
    ui{ new Ui::MainWindow }
{
    ui->setupUi( this );
    lablib = new lcLablib{ ui->PTEDebugMessages, this };

    LoadIconPixmaps();

    SetupWidgets();
    if ( valid_items ) {
        gui_update_timer = new QTimer{ this };
        connect(gui_update_timer, &QTimer::timeout, this, &lcMainWindow::UpdateClientsTableView );
        gui_update_timer->start( 500 );
    }
}

lcMainWindow::~lcMainWindow() {
    delete ui;
    delete valid_items;
    delete[] icons;
}

bool lcMainWindow::CheckIfUserIsAdmin() {
    // Query the current user's name or give an error if this fails
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString userName{ "" };
    // For Linux
    if ( env.contains( "USER" )) {
        userName = env.value( "USER", "" );
    }
    // For Windows
    if ( env.contains( "USERNAME" )) {
        userName = env.value( "USERNAME", "" );
    }
    if ( userName == "" ) {
        QMessageBox messageBox{ QMessageBox::Warning, tr( "User not detectable" ),
                    tr( "Your user name could not be queryed. The admin tab will be disabled. You won't be able to perform administrative actions but can conduct experiments normally." ), QMessageBox::Ok };
        messageBox.exec();
        return false;
    }
    ui->PTEDebugMessages->appendPlainText( tr( "[DEBUG] The user's name is %1." ).arg( userName ) );
    lablib->SetUserNameOnServer( userName );

    QStringList *adminUsers = lablib->GetAdminUsers();

    if ( !( adminUsers == nullptr ) ) {
        for ( auto s : *adminUsers ) {
            if ( s == userName ) {
                ui->PTEDebugMessages->appendPlainText( tr( "[DEBUG] '%1' has administrative rights." ).arg( userName ) );
                return true;
            }
        }
    }

    return false;
}

void lcMainWindow::DisableDisfunctionalWidgets() {
    // Disable all functions relying on the labcontrol installation directory if it is not available
    if ( !lablib->GetSettingsItem( settingsItems_t::LABCONTROL_INSTALLATION_DIRECTORY ) ) {
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
    if ( !lablib->GetSettingsItem( settingsItems_t::FILE_MANAGER ) ) {
        ui->PBShowPreprints->setEnabled( false );
    }

    // Disable 'PBBoot', if 'network_broadcast_address' was not set
    if ( !lablib->GetSettingsItem( settingsItems_t::NETWORK_BROADCAST_ADDRESS ) ) {
        ui->PBBoot->setEnabled( false );
    }

    // Disable 'PBShowORSEE', if 'orsee_command' was not set
    if ( !lablib->GetSettingsItem( settingsItems_t::ORSEE_COMMAND ) ) {
        ui->PBShowORSEE->setEnabled( false );
    }

    // Disable all widgets needless if 'public_key_path_user' or 'user_name_on_clients' was not set
    if ( !lablib->GetSettingsItem( settingsItems_t::PUBLICKEY_PATH_USER ) || !lablib->GetSettingsItem( settingsItems_t::USER_NAME_ON_CLIENTS ) ) {
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
    if ( !lablib->GetSettingsItem( settingsItems_t::PUBLICKEY_PATH_ROOT ) ) {
        ui->RBUseUserRoot->setEnabled( false );
        ui->RBUseLocalUser->click();
    }

    if ( !lablib->GetSettingsItem( settingsItems_t::PUBLICKEY_PATH_ROOT ) && !lablib->GetSettingsItem( settingsItems_t::PUBLICKEY_PATH_USER ) ) {
        ui->GBExecuteOnEveryClient->setEnabled( false );
        ui->GBOptionsForAdminActions->setEnabled( false );
        ui->PBOpenTerminal->setEnabled( false );
    }

    // Disable beam functionality if 'rcp_command' was not set
    if ( !lablib->GetSettingsItem( settingsItems_t::RCP_COMMAND ) ) {
        ui->LEFilePath->setEnabled( false );
        ui->PBBeamFile->setEnabled( false );
        ui->PBChooseFile->setEnabled( false );
    }

    // Disable 'PBRunzLeaf' and 'PBStartzLeaf' if 'server_ip' was not set
    if ( !lablib->GetSettingsItem( settingsItems_t::SERVER_IP ) ) {
        ui->PBRunzLeaf->setEnabled( false );
        ui->PBStartzLeaf->setEnabled( false );
    }

    // Disable any actions concerning the clients if 'ssh_command' was not set
    if ( !lablib->GetSettingsItem( settingsItems_t::SSH_COMMAND ) ) {
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

    // Disable 'PBOpenTerminal' if 'terminal_emulator_command' was not set
    if ( !lablib->GetSettingsItem( settingsItems_t::TERMINAL_EMULATOR_COMMAND ) ) {
        ui->GBExecuteOnEveryClient->setEnabled( false );
        ui->PBOpenTerminal->setEnabled( false );
    }

    // Disable 'PBViewDesktop' if 'vnc_viewer' was not set
    if ( !lablib->GetSettingsItem( settingsItems_t::VNC_VIEWER ) ) {
        ui->PBViewDesktop->setEnabled( false );
    }

    // Disable 'PBBoot' if 'wakeonlan_command' was not set
    if ( !lablib->GetSettingsItem( settingsItems_t::WAKEONLAN_COMMAND ) ) {
        ui->PBBoot->setEnabled( false );
    }

    // Disable the disable screensaver function if the 'xset_command' was not set
    if ( !lablib->GetSettingsItem( settingsItems_t::XSET_COMMAND ) ) {
        ui->PBDeactivateScreensaver->setEnabled( false );
    }
}

void lcMainWindow::LoadIconPixmaps() {
    if ( !lablib->GetSettingsItem( settingsItems_t::LABCONTROL_INSTALLATION_DIRECTORY ) ) {
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
        if ( !icons[ i ].load( *lablib->GetSettingsItem( settingsItems_t::LABCONTROL_INSTALLATION_DIRECTORY ) + "/icons/" + iconNames[ i ] ) ) {
            QMessageBox::information( this, tr( "Could not load icon '%1'" ).arg( iconNames[ i ] ),
                                      tr( "The icon in '%1/icons/%2' could not be loaded." )
                                      .arg( *lablib->GetSettingsItem( settingsItems_t::LABCONTROL_INSTALLATION_DIRECTORY ) ).arg( iconNames[ i ] ), QMessageBox::Ok );
        }
    }
}

void lcMainWindow::on_CBWebcamChooser_activated( int index ) {
    if ( !( index == 0 ) ) {
        QString program{ *lablib->GetSettingsItem( settingsItems_t::LABCONTROL_INSTALLATION_DIRECTORY ) + "/webcam_display" };
        QStringList arguments;
        arguments << ui->CBWebcamChooser->currentText();

        QProcess showWebcamProcess;
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        showWebcamProcess.setProcessEnvironment( env );
        showWebcamProcess.startDetached( program, arguments );
    }
}

void lcMainWindow::on_PBBeamFile_clicked() {
    QModelIndexList activatedItems = ui->TVClients->selectionModel()->selectedIndexes();
    const QString * const publickeyPathUser = lablib->GetSettingsItem( settingsItems_t::PUBLICKEY_PATH_USER ), * const userNameOnClients = lablib->GetSettingsItem( settingsItems_t::USER_NAME_ON_CLIENTS );
    const QString fileToBeam{ ui->LEFilePath->text() };
    for ( QModelIndexList::ConstIterator it = activatedItems.cbegin(); it != activatedItems.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            lcClient *client = static_cast< lcClient * >( ( *it ).data( Qt::UserRole ).value< void * >() );
            client->BeamFile( fileToBeam, publickeyPathUser, userNameOnClients );
        }
    }
}

void lcMainWindow::on_PBBoot_clicked() {
    QModelIndexList activatedItems = ui->TVClients->selectionModel()->selectedIndexes();
    for ( QModelIndexList::ConstIterator it = activatedItems.cbegin(); it != activatedItems.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            lcClient *client = static_cast< lcClient * >( ( *it ).data( Qt::UserRole ).value< void * >() );
            client->Boot( lablib->GetSettingsItem( settingsItems_t::NETWORK_BROADCAST_ADDRESS ) );
        }
    }
}

void lcMainWindow::on_PBChooseFile_clicked() {
    QFileDialog *file_dialog = new QFileDialog{ this, tr( "Choose a file to beam" ), QDir::homePath() };
    file_dialog->setFileMode( QFileDialog::Directory );
    file_dialog->setOption( QFileDialog::DontUseNativeDialog, true );
    file_dialog->setOption( QFileDialog::ReadOnly, true );
    file_dialog->setOption( QFileDialog::ShowDirsOnly, true );

    if(file_dialog->exec()) {
        ui->LEFilePath->setText(file_dialog->selectedFiles().at(0));
        ui->PTEDebugMessages->appendPlainText( tr( "[DEBUG] Chose file '%1' for beaming." ).arg( ui->LEFilePath->text() ) );
    }
    else {
        ui->LEFilePath->setText( tr( "File choosing cancelled" ) );
        ui->PTEDebugMessages->appendPlainText( tr( "[DEBUG] File choosing cancelled" ) );
    }
    delete file_dialog;
}

void lcMainWindow::on_PBDeactivateScreensaver_clicked() {
    const QString * const publickey_path_user = lablib->GetSettingsItem( settingsItems_t::PUBLICKEY_PATH_USER ), * const user_name_on_clients = lablib->GetSettingsItem( settingsItems_t::USER_NAME_ON_CLIENTS );
    QVector< lcClient* > *clients = lablib->GetClients();
    for ( auto s : *clients ) {
        if ( s->GetClientState() >= state_t::RESPONDING )
            s->DeactiveScreensaver( publickey_path_user, user_name_on_clients );
    }
}

void lcMainWindow::on_PBExecute_clicked() {
    // This will be set to false, if the command shall be executed only on the chosen clients (that's if not all clients are up)
    bool executeOnEveryClient = true;

    // Cancel, if not all clients are up and running
    QVector< lcClient* > *clients = lablib->GetClients();
    for ( auto s: *clients ) {
        if ( !( s->name.contains( "backup", Qt::CaseInsensitive ) ) ) {
            if ( s->GetClientState() < state_t::RESPONDING ) {
                QMessageBox messageBox{ QMessageBox::Warning, tr( "Not all clients are running" ),
                            tr( "Not all clients are running. The command could not be executed on every client and should therefore be canceled to keep the clients consistent.\n\nAre you sure you want to continue only with the currently chosen clients?" ), QMessageBox::No | QMessageBox::Yes, this };
                messageBox.setDefaultButton( QMessageBox::No );
                messageBox.exec();
                clients = nullptr;
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
    QString *tempPublickeyPathUser = nullptr;
    if ( ui->RBUseUserRoot->isChecked() ) {
        tempPublickeyPathUser = lablib->GetSettingsItem( settingsItems_t::PUBLICKEY_PATH_ROOT );
    } else {
        tempPublickeyPathUser = lablib->GetSettingsItem( settingsItems_t::PUBLICKEY_PATH_USER );
    }
    const QString * const publickeyPathUser = tempPublickeyPathUser;

    // and execute it
    if ( executeOnEveryClient ) {
        ui->PTEDebugMessages->appendPlainText( tr( "[DEBUG] Executing command '%1' on every client." ).arg( command ) );
        for ( auto s: *clients ) {
            if ( !( s->name.contains( "backup", Qt::CaseInsensitive ) ) ) {
                s->OpenTerminal( command, ui->RBUseUserRoot->isChecked(), publickeyPathUser, lablib->GetSettingsItem( settingsItems_t::USER_NAME_ON_CLIENTS ) );
            }
        }
    } else {
        ui->PTEDebugMessages->appendPlainText( tr( "[DEBUG] Executing command '%1' only on chosen clients." ).arg( command ) );
        QModelIndexList activated_items = ui->TVClients->selectionModel()->selectedIndexes();
        for ( QModelIndexList::ConstIterator it = activated_items.cbegin(); it != activated_items.cend(); ++it ) {
            if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
                lcClient *client = static_cast< lcClient * >( ( *it ).data( Qt::UserRole ).value< void * >() );
                client->OpenTerminal( command, ui->RBUseUserRoot->isChecked(), publickeyPathUser, lablib->GetSettingsItem( settingsItems_t::USER_NAME_ON_CLIENTS ) );
            }
        }
    }

    clients = nullptr;
}

void lcMainWindow::on_PBKillLocalzLeaf_clicked() {
    QString program{ *lablib->GetSettingsItem( settingsItems_t::LABCONTROL_INSTALLATION_DIRECTORY ) + "/scripts/kill_zLeaf_labcontrol2.sh" };

    // Start the process
    QProcess kill_zleaf_process;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    kill_zleaf_process.setProcessEnvironment( env );
    kill_zleaf_process.startDetached( program );

    local_zLeaves_are_running = false;

    // Output message via the debug messages tab
    ui->PTEDebugMessages->appendPlainText( "[DEBUG] " + program );
}

void lcMainWindow::on_PBKillzLeaf_clicked() {
    QModelIndexList activated_items = ui->TVClients->selectionModel()->selectedIndexes();
    const QString * const publickeyPathUser = lablib->GetSettingsItem( settingsItems_t::PUBLICKEY_PATH_USER ), * const userNameOnClients = lablib->GetSettingsItem( settingsItems_t::USER_NAME_ON_CLIENTS );
    for ( QModelIndexList::ConstIterator it = activated_items.cbegin(); it != activated_items.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            lcClient *client = static_cast< lcClient * >( ( *it ).data( Qt::UserRole ).value< void * >() );
            client->KillZLeaf( publickeyPathUser, userNameOnClients );
        }
    }
}

void lcMainWindow::on_PBOpenFilesystem_clicked() {
    // Determine the correct user to be used
    QString * userToBeUsed = nullptr;
    if ( ui->RBUseUserRoot->isChecked() ) {
        userToBeUsed = new QString{ "root" };
    } else  {
        userToBeUsed = new QString{ *lablib->GetSettingsItem( settingsItems_t::USER_NAME_ON_CLIENTS ) };
    }

    QModelIndexList activated_items = ui->TVClients->selectionModel()->selectedIndexes();
    for ( QModelIndexList::ConstIterator it = activated_items.cbegin(); it != activated_items.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            lcClient *client = static_cast< lcClient * >( ( *it ).data( Qt::UserRole ).value< void * >() );
            client->OpenFilesystem( userToBeUsed );
        }
    }
    delete userToBeUsed;
}

void lcMainWindow::on_PBOpenTerminal_clicked() {
    QString *tempPublickeyPathUser = nullptr;
    if ( ui->RBUseUserRoot->isChecked() ) {
        tempPublickeyPathUser = lablib->GetSettingsItem( settingsItems_t::PUBLICKEY_PATH_ROOT );
    } else {
        tempPublickeyPathUser = lablib->GetSettingsItem( settingsItems_t::PUBLICKEY_PATH_USER );
    }
    const QString * const publickeyPathUser = tempPublickeyPathUser,
                  * const userNameOnClients = lablib->GetSettingsItem( settingsItems_t::USER_NAME_ON_CLIENTS );
    QModelIndexList activated_items = ui->TVClients->selectionModel()->selectedIndexes();
    for ( QModelIndexList::ConstIterator it = activated_items.cbegin(); it != activated_items.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            lcClient *client = static_cast< lcClient * >( ( *it ).data( Qt::UserRole ).value< void * >() );
            client->OpenTerminal( QString{}, ui->RBUseUserRoot->isChecked(), publickeyPathUser, userNameOnClients );
        }
    }
}

void lcMainWindow::on_PBPrintPaymentFileManually_clicked() {
    QFileDialog *fileDialog = new QFileDialog{ this, tr( "Please choose a payment file to print." ),
                                                QDir::homePath(), "*.pay" };
    fileDialog->setFileMode( QFileDialog::ExistingFile );
    fileDialog->setOption( QFileDialog::ReadOnly, true );
    if( fileDialog->exec() ) {
        QString fileName = fileDialog->selectedFiles().at( 0 );
        QString *dateString = new QString{ fileName.split( '/', QString::KeepEmptyParts, Qt::CaseInsensitive ).last()
        .split( '.', QString::KeepEmptyParts, Qt::CaseInsensitive ).first() };
        QString *workPath = new QString{ fileName };
        workPath->truncate( workPath->lastIndexOf( '/' ) );
        // lcReceiptsHandler *receiptsHandler = new lcReceiptsHandler{ ui->PTEDebugMessages, *workPath,
        //                         ui->CBReceiptsforLocalClients->isChecked(), lablib->GetAnonymousReceiptsPlaceholder(),
        //                         ui->CBReceiptsHeader->currentText(), lablib->GetSettingsItems(), dateString, this };
        // connect( receiptsHandler, &lcReceiptsHandler::PrintingFinished, receiptsHandler, &lcReceiptsHandler::deleteLater );
    }
    delete fileDialog;
}

void lcMainWindow::on_PBRunzLeaf_clicked() {
    // Show an error message, if no zTree version was chosen yet
    if ( ui->CBzLeafVersion->currentIndex() == 0 ) {
        QMessageBox messageBox{ QMessageBox::Warning, tr( "Unset z-Leaf version" ), tr( "There is no z-Leaf version chosen yet. Please choose one." ), QMessageBox::Ok, this };
        messageBox.exec();
        return;
    }

    // Check if more than one client is selected and issue a warning message if so
    unsigned short int numberOfSelectedClients = 0;
    QModelIndexList activatedItems = ui->TVClients->selectionModel()->selectedIndexes();
    for ( QModelIndexList::ConstIterator it = activatedItems.cbegin(); it != activatedItems.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            ++numberOfSelectedClients;
        }
    }
    ui->PTEDebugMessages->appendPlainText( tr( "[DEBUG] %1 clients are selected.").arg( QString::number( numberOfSelectedClients ) ) );
    if ( numberOfSelectedClients > 1 ) {
        QMessageBox messageBox{ QMessageBox::Information, tr( "Too many clients selected" ), tr( "There are too many clients selected in the table view on the left. Please select only one." ), QMessageBox::Ok, this };
        messageBox.exec();
    } else {
        const QString * const fakeName = new QString{ ui->CBClientNames->currentText() }, * const zLeafVersion = new QString{ ui->CBzLeafVersion->currentText() },
                * const publickeyPathUser = lablib->GetSettingsItem( settingsItems_t::PUBLICKEY_PATH_USER ), * const serverIP = lablib->GetSettingsItem( settingsItems_t::SERVER_IP ),
                * const userNameOnClients = lablib->GetSettingsItem( settingsItems_t::USER_NAME_ON_CLIENTS );
        for ( QModelIndexList::ConstIterator it = activatedItems.cbegin(); it != activatedItems.cend(); ++it ) {
            if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
                lcClient *client = static_cast< lcClient * >( ( *it ).data( Qt::UserRole ).value< void * >() );
                client->StartZLeaf( publickeyPathUser, userNameOnClients, zLeafVersion, serverIP, ui->SBzLeafPort->value(), fakeName );
            }
        }
        delete fakeName;
        delete zLeafVersion;
    }
}

void lcMainWindow::on_PBShowORSEE_clicked() {
    lablib->ShowOrsee();
}

void lcMainWindow::on_PBShowPreprints_clicked() {
    lablib->ShowPreprints();
}

void lcMainWindow::on_PBShowSessions_clicked() {
    QWidget *sessionDisplay = new lcSessionDisplay{ lablib->GetSessionsModel(), this };
    sessionDisplay->setWindowFlags( Qt::Window );
    sessionDisplay->show();
    connect( sessionDisplay, &lcSessionDisplay::destroyed, sessionDisplay, &lcSessionDisplay::deleteLater );
}

void lcMainWindow::on_PBShutdown_clicked() {
    QModelIndexList activatedItems = ui->TVClients->selectionModel()->selectedIndexes();
    const QString * const publickeyPathUser = lablib->GetSettingsItem( settingsItems_t::PUBLICKEY_PATH_USER ),
                  * const userNameOnClients = lablib->GetSettingsItem( settingsItems_t::USER_NAME_ON_CLIENTS );
    for ( QModelIndexList::ConstIterator it = activatedItems.cbegin(); it != activatedItems.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            lcClient *client = static_cast< lcClient * >( ( *it ).data( Qt::UserRole ).value< void * >() );
            client->Shutdown( publickeyPathUser, userNameOnClients );
        }
    }
}

void lcMainWindow::on_PBStartLocalzLeaf_clicked() {
    // Show an error message, if no z-Leaf version was chosen yet
    if ( ui->CBzLeafVersion->currentIndex() == 0 ) {
        QMessageBox::information( this, tr( "Unset z-Leaf version" ), tr( "There is no z-Leaf version chosen yet. Please choose one." ), QMessageBox::Ok );
        return;
    }

    // Create a QMessageBox for user interaction if there is already a z-Leaf running
    QMessageBox *messageBox = nullptr;
    if ( local_zLeaves_are_running ) {
        messageBox = new QMessageBox{ QMessageBox::Warning, tr( "Running local zLeaf found" ), tr( "There already seems to exist a local zLeaf instance" ), QMessageBox::No | QMessageBox::Yes, this };
        messageBox->setInformativeText( "Do you want to start a local zLeaf nonetheless?" );
        messageBox->setDefaultButton( QMessageBox::No );
        messageBox->exec();
    }

    if ( ( messageBox != nullptr && messageBox->clickedButton() == messageBox->button( QMessageBox::Yes ) ) || !local_zLeaves_are_running ) {
        // Ask for the name the local zLeaf shall have
        QString name = QInputDialog::getText( this, tr( "The local zLeaf's name" ), tr( "Please enter the name the local zLeaf shall have." ),
                                             QLineEdit::Normal, *lablib->GetLocalZLeafDefaultName() );
        lablib->SetLocalZLeafDefaultName( name );

        QString program = QString{ *lablib->GetSettingsItem( settingsItems_t::LABCONTROL_INSTALLATION_DIRECTORY ) + "/scripts/start_zLeaf_labcontrol2.sh" };
        QStringList arguments;
        arguments << ui->CBzLeafVersion->currentText() << "127.0.0.1" << QString::number( ui->SBzLeafPort->value() - 7000 ) << name;

        // Start the process
        QProcess start_zLeaf_process;
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        start_zLeaf_process.setProcessEnvironment( env );
        start_zLeaf_process.startDetached( program, arguments );

        local_zLeaves_are_running = true;

        // Output message via the debug messages tab
        ui->PTEDebugMessages->appendPlainText( "[DEBUG] " + program + " " +  arguments.join( " " ) );
    }
    delete messageBox;
}

void lcMainWindow::on_PBStartzLeaf_clicked() {
    // Show an error message, if no z-Leaf version was chosen yet
    if ( ui->CBzLeafVersion->currentIndex() == 0 ) {
        QMessageBox messageBox{ QMessageBox::Warning, tr( "Unset z-Leaf version" ), tr( "There is no z-Leaf version chosen yet. Please choose one." ), QMessageBox::Ok, this };
        messageBox.exec();
        return;
    }

    QModelIndexList activated_items = ui->TVClients->selectionModel()->selectedIndexes();
    const QString * const publickeyPathUser = lablib->GetSettingsItem( settingsItems_t::PUBLICKEY_PATH_USER ),
                  * const serverIP = lablib->GetSettingsItem( settingsItems_t::SERVER_IP ),
                  * const userNameOnClients = lablib->GetSettingsItem( settingsItems_t::USER_NAME_ON_CLIENTS ),
                  * const zLeafVersion = new QString{ ui->CBzLeafVersion->currentText() };
    for ( QModelIndexList::ConstIterator it = activated_items.cbegin(); it != activated_items.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            lcClient *client = static_cast< lcClient * >( ( *it ).data( Qt::UserRole ).value< void * >() );
            client->StartZLeaf( publickeyPathUser, userNameOnClients, zLeafVersion, serverIP, ui->SBzLeafPort->value(), nullptr );
        }
    }
    delete zLeafVersion;
}

void lcMainWindow::on_PBStartzTree_clicked() {
    lcSessionStarter *sessionStarter = new lcSessionStarter{ lablib, ui->PTEDebugMessages, this };
    sessionStarter->setWindowFlags( Qt::Window );
    sessionStarter->show();
    connect( sessionStarter, &lcSessionStarter::destroyed, sessionStarter, &lcSessionStarter::deleteLater );
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

//    // Show an error message, if there is already a zTree instance using the currently chosen port
//    QVector<int> *occupiedPorts = lablib->GetOccupiedPorts();
//    if ( occupiedPorts->length() != 0 ) {
//        int chosenPort = ui->SBPort->value();
//        for ( auto cit = occupiedPorts->cbegin(); cit != occupiedPorts->cend(); ++cit ) {
//            if ( *cit == chosenPort ) {
//                QMessageBox messageBox{ QMessageBox::Warning, tr( "Port is already used" ),
//                            tr( "The chosen port is already used by another started zTree instance.\nPlease choose another port." ), QMessageBox::Ok, this };
//                messageBox.exec();
//                occupiedPorts = nullptr;
//                return;
//            }
//        }
//    }
//    occupiedPorts = nullptr;

//    ui->CBDataTargetPath->setStyleSheet( "" );
//    ui->CBPrintanonymousreceipts->setStyleSheet( "" );
//    ui->CBReceiptsHeader->setStyleSheet( "" );
//    ui->CBReceiptsforLocalClients->setStyleSheet( "" );
//    ui->SBPort->setStyleSheet( "" );

//    lablib->StartNewZTreeInstance();
}

void lcMainWindow::on_PBViewDesktop_clicked() {
    QModelIndexList activatedItems = ui->TVClients->selectionModel()->selectedIndexes();
    for ( QModelIndexList::ConstIterator it = activatedItems.cbegin(); it != activatedItems.cend(); ++it ) {
        if ( ( *it ).data( Qt::DisplayRole ).type() != 0 ) {
            lcClient *client = static_cast< lcClient * >( ( *it ).data( Qt::UserRole ).value< void * >() );
            client->ShowDesktop();
        }
    }
}

void lcMainWindow::on_RBUseLocalUser_toggled(bool checked) {
    if ( checked ) {
        ui->PTEDebugMessages->appendPlainText( tr( "[DEBUG] RBUseLocalUser got toggled." ) );
    }
}

void lcMainWindow::SetupWidgets() {
    // Fill the 'CBClientNames' with possible client names and the 'TVClients' with the clients
    const QVector< lcClient* > *clients = lablib->GetClients();
    if ( !( clients == nullptr ) ) {
        valid_items = new QVector< QStandardItem * >;
        valid_items->reserve( clients->size() );
        clients_view_model = new QStandardItemModel{ this };
        for ( auto *s : *clients ) {
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
    ui->CBWebcamChooser->addItem( "Choose a webcam to view:", QVariant( 0 ) );
    QStringList *webcams = lablib->GetWebcams();
    if ( !( webcams == nullptr ) ) {
        for ( auto s : *webcams )
            ui->CBWebcamChooser->addItem( s, QVariant( 0 ) );
    }

    // Append available client webcams to the 'CBWebcamChooser'
    if ( !( clients == nullptr ) ) {
        for ( auto *s : *clients ) {
            if ( s->hasWebcam )
                ui->CBWebcamChooser->addItem( s->name, QVariant( 1 ) );
        }
    }
    // Deactivate the webcam choosing interface, if no webcams are available (none mounted at clients and none mounted in the laboratory)
    if ( ui->CBWebcamChooser->count() == 1 ) {
        ui->LWebcamChooser->setEnabled( false );
        ui->CBWebcamChooser->setEnabled( false );
        QMessageBox messageBox{ QMessageBox::Warning, tr( "Could not add webcams" ),
                    tr( "The initialization of the webcams failed. Please check the configuration file of Labcontrol." ), QMessageBox::Ok, this };
        messageBox.exec();
    }
    clients = nullptr;
    webcams = nullptr;

    const QStringList *zTreeEntries = lablib->GetInstalledZTreeVersions();
    if ( zTreeEntries == nullptr ) {
        ui->CBClientNames->setEnabled( false );
        ui->GBzTree->setEnabled( false );
        ui->GLzLeafSettings->setEnabled( false );
        ui->LFakeName->setEnabled( false );
        ui->PBRunzLeaf->setEnabled( false );
        ui->PBStartLocalzLeaf->setEnabled( false );
        ui->PBStartzLeaf->setEnabled( false );
    } else {
        ui->CBzLeafVersion->addItem( "NONE" );
        for ( auto zTreeVersionString : *zTreeEntries ) {
            ui->CBzLeafVersion->addItem( zTreeVersionString );
        }
        ui->CBzLeafVersion->setCurrentIndex( 0 );
    }

    // Disable the admin tab if the user has no administrative rights and set it up
    if ( CheckIfUserIsAdmin() ) {
        ui->TAdminActions->setEnabled( true );
        ui->LAdministrativeRights->setText( tr( "You have administrative rights." ) );
    } else {
        ui->LAdministrativeRights->setText( tr( "You don't have administrative rights." ) );
    }
    ui->LUserName->setText( tr( "You are user %1" ).arg( lablib->GetUserNameOnServer() ) );
    if ( lablib->GetSettingsItem( settingsItems_t::USER_NAME_ON_CLIENTS ) ) {
        ui->RBUseLocalUser->setText( *lablib->GetSettingsItem( settingsItems_t::USER_NAME_ON_CLIENTS ) );
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
    if ( lablib->GetSettingsItem( settingsItems_t::USER_NAME_ON_CLIENTS ) ) {
        ui->CBCommandToExecute->addItems( QStringList{} << "" << "apt update" << "apt full-upgrade -y" << "reboot" << "rm -rfv /home/" + *lablib->GetSettingsItem( settingsItems_t::USER_NAME_ON_CLIENTS ) + "/.mozilla" << "uname -a" );
    } else {
        ui->CBCommandToExecute->addItems( QStringList{} << "" << "apt update" << "apt full-upgrade -y" << "reboot" << "uname -a" );
    }

    DisableDisfunctionalWidgets();

    const QString date{ __DATE__ };
    const QString time{ __TIME__ };
    // Set the info text in LInfo on the TInfo tab
    ui->LInfo->setText( "This is Labcontrol.\n\nCopyright 2014-2015 Markus Prasser\n\n\n"
                       "Labcontrol is free software: you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation"
                       ", either version 3 of the License, or\n(at your option) any later version.\n\n"
                       "Labcontrol is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\nSee the GNU General Public License for more details.\n\n"
                       "You should have received a copy of the GNU General Public License\nalong with Labcontrol.  If not, see <http://www.gnu.org/licenses/>.\n\n\n"
                       "This version of Labcontrol was compiled on " + date + " at " + time + "." );
}

void lcMainWindow::UpdateClientsTableView() {
    for ( auto s : *valid_items ) {
        state_t state = static_cast< lcClient * >( s->data( Qt::UserRole ).value<void *>() )->GetClientState();
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