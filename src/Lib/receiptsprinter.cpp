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

#include "receiptsprinter.h"
#include "settings.h"

lc::ReceiptsPrinter::ReceiptsPrinter(const QString &argDateString,
                                     const Settings *const argSettings,
                                     const QString &argWorkpath,
                                     QObject *argParent) :
    QThread{argParent},
    dateString{argDateString},
    dvipsCmd{argSettings->dvipsCmd},
    latexCmd{argSettings->latexCmd},
    lprCmd{argSettings->lprCmd},
    postscriptViewer{argSettings->postscriptViewer},
    ps2pdfCmd{argSettings->ps2pdfCmd},
    rmCmd{argSettings->rmCmd},
    vncViewer{argSettings->vncViewer},
    workpath{argWorkpath}
{
}
