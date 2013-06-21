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
    mdb.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ=" + location + "\\Stevefire.mdb;PWD=raistlin31;READONLY=true");
    if (!mdb.open()) {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"),
                           "Error opening Aurora database. Please select the installation directory of Aurora.", 0, this);
        msgBox.addButton(tr("&Continue"), QMessageBox::RejectRole);
        msgBox.exec();
        return false;
    }
    QSqlDatabase mdb2 = QSqlDatabase::addDatabase("QODBC", "mdbConnection2");
    mdb2.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ=" + location + "\\Stevefire.mdb;PWD=raistlin31;READONLY=true");
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
    db.setDatabaseName("aurora.db3");
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
    args << "-classpath" << "lib\\*;aurora.jar" << "org.larz.aurorareports.Aurora";
    args << ui->gameComboBox->itemData(ui->gameComboBox->currentIndex()).toString();
    args << ui->raceComboBox->itemData(ui->raceComboBox->currentIndex()).toString();
    args << ui->reportComboBox->currentText() + " Report for " + ui->raceComboBox->currentText();
    args << ui->reportComboBox->currentText().toLower();

    QString program = "java";

    QProcess *myProcess = new QProcess();
    QFileInfo exe = QFileInfo(program);
    myProcess->setWorkingDirectory(exe.absolutePath());
    myProcess->start(program, args);

    connect(myProcess, SIGNAL(finished(int)), this, SLOT(generateReportFinished()));

}
