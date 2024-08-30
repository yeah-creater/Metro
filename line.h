#ifndef LINE_H
#define LINE_H
#include <bits/stdc++.h>
using namespace std;


class Line
{
public:
    Line();
    Line(string name, string color, bool isLoop);

private:
     string name;
     string color;
     bool isLoop;
};

#endif // LINE_H
