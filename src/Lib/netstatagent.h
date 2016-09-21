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

#ifndef NETSTATAGENT_H
#define NETSTATAGENT_H

#include <QObject>
#include <QProcess>
#include <QRegularExpression>
#include <QStringList>

//! The NetstatAgent class is used to do repetitive runs of the 'netstat' program to check for active zLeaf connections.
/*!
  This class is just used for repetive executions of netstat.
*/
class lcNetstatAgent : public QObject
{
    Q_OBJECT
public:
    explicit lcNetstatAgent( QString *argNetstatCommand, QObject *argParent = nullptr );
    
signals:
    //! This signal is emitted if the query of the currently active zLeaf connections finished
    void QueryFinished(QStringList *argActiveZLeafConnections);
    
public slots:
    void QueryClientConnections();
    
private:
    const QRegularExpression extractionRegexp;
    const QStringList netstatArguments;
    const QString netstatCommand;
    QProcess netstatQueryProcess;
    const QRegularExpression searchRegexp;
};

#endif // NETSTATAGENT_H
