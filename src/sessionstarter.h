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

#ifndef SESSIONSTARTER_H
#define SESSIONSTARTER_H

#include "src/Lib/lablib.h"

#include <QFileDialog>
#include <QWidget>

namespace lc {

namespace Ui {
class SessionStarter;
}

class SessionStarter : public QWidget {
    Q_OBJECT

public:
    explicit SessionStarter( Lablib *argLablib, QPlainTextEdit *argDebugMessagesTextEdit,
                             QWidget *parent = nullptr );
    ~SessionStarter();

    //! This gets thrown as an exception if this class is created even if it shouldn't
    //! (because no installed z-Tree instances could be detected).
    class lcForbiddenCall {};

signals:
    void SessionRequested( QString argDataTargetPath, int argPort, QString argzTreeVersion,
                           bool argReceiptsForLocalClients, QString argAnonReceiptPlaceholder,
                           QString argChosenLatexHeader );

private:
    QPlainTextEdit * const debugMessagesTextEdit = nullptr;
    Lablib * const lablib = nullptr;
    Ui::SessionStarter *ui = nullptr;

    void SetupWidgets();

private slots:
    //! Opens a 'QFileDialog' and tries to choose a new zTree data target path
    void GetNewDataTargetPath();
    void on_CBDataTargetPath_activated(const QString &arg1);
    void on_CBReceiptsHeader_activated( const QString &argHeader );
    void on_CBzTreeVersion_activated( const QString &argVersion );
    void on_ChBPrintanonymousreceipts_clicked( bool checked );
    void on_ChBReceiptsforLocalClients_clicked(bool checked);
    void on_PBStartzTree_clicked();
    void on_SBPort_editingFinished();

signals:
    //! This signal becomes emitted if "Set new path" was chosen in the 'CBDataTargetPath'
    void NewDataTargetPathRequested();
};

}

#endif // SESSIONSTARTER_H
