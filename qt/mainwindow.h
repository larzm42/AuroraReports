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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql\QSqlDatabase>
#include <QDateTime>
#include <QProgressDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    struct GameData {
        int id;
        QString name;
    };

    bool getMDBConnection(QString location);
    bool getConnection();
    void connectToAurora();
    static QList<GameData> getGameList(QString conn);
    static QList<GameData> getRaceList(QString conn, int gameId);
    QString getMinDate();
    QString getMaxDate();

private slots:
    void putDBReaderInAThread();
    void startTimer();
    void readSettings();
    void writeSettings();
    void on_quitButton_clicked();
    void on_auroraBrowseButton_clicked();
    void on_gameComboBox_currentIndexChanged(int index);
    void on_generateReportButton_clicked();
    void generateReportFinished();
    void on_dateComboBox_currentIndexChanged(int index);
    void on_reportComboBox_currentIndexChanged(const QString &arg1);

    void on_deleteButton_clicked();

private:
    Ui::MainWindow *ui;
    bool connected;
    QProgressDialog dialog;

};

#endif // MAINWINDOW_H
