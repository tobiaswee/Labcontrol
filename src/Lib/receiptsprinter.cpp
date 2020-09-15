/*
 * Copyright 2014-2020 Markus Prasser
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

#include <memory>

#include "receiptsprinter.h"
#include "settings.h"

extern std::unique_ptr< lc::Settings > settings;

lc::ReceiptsPrinter::ReceiptsPrinter( const QString &argDateString,
                                      const QString &argWorkpath,
                                      QObject *argParent ) :
    QThread{ argParent },
    dateString{ argDateString },
    dvipsCmd{ settings->dvipsCmd },
    latexCmd{ settings->latexCmd },
    lprCmd{ settings->lprCmd },
    postscriptViewer{ settings->postscriptViewer },
    ps2pdfCmd{ settings->ps2pdfCmd },
    rmCmd{ settings->rmCmd },
    vncViewer{ settings->vncViewer },
    workpath{ argWorkpath }
{
}
