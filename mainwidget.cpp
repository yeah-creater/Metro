#include "mainwidget.h"
#include "loginwidget.h"
#include "ui_mainwidget.h"
#include "network.h"
#include <QCompleter>
#include <QListView>
MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
    , loginWidget(new LoginWidget())
{
    ui->setupUi(this);
    //登录界面的信号连接主页面的槽
    connect(this->loginWidget, &LoginWidget::login,[=](const QString &username, const QString &password){
        if (emit(login(username, password))) {
            this->showMainWidget(username);
            return 1;
        }
        else {
            return 0;
        }
    });
    connect(this->loginWidget, &LoginWidget::skip,[=](){
        this->showMainWidget("游客");
        return 1;
    });

}

MainWidget::~MainWidget()
{
    delete loginWidget;
    delete ui;
}

void MainWidget::showLoginWidget()
{
    this->loginWidget->show();
}

void MainWidget::fuzzySearch(QLineEdit *lineEdit, QComboBox *combobox, QStringList items)
{
    combobox->addItems(items);
    combobox->setView(new QListView());
    combobox->setEditable(true);
    combobox->setLineEdit(lineEdit);
    // 确保 startStation 控件的样式表设置正确
    combobox->view()->setStyleSheet("QListView {font-family: \"Arial\"; font-size: 13px; outline: 0px;}"
       "QListView::item {padding: 3px 3px 3px 5px; border-width: 0px;}"
       "QListView::item:selected {background-color: rgb(4, 144, 226);}");

    QCompleter *pCompleter = new QCompleter(items, this);

    lineEdit->setCompleter(pCompleter);
    pCompleter->setFilterMode(Qt::MatchContains);
    combobox->setCompleter(pCompleter);
    lineEdit->clear();
}


void MainWidget::on_currentQueryButton_clicked()
{
    QString startStation = ui->startStation->currentText();
    QString endStation = ui->endStation->currentText();
    qint32 priority = 0;
    if (!this->network.getAllStationNames().contains(startStation) || !this->network.getAllStationNames().contains(endStation)) {
        QMessageBox::warning(nullptr, "Metro大都会", "输入站点不存在",
                                        QMessageBox::Yes);
        return;
    }
    else if (startStation == endStation) {
        QMessageBox::warning(nullptr, "Metro大都会", "你已经在目的地了哦亲",
                                        QMessageBox::Yes);
        return;
    }
    if (ui->priority->currentText() == "站数最少") {
        priority = 1;
    }
    else if (ui->priority->currentText() == "换乘最少") {
        priority = 2;
    }
    this->network.shortestDistance(startStation, endStation, priority);
    //在地图上显示
    this->update();
}

void MainWidget::showMainWidget(QString username)
{
    this->ui->username->setText(username);
    this->show();
    this->network = Utils::buildMetroNetwork();
    fuzzySearch(ui->startStationEdit, ui->startStation, network.getAllStationNames());
    fuzzySearch(ui->endStationEdit, ui->endStation, network.getAllStationNames());
}


void MainWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    // Define the data
    QStringList lines;
    QStringList stations;
    QList<QColor> colors;
    for (auto path:this->network.shortestPaths) {
        lines.append(path.first);
        stations.append(path.second.first);
        stations.append(path.second.second);
        colors.append(QColor(this->network.linecolors[path.first]));
    }

    int startY = 100;
    int startX = 50;
    int lineHeight = 60;
    int circleRadius = 10;
    painter.setRenderHint(QPainter::Antialiasing);


    //流程图
    for (int i = 0; i < stations.size(); i += 2){

        //draw dashline
        if (i > 0) {
            QPen pen;
            pen.setStyle(Qt::DashLine);
            pen.setColor(Qt::black);
            painter.setPen(pen);
            painter.drawLine(startX, startY + (i - 1) * lineHeight + circleRadius, startX, startY + (i - 1) * lineHeight + lineHeight / 2 - circleRadius);
        }
        // Draw station circles
        painter.setBrush(QBrush(colors[i / 2]));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(QPoint(startX, startY + (i - 1) * lineHeight + lineHeight / 2), circleRadius, circleRadius);

        // Draw station names
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", circleRadius));
        painter.drawText(startX + 20, startY + (i - 1) * lineHeight + lineHeight / 2 + 5, stations[i]);

        painter.setPen(Qt::NoPen);
        painter.drawRect(startX + 20, startY + i * lineHeight, lines[i / 2].size() * 15, 20);
        painter.setPen(Qt::white);
        painter.drawText(startX + 20, startY + i * lineHeight, lines[i / 2].size() * 15, 20, Qt::AlignCenter, lines[i / 2]);
        painter.setPen(Qt::black);

        // Draw connect line
        painter.setPen(QPen(colors[i / 2], 2));
        painter.drawLine(startX, startY + (i - 1) * lineHeight + lineHeight / 2 + circleRadius, startX, startY + (i + 1) * lineHeight - circleRadius);

        // Draw next station circles
        painter.setBrush(QBrush(colors[i / 2]));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(QPoint(startX, startY + (i + 1) * lineHeight ), circleRadius, circleRadius);

        // Draw next station names
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", circleRadius));
        painter.drawText(startX + 20, startY + (i + 1) * lineHeight + 5, stations[i + 1]);
    }
    //地图 绘制点
    for (QString stationName:this->network.getAllStationNames()) {
        painter.setPen(Qt::gray);
        painter.setBrush(QBrush(Qt::gray));
        auto &hash = this->network.stationLocations[stationName];
        QPointF point = Utils::latLonToScreen(hash.first, hash.second);
        painter.drawEllipse(point.x(),point.y(), 4, 4); // 绘制半径为4的
    }
    //地图 绘制点和边
    for (int i = 0; i < stations.size(); i += 2) {

        QString lineName = lines[i / 2];
        QVector<QString> connectStations =  this->network.getConnectStations(stations[i], stations[i + 1], lineName);
        for (int j = 0; j < connectStations.size() - 1; j ++ ) {
            auto &hash1 = this->network.stationLocations[connectStations[j]];
            auto &hash2 = this->network.stationLocations[connectStations[j + 1]];
            QPointF point1 = Utils::latLonToScreen(hash1.first, hash1.second);
            QPointF point2 = Utils::latLonToScreen(hash2.first, hash2.second);
            painter.setPen(QColor(this->network.linecolors[lines[i / 2]]));
            painter.setBrush(QBrush(Qt::white));
            painter.drawEllipse(point1.x(),point1.y(), 4, 4);
            painter.drawEllipse(point2.x(),point2.y(), 4, 4);
            painter.drawLine(QPointF(point1.x() + 2, point1.y() + 2), QPointF(point2.x() + 2, point2.y() + 2));
        }
    }
    event->accept();

}

