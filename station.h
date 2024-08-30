#ifndef STATION_H
#define STATION_H

#include <bits/stdc++.h>
using namespace std;

class Station
{
public:
    Station();
    Station(string name, double latitude, double longitude);

private:
    string name;
    double latitude; //纬度
    double longitude; //经度
};

#endif // STATION_H
