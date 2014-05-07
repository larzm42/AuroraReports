/* This file is part of AuroraReports.
 *
 * AuroraReports is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AuroraReports is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with AuroraReports.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef DBREADER_H
#define DBREADER_H
#include <QObject>

class DBReader : public QObject
{
    Q_OBJECT

public:
    DBReader();
    ~DBReader();

private:
    qlonglong getCurrentDate(int gameId);
    void updateMinerals(int gameId, int raceId, qlonglong currentDate);

public slots:
    void process();

signals:
    void finished();
    void error(QString err);

};

#endif // DBREADER_H
