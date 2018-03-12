/*
 * Copyright 2014-2018 Markus Prasser, Tobias Weiss
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
} // namespace Ui

class ManualPrintingSetup : public QWidget
{
    Q_OBJECT

public:
    explicit ManualPrintingSetup(QWidget *argParent = nullptr);
    ~ManualPrintingSetup() override;

signals:
    void RequestReceiptsHandler(QString argzTreeDataTargetPath,
                                bool argReceiptsForLocalClients,
                                QString argAnonymousReceiptsPlaceholder,
                                QString argLatexHeaderName,
                                QString argDateString);

private:
    QString dateString;
    Ui::ManualPrintingSetup *const ui = nullptr;
    QString workPath;

private slots:
    void on_CBReceiptsHeader_activated(const int argIndex);
    void on_ChBPrintAnonymousReceipts_clicked(const bool argChecked);
    void on_PBPrint_clicked();
    void on_PBSelectFile_clicked();
};

} // namespace lc

#endif // MANUALPRINTINGSETUP_H
