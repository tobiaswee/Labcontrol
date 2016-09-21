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

#include "sessionsmodel.h"

lc::SessionsModel::SessionsModel( QObject *argParent ) :
    QAbstractTableModel{ argParent },
    sessions_vector{ new QVector< Session* > }
{
}

lc::SessionsModel::~SessionsModel() {
    for (auto s: *sessions_vector)
        delete s;
    delete sessions_vector;
}

lc::Session *lc::SessionsModel::back() const {
    return sessions_vector->back();
}

int lc::SessionsModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 2;
}

QVariant lc::SessionsModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant{};

    if (index.row() >= sessions_vector->size() || index.row() < 0)
        return QVariant{};

    if (role == Qt::DisplayRole)
        return sessions_vector->at( index.row() )->GetDataItem( index.column() );

    return QVariant{};
}

QVariant lc::SessionsModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch(section) {
        case 0:
            return tr("zTree Version");
        case 1:
            return tr("Port");
        case 2:
            return tr("LaTeX Header");
        case 3:
            return tr("Anonymous Receipts Placeholder");
        default:
            return QVariant{};
        }
    }
    return QVariant{};
}

void lc::SessionsModel::push_back( Session *argSession ) {
    sessions_vector->push_back( argSession );
}

int lc::SessionsModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return sessions_vector->length();
}
