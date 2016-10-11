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

#ifndef MANUALPRINTINGSETUP_H
#define MANUALPRINTINGSETUP_H

#include <QWidget>

namespace lc {

namespace Ui {
class ManualPrintingSetup;
}

class ManualPrintingSetup : public QWidget {
    Q_OBJECT

public:
    explicit ManualPrintingSetup( QWidget *argParent = nullptr );
    ~ManualPrintingSetup();

signals:
    void RequestReceiptsHandler( bool argReceiptsForLocalClients,
                                 QString argAnonymousReceiptsPlaceholder,
                                 QString argLatexHeaderName );

private:
    QString filePath;
    Ui::ManualPrintingSetup *ui = nullptr;

private slots:
    void on_CBReceiptsHeader_activated( int argIndex );
    void on_ChBPrintAnonymousReceipts_clicked( bool argChecked );
    void on_PBPrint_clicked();
    void on_PBSelectFile_clicked();
};

}

#endif // MANUALPRINTINGSETUP_H
