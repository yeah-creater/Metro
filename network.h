#ifndef NETWORK_H
#define NETWORK_H

#include <line.h>
#include <station.h>
#include <QString>
#include <QVector>
#include <QSet>
#include <QMap>
#include <qDebug>
#include <QPair>
class Network
{
public:
    Network();
    Network(QString originStr);

    void setOriginStr(QString originStr);
    void initMetroNetwork();
    QStringList getAllStationNames();
    void shortestDistance(QString startStation, QString endStation, qint32 priority);

    QMap<QString, QString> linecolors;
    QMap<QString, QPair<double, double>> stationLocations;

    QMap<QString, QVector<QString>> lineStations;
    QMap<qint32, QString> hash2StationName;
    QMap<QString, qint32> stationName2Hash;
    QMap<qint32, QString> hash2LineName;
    QMap<QString, qint32> lineName2Hash;

    QVector<QVector<qint32>> paths;
    QVector<QPair<QString, QPair<QString, QString>>> shortestPaths;
    qint32 shortestStationNum;

    QVector<QString> getConnectStations(QString startStation, QString endStation, QString lineName);

private:
    QString originStr;
    QStringList allStationNames;
    QStringList allLineNames;
//    QVector<Line> allLines;

};



#endif // NETWORK_H
