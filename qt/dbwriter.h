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
#ifndef DBWRITER_H
#define DBWRITER_H

#include <iostream>
#include <QRunnable>
#include <QtSql\QSqlDatabase>
#include <QSqlQuery>
#include <QCloseEvent>

class DBWriter : public QRunnable
{
    void run() {
        //std::cout << "WRITE STARTED" << std::endl;
        QSqlDatabase db = QSqlDatabase::database("sqlLiteConnection");
        if (!db.open() || !db.isValid()) {
            return;
        }

        QSqlQuery query2(db);
        query2.prepare("SELECT \"Date\" FROM \"Wealth\" where \"GameID\"=? and \"RaceId\"=? and \"Date\">?");
        query2.bindValue(0, gameId);
        query2.bindValue(1, raceId);
        query2.bindValue(2, currentDate);
        query2.exec();
        if (query2.next()) {
           // std::cout << "Delete future rows" << std::endl;
            QSqlQuery query21(db);
            query21.prepare("DELETE FROM \"Wealth\" where \"GameID\"=? and \"RaceId\"=? and \"Date\">?");
            query21.bindValue(0, gameId);
            query21.bindValue(1, raceId);
            query21.bindValue(2, currentDate);
            query21.exec();

            QSqlQuery query22(db);
            query22.prepare("DELETE FROM \"Minerals\" where \"GameID\"=? and \"RaceId\"=? and \"Date\">?");
            query22.bindValue(0, gameId);
            query22.bindValue(1, raceId);
            query22.bindValue(2, currentDate);
            query22.exec();

            QSqlQuery query23(db);
            query23.prepare("DELETE FROM \"Population\" where \"GameID\"=? and \"RaceId\"=? and \"Date\">?");
            query23.bindValue(0, gameId);
            query23.bindValue(1, raceId);
            query23.bindValue(2, currentDate);
            query23.exec();

            QSqlQuery query24(db);
            query24.prepare("DELETE FROM \"Fuel\" where \"GameID\"=? and \"RaceId\"=? and \"Date\">?");
            query24.bindValue(0, gameId);
            query24.bindValue(1, raceId);
            query24.bindValue(2, currentDate);
            query24.exec();
        }

        QSqlQuery query(db);
        query.prepare("SELECT \"Date\" FROM \"Wealth\" where \"GameID\"=? and \"RaceId\"=? and \"Date\"=?");
        query.bindValue(0, gameId);
        query.bindValue(1, raceId);
        query.bindValue(2, currentDate);
        query.exec();
        while (query.next()) {
           // std::cout << "ROW EXISTS" << std::endl;
            return;
        }

        for (int x = 0; x < Duranium.size(); x++) {
            QSqlQuery query3(db);
            query3.prepare("INSERT into \"Minerals\" (\"Date\", \"GameID\", \"RaceID\", \"SystemID\", \"SystemBodyID\", "
                           "\"Duranium\", \"Neutronium\", \"Corbomite\", \"Tritanium\", \"Boronide\", \"Mercassium\", "
                           "\"Vendarite\", \"Sorium\", \"Uridium\", \"Corundium\", \"Gallicite\") values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
            query3.addBindValue(currentDate);
            query3.addBindValue(gameId);
            query3.addBindValue(raceId);
            query3.addBindValue(SystemID.at(x));
            query3.addBindValue(SystemBodyID.at(x));
            query3.addBindValue(Duranium.at(x));
            query3.addBindValue(Neutronium.at(x));
            query3.addBindValue(Corbomite.at(x));
            query3.addBindValue(Tritanium.at(x));
            query3.addBindValue(Boronide.at(x));
            query3.addBindValue(Mercassium.at(x));
            query3.addBindValue(Vendarite.at(x));
            query3.addBindValue(Sorium.at(x));
            query3.addBindValue(Uridium.at(x));
            query3.addBindValue(Corundium.at(x));
            query3.addBindValue(Gallicite.at(x));
            query3.exec();

            //std::cout << "INSERT into \"Minerals\" (\"Date\" \"SystemID\", \"SystemBodyID\", \"Duranium\") values (" << currentDate << ", " << SystemID.at(x) << ", " << SystemBodyID.at(x) << ", " << Duranium.at(x) << ")" << std::endl;
        }

        for (int x = 0; x < FuelStockpile.size(); x++) {
            QSqlQuery query4(db);
            query4.prepare("INSERT into \"Fuel\" (\"Date\", \"GameID\", \"RaceID\", \"SystemID\", \"SystemBodyID\", "
                           "\"FuelStockpile\") values (?, ?, ?, ?, ?, ?)");
            query4.addBindValue(currentDate);
            query4.addBindValue(gameId);
            query4.addBindValue(raceId);
            query4.addBindValue(SystemID.at(x));
            query4.addBindValue(SystemBodyID.at(x));
            query4.addBindValue(FuelStockpile.at(x));
            query4.exec();

            //std::cout << "INSERT into \"Fuel\" (\"Date\" \"SystemID\", \"SystemBodyID\", \"FuelStockpile\") values (" << currentDate << ", " << SystemID.at(x) << ", " << SystemBodyID.at(x) << ", " << FuelStockpile.at(x) << ")" << std::endl;
        }

        for (int x = 0; x < Population.size(); x++) {
            QSqlQuery query5(db);
            query5.prepare("INSERT into \"Population\" (\"Date\", \"GameID\", \"RaceID\", \"SystemID\", \"SystemBodyID\", "
                           "\"Population\") values (?, ?, ?, ?, ?, ?)");
            query5.addBindValue(currentDate);
            query5.addBindValue(gameId);
            query5.addBindValue(raceId);
            query5.addBindValue(SystemID.at(x));
            query5.addBindValue(SystemBodyID.at(x));
            query5.addBindValue(Population.at(x));
            query5.exec();

            //std::cout << "INSERT into \"Population\" (\"Date\" \"SystemID\", \"SystemBodyID\", \"Population\") values (" << currentDate << ", " << SystemID.at(x) << ", " << SystemBodyID.at(x) << ", " << Population.at(x) << ")" << std::endl;
        }

        QSqlQuery query6(db);
        query6.prepare("INSERT into \"Wealth\" (\"Date\", \"GameID\", \"RaceID\", \"WealthPoints\") values (?, ?, ?, ?)");
        query6.addBindValue(currentDate);
        query6.addBindValue(gameId);
        query6.addBindValue(raceId);
        query6.addBindValue(WealthPoints);
        query6.exec();

        //std::cout << "INSERT into \"Wealth\" (\"Date\" \"WealthPoints\") values (" << currentDate << ", " << WealthPoints << ")" << std::endl;
        //std::cout << "WRITE COMPLETE" << std::endl;
    }

public:
    DBWriter(QList<double> duranium, QList<double> neutronium, QList<double> corbomite, QList<double> tritanium, QList<double> boronide,
                QList<double> mercassium, QList<double> vendarite, QList<double> sorium, QList<double> uridium, QList<double> corundium, QList<double> gallicite,
                QList<double> fuelStockpile, QList<double> population, QList<int> systemID, QList<int> systemBodyID, double wealthPoints,
                int game, int race, qlonglong date);

private:
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
    double WealthPoints;
    int gameId;
    int raceId;
    qlonglong currentDate;
};

#endif // DBWRITER_H
