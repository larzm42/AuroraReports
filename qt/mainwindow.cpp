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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QtSql\QSqlDatabase>
#include <QSqlQuery>
#include <QTimer>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QCloseEvent>
#include <QThread>
#include <QProcess>
#include <QThreadPool>
#include "dbreader.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QThreadPool::globalInstance()->setMaxThreadCount(1);
    ui->startDateText->setText("2025");
    ui->endDateText->setText("2025");

    connected = false;
    getConnection();
    readSettings();

    if (ui->auroraText->text().length() > 0) {
        connectToAurora();
    }

    startTimer();
}

MainWindow::~MainWindow()
{
    writeSettings();
    delete ui;
}

void MainWindow::startTimer()
{
    QTimer::singleShot(5000, this, SLOT(putDBReaderInAThread()));
}

bool MainWindow::getMDBConnection(QString location)
{
    location.replace("\\", "\\\\");
    QSqlDatabase mdb = QSqlDatabase::addDatabase("QODBC", "mdbConnection");
    mdb.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ=" + location + "\\Stevefire.mdb;PWD=" + QByteArray::fromBase64("cmFpc3RsaW4zMQ==") + ";READONLY=true");
    if (!mdb.open()) {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"),
                           "Error opening Aurora database. Please select the installation directory of Aurora.", 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::RejectRole);
        msgBox.exec();
        return false;
    }
    QSqlDatabase mdb2 = QSqlDatabase::addDatabase("QODBC", "mdbConnection2");
    mdb2.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ=" + location + "\\Stevefire.mdb;PWD=" + QByteArray::fromBase64("cmFpc3RsaW4zMQ==") + ";READONLY=true");
    if (!mdb2.open()) {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"),
                           "Error opening Aurora database. Please select the installation directory of Aurora.", 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::RejectRole);
        msgBox.exec();
        return false;
    }
    return true;
}

bool MainWindow::getConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "sqlLiteConnection");
    db.setDatabaseName("data\\aurora.db3");
    if (!db.open()) {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"),
                           "Error opening local database.", 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::RejectRole);
        msgBox.exec();
        return false;
    }
    return true;
}

void MainWindow::writeSettings()
{
    QSettings settings("Larz Soft", "auroraReports");
    settings.beginGroup("aurora");
    settings.setValue("auroraLocation", ui->auroraText->text());
    settings.endGroup();
}

void MainWindow::readSettings()
{
    QSettings settings("Larz Soft", "auroraReports");
    settings.beginGroup("aurora");
    ui->auroraText->setText(settings.value("auroraLocation", "").toString());
    settings.endGroup();
}

void MainWindow::on_quitButton_clicked()
{
    QApplication::quit();
}

void MainWindow::on_auroraBrowseButton_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Aurora Location"), "");
    ui->auroraText->setText(fileName);
    connectToAurora();
}

QList<MainWindow::GameData> MainWindow::getGameList(QString conn)
{
    QList<MainWindow::GameData> gameList;
    QSqlDatabase db = QSqlDatabase::database(conn);
    if (!db.open() || !db.isValid()) {
        return gameList;
    }
    QSqlQuery query = db.exec("SELECT \"GameID\", \"GameName\" FROM Game");
    while (query.next()) {
        MainWindow::GameData data;
        data.id = query.value(0).toInt();
        data.name = query.value(1).toString();
        gameList.append(data);
    }
    return gameList;
}

QList<MainWindow::GameData> MainWindow::getRaceList(QString conn, int gameId)
{
    QList<MainWindow::GameData> gameList;
    QSqlDatabase db = QSqlDatabase::database(conn);
    if (!db.open() || !db.isValid()) {
        return gameList;
    }
    QSqlQuery query(db);
    query.prepare("SELECT \"RaceID\", \"RaceTitle\" FROM Race where \"GameID\"=? and NPR=False");
    query.addBindValue(gameId);
    query.exec();

    while (query.next()) {
        MainWindow::GameData data;
        data.id = query.value(0).toInt();
        data.name = query.value(1).toString();
        gameList.append(data);
    }
    return gameList;
}

void MainWindow::connectToAurora()
{
    if (getMDBConnection(ui->auroraText->text())) {
        connected = true;
        ui->statusBar->showMessage("Connected to " + ui->auroraText->text() + "\\Stevefire.mdb");
        ui->gameComboBox->clear();
        ui->raceComboBox->clear();
        QList<MainWindow::GameData> games = MainWindow::getGameList("mdbConnection2");
        QListIterator<MainWindow::GameData> iter(games);
        while (iter.hasNext()) {
            GameData data = iter.next();
            ui->gameComboBox->addItem(data.name, data.id);
        }
    } else {
        connected = false;
        ui->statusBar->showMessage("");
        ui->gameComboBox->clear();
        ui->raceComboBox->clear();
    }
}

void MainWindow::on_gameComboBox_currentIndexChanged(int index)
{
    if (index == -1) {
        return;
    }
    int id = ui->gameComboBox->itemData(index).toInt();
    ui->raceComboBox->clear();
    QList<MainWindow::GameData> races = MainWindow::getRaceList("mdbConnection2", id);
    QListIterator<MainWindow::GameData> iter(races);
    while (iter.hasNext()) {
        GameData data = iter.next();
        ui->raceComboBox->addItem(data.name, data.id);
    }
}

