#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include "mainwidget.h"
#include <QSqlDatabase>
class MainController : public QObject
{
    Q_OBJECT
public:
    MainController(MainWidget *mainWidget, QObject *parent = nullptr);

signals:

private slots:
    int login(QString username, QString password);
    QSqlDatabase getDB();

private:
    MainWidget* mainWidget;
    QSqlDatabase db;

};

#endif // MAINCONTROLLER_H
