#ifndef MAINMODEL_H
#define MAINMODEL_H

#include <QObject>

class mainmodel : public QObject
{
    Q_OBJECT
public:
    explicit mainmodel(QObject *parent = nullptr);

signals:

};

#endif // MAINMODEL_H
