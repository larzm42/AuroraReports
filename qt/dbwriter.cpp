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
#include "dbwriter.h"

DBWriter::DBWriter(QList<double> duranium, QList<double> neutronium, QList<double> corbomite, QList<double> tritanium, QList<double> boronide,
                         QList<double> mercassium, QList<double> vendarite, QList<double> sorium, QList<double> uridium, QList<double> corundium, QList<double> gallicite,
                         QList<double> fuelStockpile, QList<double> population, QList<int> systemID, QList<int> systemBodyID, double wealthPoints,
                         int game, int race, qlonglong date) {
    Duranium = QList<double>(duranium);
    Neutronium = QList<double>(neutronium);
    Corbomite = QList<double>(corbomite);
    Tritanium = QList<double>(tritanium);
    Boronide = QList<double>(boronide);
    Mercassium = QList<double>(mercassium);
    Vendarite = QList<double>(vendarite);
    Sorium = QList<double>(sorium);
    Uridium = QList<double>(uridium);
    Corundium = QList<double>(corundium);
    Gallicite = QList<double>(gallicite);
    FuelStockpile = QList<double>(fuelStockpile);
    Population = QList<double>(population);
    SystemID = QList<int>(systemID);
    SystemBodyID = QList<int>(systemBodyID);
    WealthPoints = wealthPoints;
    gameId = game;
    raceId = race;
    currentDate = date;
}
