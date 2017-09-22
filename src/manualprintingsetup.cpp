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

#include <QFileDialog>

#include "manualprintingsetup.h"
#include "ui_manualprintingsetup.h"
#include "Lib/settings.h"

extern std::unique_ptr< lc::Settings > settings;

lc::ManualPrintingSetup::ManualPrintingSetup( QWidget *argParent ) :
    QWidget{ argParent },
    ui{ new Ui::ManualPrintingSetup }
{
    ui->setupUi( this );

    if ( settings->dvipsCmd.isEmpty() || settings->latexCmd.isEmpty()
         || settings->lcDataDir.isEmpty() || settings->lprCmd.isEmpty()
         || settings->postscriptViewer.isEmpty() || settings->ps2pdfCmd.isEmpty()
         || settings->rmCmd.isEmpty() || settings->vncViewer.isEmpty() ) {
        ui->VLManualPrintingSetup->setEnabled( false );
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

lc::ManualPrintingSetup::~ManualPrintingSetup() {
    delete ui;
}

void lc::ManualPrintingSetup::on_PBSelectFile_clicked() {
    QFileDialog fileDialog{ this, tr( "Please choose a payment file to print." ),
                            QDir::homePath(), "*.pay" };
    fileDialog.setFileMode( QFileDialog::ExistingFile );
    fileDialog.setOption( QFileDialog::ReadOnly, true );
    if ( fileDialog.exec() ) {
        ui->PBSelectFile->setStyleSheet( "" );
        const QString tmpFileName{ fileDialog.selectedFiles().at( 0 ) };
        dateString = tmpFileName.split( '/', QString::KeepEmptyParts,
                                        Qt::CaseInsensitive ).last()
                                .split( '.', QString::KeepEmptyParts,
                                        Qt::CaseInsensitive ).first();
        workPath = tmpFileName;
        workPath.truncate( workPath.lastIndexOf( '/' ) );
    }
}

void lc::ManualPrintingSetup::on_CBReceiptsHeader_activated( int argIndex ) {
    Q_UNUSED( argIndex );
    ui->CBReceiptsHeader->setStyleSheet( "" );
}

void lc::ManualPrintingSetup::on_ChBPrintAnonymousReceipts_clicked( bool argChecked ) {
    ui->ChBPrintAnonymousReceipts->setStyleSheet( "" );

    ui->LReplaceParticipantNames->setEnabled( argChecked );
    ui->CBReplaceParticipantNames->setEnabled( argChecked );
}

void lc::ManualPrintingSetup::on_PBPrint_clicked() {
    QString anonymousReceiptsPlaceholder;
    if ( ui->ChBPrintAnonymousReceipts->isChecked() ) {
        anonymousReceiptsPlaceholder = ui->CBReplaceParticipantNames->currentText();
    }

    emit RequestReceiptsHandler( workPath, ui->ChBReceiptsForLocalClients->isChecked(),
                                 anonymousReceiptsPlaceholder,
                                 ui->CBReceiptsHeader->currentText(), dateString );

    this->deleteLater();
}
