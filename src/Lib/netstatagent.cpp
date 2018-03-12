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

#include "netstatagent.h"

lc::NetstatAgent::NetstatAgent(const QString &argNetstatCommand,
                               QObject *argParent) :
    QObject{argParent},
    netstatCommand{argNetstatCommand}
{
    netstatQueryProcess.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
}

void lc::NetstatAgent::QueryClientConnections()
{
    netstatQueryProcess.start(netstatCommand, netstatArguments);
    if (!netstatQueryProcess.waitForFinished(400)) {
        emit QueryFinished(nullptr);
    } else {
        // Get all 'netstat_query_process' standard output and store it temporarily in 'temp_strings'
        const QString netstatQueryProcessOutputString{netstatQueryProcess.readAllStandardOutput()};
        const QStringList tempStrings{
            netstatQueryProcessOutputString.split('\n', QString::SkipEmptyParts)};

        const auto netstatQueryProcessOutput = new QStringList;
        for (const auto &s : tempStrings) {
            if (s.contains(searchRegexp)) {
                QRegularExpressionMatch match{extractionRegexp.match(s, s.indexOf(':', 0,
                                                                                  Qt::CaseInsensitive))};
                netstatQueryProcessOutput->append(match.captured());
            }
        }

        emit QueryFinished(netstatQueryProcessOutput);
    }
}
