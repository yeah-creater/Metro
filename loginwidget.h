#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QMessageBox>
#include "ui_loginwidget.h"
#include <QDebug>
#include <QThread>
class LoginWidget : public QWidget {
    Q_OBJECT

public:
    LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

signals:
     int skip();
     int login(QString username, QString password);

private slots:
     void on_loginButton_clicked();
     void on_skipButton_clicked();


private:
    Ui::LoginWidget *ui;
};

#endif // LOGINWIDGET_H
