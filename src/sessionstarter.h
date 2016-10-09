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

#include <QWidget>

class QStandardItemModel;

namespace lc {

class Client;

namespace Ui {
class SessionStarter;
}

class SessionStarter : public QWidget {
    Q_OBJECT

public:
    explicit SessionStarter( QWidget *argParent = nullptr );
    ~SessionStarter();

signals:
    void RequestNewDataTargetPath();
    void RequestNewSession( QVector< Client* > argAssocCl, QString argParticipNameReplacement,
                            bool argPrintAnonReceipts, QString argReceiptsHeader,
                            QString argzTreeDataTargetPath, quint16 argzTreePort,
                            QString argzTreeVersion );

private:
    QStandardItemModel *clientsViewModel = nullptr;
    Ui::SessionStarter *ui = nullptr;

private slots:
    void GetNewDataTargetPath();
    void on_CBDataTargetPath_activated( int argIndex );
    void on_CBReceiptsHeader_activated( int argIndex );
    void on_CBzTreeVersion_activated( int argIndex );
    void on_ChBPrintAnonymousReceipts_clicked( bool argChecked );
    void on_PBStartSession_clicked();
    void on_SBPort_editingFinished();
};

}

#endif // SESSIONSTARTER_H
