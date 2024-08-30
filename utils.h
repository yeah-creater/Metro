#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <bits/stdc++.h>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <network.h>
class Utils
{
private:
    Utils();
    static const QString URL;
public:
    static Network buildMetroNetwork();
    static QPointF latLonToScreen(double latitude, double longitude);
};

#endif // UTILS_H
