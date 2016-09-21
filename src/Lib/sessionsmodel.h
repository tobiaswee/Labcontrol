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

#ifndef SESSIONSMODEL_H
#define SESSIONSMODEL_H

#include <QAbstractTableModel>

#include "session.h"

class SessionsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SessionsModel(QObject *parent = 0);
    ~SessionsModel();
    SessionsModel(const SessionsModel&) = delete;
    lcSession *back() const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void push_back( lcSession *argSession );
    int rowCount(const QModelIndex &parent) const;


signals:

public slots:

private:
    QVector<lcSession*> *sessions_vector;
};

#endif // SESSIONSMODEL_H
