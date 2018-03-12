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

#ifndef LOCALZLEAFSTARTER_H
#define LOCALZLEAFSTARTER_H

#include <QWidget>

namespace lc {

namespace Ui {
class LocalzLeafStarter;
} // namespace Ui

class LocalzLeafStarter : public QWidget
{
    Q_OBJECT

public:
    explicit LocalzLeafStarter(QWidget *argParent = nullptr);
    ~LocalzLeafStarter() override;

signals:
    void LocalzLeafRequested(QString argzLeafName, QString argzLeafVersion,
                             int argzTreePort);

private:
    Ui::LocalzLeafStarter *const ui = nullptr;

private slots:
    void on_PBStartLocalzLeaf_clicked();
};

}

#endif // LOCALZLEAFSTARTER_H