void MainWindow::putDBReaderInAThread()
{
    if (connected) {
        QThread* thread = new QThread;
        DBReader* reader = new DBReader();
        reader->moveToThread(thread);
        connect(thread, SIGNAL(started()), reader, SLOT(process()));
        connect(reader, SIGNAL(finished()), thread, SLOT(quit()));
        connect(reader, SIGNAL(finished()), reader, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        connect(reader, SIGNAL(finished()), this, SLOT(startTimer()));
        thread->start();
    } else {
        startTimer();
    }
}

void MainWindow::generateReportFinished()
{
    dialog.close();
}

void MainWindow::on_generateReportButton_clicked()
{
    dialog.reset();
    Qt::WindowFlags flags = dialog.windowFlags();
    Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
    flags = flags & (~helpFlag);
    dialog.setWindowFlags(flags);
    dialog.setModal(true);
    dialog.setWindowTitle("Generating Report");
    dialog.setWindowIcon(QIcon(":/EnterpriseA.gif"));
    dialog.setMinimum(0);
    dialog.setMaximum(0);
    dialog.show();

    QStringList args;
    args << "-classpath" << "lib\\*;lib\\aurora.jar" << "org.larz.aurorareports.Aurora";
    args << ui->gameComboBox->itemData(ui->gameComboBox->currentIndex()).toString();
    args << ui->raceComboBox->itemData(ui->raceComboBox->currentIndex()).toString();
    args << ui->reportComboBox->currentText() + " Report for " + ui->raceComboBox->currentText();
    args << getMinDate();
    args << getMaxDate();
    QString reportName = ui->reportComboBox->currentText().toLower();
    if (reportName == "minerals") {
        if (ui->mineralComboBox->currentIndex() != 0) {
            reportName = "minerals" + ui->mineralComboBox->currentText();
        }
    }
    args << reportName;

    QString program = "jre7\\bin\\java";

    QProcess *myProcess = new QProcess();
    myProcess->start(program, args);

    connect(myProcess, SIGNAL(finished(int)), this, SLOT(generateReportFinished()));
}

QString MainWindow::getMinDate()
{
    if (ui->dateComboBox->currentText() == "All") {
        return "-9999999999999";
    }
    qlonglong day = ui->startDateSpinBox->value();
    qlonglong month = ui->startDateComboBox->currentIndex()+1;
    qlonglong year = ui->startDateText->text().toLongLong();
    qlonglong seconds = (day-1) * 24*60*60;
    seconds += (month-1) * 30*24*60*60;
    seconds += (year-1970) * 12*30*24*60*60;
    return QString::number(seconds);
}

QString MainWindow::getMaxDate()
{
    if (ui->dateComboBox->currentText() == "All") {
        return "9999999999999";
    }
    qlonglong day = ui->endDateSpinBox->value();
    qlonglong month = ui->endDateComboBox->currentIndex()+1;
    qlonglong year = ui->endDateText->text().toLongLong();
    qlonglong seconds = (day-1) * 24*60*60;
    seconds += (month-1) * 30*24*60*60;
    seconds += (year-1970) * 12*30*24*60*60;
    return QString::number(seconds);
}

void MainWindow::on_dateComboBox_currentIndexChanged(int index)
{
    if (index == 0) {
        ui->startDateText->setEnabled(false);
        ui->startDateComboBox->setEnabled(false);
        ui->startDateSpinBox->setEnabled(false);
        ui->endDateText->setEnabled(false);
        ui->endDateComboBox->setEnabled(false);
        ui->endDateSpinBox->setEnabled(false);
    } else {
        ui->startDateText->setEnabled(true);
        ui->startDateComboBox->setEnabled(true);
        ui->startDateSpinBox->setEnabled(true);
        ui->endDateText->setEnabled(true);
        ui->endDateComboBox->setEnabled(true);
        ui->endDateSpinBox->setEnabled(true);
    }
}

void MainWindow::on_reportComboBox_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "Minerals") {
        ui->mineralComboBox->setEnabled(true);
    } else {
        ui->mineralComboBox->setEnabled(false);
    }
}

void MainWindow::on_deleteButton_clicked()
{
    int ret = QMessageBox::information(this, tr("Delete"),
                                    tr("Delete report data for game " + ui->gameComboBox->currentText().toLocal8Bit() + ", race " + ui->raceComboBox->currentText().toLocal8Bit() + "?"),
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        QSqlDatabase db = QSqlDatabase::database("sqlLiteConnection");
        if (!db.open() || !db.isValid()) {
            return;
        }
        int gameId = ui->gameComboBox->itemData(ui->gameComboBox->currentIndex()).toInt();
        int raceId = ui->raceComboBox->itemData(ui->raceComboBox->currentIndex()).toInt();

        QSqlQuery query1(db);
        query1.prepare("DELETE FROM \"Wealth\" where \"GameID\"=? and \"RaceId\"=?");
        query1.bindValue(0, gameId);
        query1.bindValue(1, raceId);
        query1.exec();

        QSqlQuery query2(db);
        query2.prepare("DELETE FROM \"Minerals\" where \"GameID\"=? and \"RaceId\"=?");
        query2.bindValue(0, gameId);
        query2.bindValue(1, raceId);
        query2.exec();

        QSqlQuery query3(db);
        query3.prepare("DELETE FROM \"Population\" where \"GameID\"=? and \"RaceId\"=?");
        query3.bindValue(0, gameId);
        query3.bindValue(1, raceId);
        query3.exec();

        QSqlQuery query4(db);
        query4.prepare("DELETE FROM \"Fuel\" where \"GameID\"=? and \"RaceId\"=?");
        query4.bindValue(0, gameId);
        query4.bindValue(1, raceId);
        query4.exec();

    }

}
