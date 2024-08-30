#include "maincontroller.h"

#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

MainController::MainController(MainWidget *mainWidget, QObject *parent) :
    QObject(parent),
    mainWidget(mainWidget)
{
    connect(this->mainWidget, SIGNAL(login(QString, QString)), this, SLOT(login(QString, QString)));
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:/Data/GoogleShare/QT/Project/metro/metro.db");
}

int MainController::login(QString username, QString password)
{
       // 初始化数据库连接
       QSqlDatabase db = this->getDB();
       if (!db.open()) {
           qDebug() << "Error opening database:" << db.lastError().text();
           return 0;
       }
       QSqlQuery query;
       query.prepare("SELECT * FROM user WHERE username = :username AND password = :password");
       query.bindValue(":username", username);
       query.bindValue(":password", password);
       if (query.exec()) {
           if (query.next()) {
               return 1;
           }
       }
       return 0;

}

QSqlDatabase MainController::getDB(){
    return db;
}

