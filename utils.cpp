#include "utils.h"

#include <QDir>

Utils::Utils()
{

}
const QString Utils::URL = "shanghai.txt";

Network Utils::buildMetroNetwork() {
    QFile file(QDir::currentPath() + "/" + URL);
    qDebug() << QDir::currentPath();
    Network network;
    if (!file.open(QIODevice::ReadOnly| QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Metro大都会", "找不到地铁文件信息",
                                        QMessageBox::Yes);
        return network;
    }
    QTextStream input(&file);
    input.setCodec("UTF-8");
    QString res;
    while(!input.atEnd()){
        QString line = input.readLine();
        res += line;
        res += "\n";
    }
    network.setOriginStr(res);
    network.initMetroNetwork();
    return network;
}

QPointF Utils::latLonToScreen(double latitude, double longitude)
{
    QSize screenSize = QSize(1000, 800);

    double minLat = 30.85; // 最小纬度
    double maxLat = 31.65; // 最大纬度
    double minLon = 120.9; // 最小经度
    double maxLon = 122.1; // 最大经度

    // 根据屏幕大小和地图范围计算比例尺
    double latScale = (screenSize.height()) / (maxLat - minLat);
    double lonScale = (screenSize.width()) / (maxLon - minLon);

    double screenX = (longitude - minLon) * lonScale;
    double screenY = (maxLat - latitude) * latScale; // 翻转Y轴

    return QPointF(screenX, screenY);

}

