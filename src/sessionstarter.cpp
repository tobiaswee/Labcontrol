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

#include <QDir>
#include <QFileDialog>
#include <QStandardItemModel>

#include "sessionstarter.h"
#include "ui_sessionstarter.h"
#include "Lib/settings.h"

extern std::unique_ptr< lc::Settings > settings;

lc::SessionStarter::SessionStarter( const QVector< quint16 > &argOccupiedPorts,
                                    QWidget *argParent ) :
    QWidget{ argParent },
    clientsViewModel{ new QStandardItemModel{ this } },
    occupiedPorts{ argOccupiedPorts },
    ui{ new Ui::SessionStarter }
{
    ui->setupUi( this );

    CheckIfPortIsOccupied( settings->GetChosenZTreePort() );

    if ( settings->dvipsCmd.isEmpty() || settings->latexCmd.isEmpty()
         || settings->lcInstDir.isEmpty() || settings->lprCmd.isEmpty()
         || settings->postscriptViewer.isEmpty() || settings->ps2pdfCmd.isEmpty()
         || settings->rmCmd.isEmpty() || settings->vncViewer.isEmpty() ) {
        ui->GBReceipts->setEnabled( false );
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

    ui->CBzTreeVersion->addItem( tr( "Please choose a version:" ) );
    ui->CBzTreeVersion->addItems( settings->installedZTreeVersions );

    ui->CBDataTargetPath->addItem( tr( "Set a new path HERE" ) );
    ui->CBDataTargetPath->addItem( QDir::homePath() );
    ui->CBDataTargetPath->addItem( QDir::homePath() + "/zTreeData" );
    ui->CBDataTargetPath->setCurrentIndex( 2 );
    connect( this, &SessionStarter::RequestNewDataTargetPath,
             this, &SessionStarter::GetNewDataTargetPath );

    for ( auto *s : settings->GetClients() ) {
        int tempXPos = s->xPosition - 1, tempYPos = s->yPosition;

        if ( clientsViewModel->item( tempYPos, tempXPos ) ) {
            QMessageBox::information( this, tr( "Double assignment to one position" ),
                                      tr( "Two client where set for the same position. Client"
                                          " '%1' will be dropped." ).arg( s->name ) );
            continue;
        }

        QStandardItem *item = new QStandardItem( s->name );
        item->setEditable( false );
        const QString sessionPort{ QString::number( s->GetSessionPort() ) };
        if ( sessionPort == "0" ) {
            item->setText( s->name );
        } else {
            item->setBackground( QBrush{ QColor{ 196, 196, 255 } } );
            item->setSelectable( false );
            item->setText( QString{ "(" + sessionPort + ") " + s->name } );
        }
        QVariant v = qVariantFromValue( static_cast< void* >( s ) );
        item->setData( v, Qt::UserRole );
        clientsViewModel->setItem( tempYPos, tempXPos, item );
    }
    ui->TVClients->setModel( clientsViewModel );
}

lc::SessionStarter::~SessionStarter() {
    delete ui;
}
void lc::SessionStarter::CheckIfPortIsOccupied( quint16 argPort ) {
    if ( occupiedPorts.contains( argPort ) ) {
        CheckIfPortIsOccupied( argPort + 1 );
    } else {
        ui->SBPort->setValue( argPort );
    }
}

void lc::SessionStarter::GetNewDataTargetPath() {
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

void lc::SessionStarter::on_CBDataTargetPath_activated( int argIndex ) {
    if ( !argIndex ) {
        emit RequestNewDataTargetPath();
    }

    ui->CBDataTargetPath->setStyleSheet( "" );
}

void lc::SessionStarter::on_CBReceiptsHeader_activated( int argIndex ) {
    Q_UNUSED( argIndex );
    ui->CBReceiptsHeader->setStyleSheet( "" );
}

void lc::SessionStarter::on_CBzTreeVersion_activated( int argIndex ) {
    Q_UNUSED( argIndex );
    ui->CBzTreeVersion->setStyleSheet( "" );
}

void lc::SessionStarter::on_ChBPrintAnonymousReceipts_clicked( bool argChecked ) {
    ui->ChBPrintAnonymousReceipts->setStyleSheet( "" );

    ui->LReplaceParticipantNames->setEnabled( argChecked );
    ui->CBReplaceParticipantNames->setEnabled( argChecked );
}

void lc::SessionStarter::on_PBStartSession_clicked() {
    if ( ui->CBzTreeVersion->currentIndex() == 0 ) {
        QMessageBox::information( this, tr( "No z-Tree version chosen" ),
                                  tr( "A z-Tree version was not chosen, yet. This setting is"
                                      " mandatory." ), QMessageBox::Ok );
        return;
    }

    const QModelIndexList activatedItems = ui->TVClients->selectionModel()->selectedIndexes();
    if( !ui->ChBSessionWithoutClients->isChecked() ) {
        if ( !activatedItems.length() ) {
            QMessageBox::information( this, tr( "Canceled, no clients were chosen" ),
                                      tr( "The start of a new session was canceled."
                                          " Some clients have to be selected first or the"
                                          " creation of sessions without clients must be"
                                          " allowed with the checkbox close to the bottom" ) );
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

    emit RequestNewSession( associatedClients, anonymousReceiptsPlaceholder,
                            ui->ChBReceiptsForLocalClients->isChecked(),
                            ui->CBReceiptsHeader->currentText(),
                            ui->CBDataTargetPath->currentText(),
                            static_cast< quint16 >( ui->SBPort->value() ),
                            ui->CBzTreeVersion->currentText() );

    //Start z-Leaf on selected clients if checkbox is activated
    if( ui->ChBautoStartClientZleaf->isChecked() ) {
        for ( auto cit = activatedItems.cbegin(); cit != activatedItems.cend(); ++cit ) {
            if ( ( *cit ).data( Qt::DisplayRole ).type() != 0 ) {
                Client *client = static_cast< Client* >( ( *cit ).data( Qt::UserRole ).value< void * >() );
                client->StartZLeaf( nullptr );
            }
        }
    }

    this->deleteLater();
}

void lc::SessionStarter::on_SBPort_editingFinished() {
    ui->SBPort->setStyleSheet( "" );

    CheckIfPortIsOccupied( ui->SBPort->value() );
}
