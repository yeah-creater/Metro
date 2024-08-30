#include "maincontroller.h"
#include "mainwidget.h"
#include <QApplication>
#include <utils.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget mainWidget;
    MainController controller(&mainWidget);
    mainWidget.showLoginWidget();
    return a.exec();
}
