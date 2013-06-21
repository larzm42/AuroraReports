/* This file is part of AuroraReports.
 *
 * AuroraReports is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * dom3ai is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with AuroraReports.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <iostream>
#include <QtSql\QSqlDatabase>
#include <QSqlQuery>
#include <QCloseEvent>
#include <QThreadPool>
#include "dbreader.h"
#include "dbwriter.h"
#include "mainwindow.h"

DBReader::DBReader() { }

DBReader::~DBReader() { }

void DBReader::process()
{
    std::cout << "Read started" << std::endl;
    QList<MainWindow::GameData> games = MainWindow::getGameList("mdbConnection");
    QListIterator<MainWindow::GameData> gameIter(games);
    while (gameIter.hasNext()) {
        MainWindow::GameData gameData = gameIter.next();
        qlonglong currentDate = getCurrentDate(gameData.id);

        QList<MainWindow::GameData> races = MainWindow::getRaceList("mdbConnection", gameData.id);
        QListIterator<MainWindow::GameData> raceIter(races);
        while (raceIter.hasNext()) {
            MainWindow::GameData raceData = raceIter.next();
            updateMinerals(gameData.id, raceData.id, currentDate);
        }
    }
    std::cout << "Read complete" << std::endl;
    emit finished();
}

qlonglong DBReader::getCurrentDate(int gameId)
{
    qlonglong timestamp = 0;
    QSqlDatabase db = QSqlDatabase::database("mdbConnection");
    if (!db.open() || !db.isValid()) {
        std::cout << "Error opening Aurora database." << std::endl;
        return timestamp;
    }
    QSqlQuery query(db);
    query.prepare("SELECT \"GameTime\", \"StartYear\" FROM Game where GameID=?");
    query.bindValue(0, gameId);
    query.exec();
    if (query.next()) {
        timestamp = query.value(0).toLongLong();
        qlonglong startYear = query.value(1).toInt();
        timestamp += (startYear-1970)*12*30*24*60*60;
    }
    qlonglong days = timestamp / (60*60*24);
    timestamp = days * 24*60*60;
    return timestamp;
}

void DBReader::updateMinerals(int gameId, int raceId, qlonglong currentDate)
{
    QList<double> Duranium;
    QList<double> Neutronium;
    QList<double> Corbomite;
    QList<double> Tritanium;
    QList<double> Boronide;
    QList<double> Mercassium;
    QList<double> Vendarite;
    QList<double> Sorium;
    QList<double> Uridium;
    QList<double> Corundium;
    QList<double> Gallicite;
    QList<double> FuelStockpile;
    QList<double> Population;
    QList<int> SystemID;
    QList<int> SystemBodyID;
    double WealthPoints = 0;

    QSqlDatabase mdb = QSqlDatabase::database("mdbConnection");
    if (!mdb.open() || !mdb.isValid()) {
        std::cout << "Error opening Aurora database." << std::endl;
        return;
    }
    QSqlQuery query(mdb);
    query.prepare("SELECT \"Duranium\", \"Neutronium\", \"Corbomite\", \"Tritanium\", \"Boronide\", \"Mercassium\", \"Vendarite\", "
                  "\"Sorium\", \"Uridium\", \"Corundium\", \"Gallicite\", \"FuelStockpile\", \"Population\", \"SystemID\", \"SystemBodyID\" "
                  "FROM Population where GameID=? and RaceID=?");
    query.bindValue(0, gameId);
    query.bindValue(1, raceId);
    query.exec();
    while (query.next()) {
        Duranium.append(query.value(0).toDouble());
        Neutronium.append(query.value(1).toDouble());
        Corbomite.append(query.value(2).toDouble());
        Tritanium.append(query.value(3).toDouble());
        Boronide.append(query.value(4).toDouble());
        Mercassium.append(query.value(5).toDouble());
        Vendarite.append(query.value(6).toDouble());
        Sorium.append(query.value(7).toDouble());
        Uridium.append(query.value(8).toDouble());
        Corundium.append(query.value(9).toDouble());
        Gallicite.append(query.value(10).toDouble());
        FuelStockpile.append(query.value(11).toDouble());
        Population.append(query.value(12).toDouble());
        SystemID.append(query.value(13).toInt());
        SystemBodyID.append(query.value(14).toInt());
    }

    QSqlQuery query2(mdb);
    query2.prepare("SELECT \"WealthPoints\" FROM Race where GameID=? and RaceID=?");
    query2.bindValue(0, gameId);
    query2.bindValue(1, raceId);
    query2.exec();
    while (query2.next()) {
        WealthPoints = query2.value(0).toDouble();
    }

    DBWriter *task = new DBWriter(Duranium, Neutronium, Corbomite, Tritanium, Boronide, Mercassium, Vendarite, Sorium, Uridium,
                                  Corundium ,Gallicite, FuelStockpile, Population, SystemID, SystemBodyID, WealthPoints, gameId, raceId, currentDate);
    QThreadPool::globalInstance()->start(task);
}
