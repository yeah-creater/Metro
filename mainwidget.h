#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QShowEvent>
#include <QWidget>
#include "ui_mainwidget.h"
#include "loginwidget.h"
#include <QDebug>
#include <QThread>
#include <QSqlDatabase>
#include <utils.h>
#include <QPainter>

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    void showLoginWidget();
    void fuzzySearch(QLineEdit *lineedit, QComboBox *combobox, QStringList items);
    void showMainWidget(QString username);
    void paintEvent(QPaintEvent *event);
    Network network;

signals:
    int login(QString username, QString password);
private slots:
    void on_currentQueryButton_clicked();

private:
    Ui::MainWidget *ui; //并不是class MainWidget : public QWidget这个类型
    LoginWidget* loginWidget;

};
#endif // MAINWIDGET_H
