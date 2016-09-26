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

#include "sessionstarter.h"
#include "ui_sessionstarter.h"
#include "Lib/settings.h"

extern std::unique_ptr< lc::Settings > settings;

lc::SessionStarter::SessionStarter( Lablib *argLablib, QPlainTextEdit *argDebugMessagesTextEdit, QWidget *parent ) :
    QWidget{ parent },
    debugMessagesTextEdit { argDebugMessagesTextEdit },
    lablib{ argLablib },
    ui{ new Ui::SessionStarter }
{
    ui->setupUi( this );
    this->SetupWidgets();
}

lc::SessionStarter::~SessionStarter() {
    delete ui;
}

void lc::SessionStarter::GetNewDataTargetPath() {
    QFileDialog *file_dialog = new QFileDialog{ this };
    file_dialog->setFileMode( QFileDialog::Directory );
    file_dialog->setDirectory( QDir::homePath() );
    file_dialog->setOption( QFileDialog::ShowDirsOnly, true );
    file_dialog->setOption( QFileDialog::DontUseNativeDialog, true );
    if( file_dialog->exec() ) {
        QString file_name = file_dialog->selectedFiles().at( 0 );
        ui->CBDataTargetPath->addItem( file_name );
        ui->CBDataTargetPath->setCurrentText( file_name );
        ui->CBDataTargetPath->activated( file_name );
    }
    delete file_dialog;
}

void lc::SessionStarter::on_CBDataTargetPath_activated(const QString &arg1) {
    if ( ui->CBDataTargetPath->currentIndex() == 0 ) {
        emit NewDataTargetPathRequested();
        return;
    }
    ui->CBDataTargetPath->setStyleSheet( "" );
    lablib->SetChosenZTreeDataTargetPath( arg1 );
}

void lc::SessionStarter::on_CBReceiptsHeader_activated( const QString &argHeader ) {
    Q_UNUSED( argHeader );
    ui->CBReceiptsHeader->setStyleSheet( "" );
}

void lc::SessionStarter::on_CBzTreeVersion_activated( const QString &argVersion ) {
    Q_UNUSED( argVersion );
    ui->CBzTreeVersion->setStyleSheet( "" );
}

void lc::SessionStarter::on_ChBPrintanonymousreceipts_clicked( bool checked ) {
    // Enable or disable the corresponding widgets
    if ( checked ) {
        ui->LReplaceParticipantName->setEnabled( true );
        ui->CBReplaceParticipantName->setEnabled( true );
        ui->CBReplaceParticipantName->setStyleSheet( "background: cyan" );
    } else {
        ui->LReplaceParticipantName->setEnabled( false );
        ui->CBReplaceParticipantName->setEnabled( false );
        ui->CBReplaceParticipantName->setStyleSheet( "" );
    }
    ui->ChBPrintanonymousreceipts->setStyleSheet( "" );
}

void lc::SessionStarter::on_ChBReceiptsforLocalClients_clicked( bool checked ) {
    ui->ChBReceiptsforLocalClients->setStyleSheet( "" );
    lablib->SetPrintReceiptsForLocalClients( checked );
}

void lc::SessionStarter::on_PBStartzTree_clicked() {
    emit SessionRequested( ui->CBDataTargetPath->currentText(), ui->SBPort->value(),
                           ui->CBzTreeVersion->currentText(),
                           ui->ChBReceiptsforLocalClients->isChecked(),
                           ui->CBReplaceParticipantName->currentText(),
                           ui->CBReceiptsHeader->currentText() );
    close();
}

void lc::SessionStarter::on_SBPort_editingFinished() {
    ui->SBPort->setStyleSheet( "" );
    lablib->SetChosenZTreePort( ui->SBPort->value() );
}

void lc::SessionStarter::SetupWidgets() {
    ui->SBPort->setValue( lablib->GetChosenZTreePort() );

    // Fill the 'CBzTreeVersion' combobox with known entries from the lablib class
    ui->CBzTreeVersion->addItem( "NONE" );
    const QStringList &zTreeEntries = settings->installedZTreeVersions;
    if ( !zTreeEntries.isEmpty() ) {
        for ( const auto &zTreeVersionString : zTreeEntries ) {
            ui->CBzTreeVersion->addItem( zTreeVersionString );
        }
        ui->CBzTreeVersion->setCurrentIndex( 0 );
    } else {
        throw lcForbiddenCall{};
    }

    // Fill the 'CBReceipts' combobox with known entries from the lablib class
    const QStringList *laTeXHeaders = lablib->GetInstalledLaTeXHeaders();
    if ( laTeXHeaders ) {
        if ( ( laTeXHeaders->count() == 1 ) && ( laTeXHeaders->at(0) == "None found" ) ) {
            ui->GBReceipts->setEnabled( false );
        }
        ui->CBReceiptsHeader->addItems( *laTeXHeaders );
        if ( laTeXHeaders->length() - 1 < lablib->GetDefaultReceiptIndex() ) {
            QMessageBox::information( this, tr( "'default_receipt_index' to high" ),
                                      tr( "'default_receipt_index' was set to big. The combo box containing the receipt templates will default to the first entry." ) );
            debugMessagesTextEdit->appendPlainText( tr("'default_receipt_index' was set to big. The combo box containing the receipt templates will default to the first entry." ) );
            ui->CBReceiptsHeader->setCurrentIndex( 0 );
        } else {
            ui->CBReceiptsHeader->setCurrentIndex( lablib->GetDefaultReceiptIndex() );
        }
        laTeXHeaders = nullptr;
    }

    // Fill the 'CBDataTargetPath' combobox with some data target path examples
    ui->CBDataTargetPath->addItem( tr( "Set new path" ) );
    ui->CBDataTargetPath->addItem( QDir::homePath() );
    ui->CBDataTargetPath->addItem( QString{ QDir::homePath() + "/zTreeData" } );
    ui->CBDataTargetPath->setCurrentIndex( 2 );
    connect( this, &SessionStarter::NewDataTargetPathRequested,
             this, &SessionStarter::GetNewDataTargetPath );

    // Since filling a QComboBox does not emit the 'activated' signal, initially set some variables manually
    lablib->SetChosenZTreeDataTargetPath( ui->CBDataTargetPath->currentText() );

    // Set the initial status of CBReceiptsforLocalClients according to the settings in lcLablib
    ui->ChBReceiptsforLocalClients->setChecked( lablib->GetPrintReceiptsForLocalClients() );
}
