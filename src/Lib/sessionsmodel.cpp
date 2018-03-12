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

#include <QDebug>

#include "session.h"
#include "sessionsmodel.h"

lc::SessionsModel::SessionsModel(QObject *argParent) :
    QAbstractTableModel{argParent}
{
}

lc::Session *lc::SessionsModel::back() const
{
    return sessionsList.back();
}

int lc::SessionsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant lc::SessionsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant{};

    if (index.row() >= sessionsList.size() || index.row() < 0)
        return QVariant{};

    if (role == Qt::DisplayRole)
        return sessionsList.at(index.row())->GetDataItem(index.column());

    return QVariant{};
}

QVariant lc::SessionsModel::headerData(int section, Qt::Orientation orientation,
                                       int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
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

void lc::SessionsModel::push_back(Session *argSession)
{
    connect( argSession, &Session::SessionFinished,
             this, &SessionsModel::RemoveSession );
    argSession->setParent(this);
    sessionsList.push_back(argSession);
}

void lc::SessionsModel::RemoveSession(Session *argSession)
{
    if (sessionsList.removeAll(argSession)) {
        qDebug() << "Successfully removed" << argSession << "from lc::SessionsModel";
        argSession->deleteLater();
    }
}

int lc::SessionsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return sessionsList.length();
}
