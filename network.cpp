#include "network.h"
#include <bits/stdc++.h>
typedef pair<int, int> PII;

Network::Network()
{

}

Network::Network(QString originStr)
{
    this->originStr = originStr;
}

void Network::setOriginStr(QString originStr)
{
    this->originStr = originStr;
}

void Network::initMetroNetwork()
{
    QStringList list = originStr.split("\n");
    list.removeAt(0);
    QSet<QString> stationHash;
    QSet<QString> lineHash;
    qint32 lineIdx = 0, stationIdx = 0;
    for (QString s: list) {
        if (s.split(",").size() >= 4){
            QString lineName = s.split(",").at(0);
            if (!lineHash.contains(lineName)) {
                this->allLineNames.push_back(lineName);
                this->linecolors[lineName] = s.split(",").at(2);
                lineHash.insert(lineName);
                this->lineName2Hash[lineName] = lineIdx;
                this->hash2LineName[lineIdx] = lineName;
                lineIdx ++;
            }
        }
    }
    for (qint32 i = 0; i < lineIdx; i ++) {
        QVector<qint32> path(1, i);
        paths.push_back(path);
    }
    for (QString s: list) {
        if (s.split(",").size() >= 4){
            QString lineName = s.split(",").at(0);
            QString stationName = s.split(",").at(3);
            double latitude = s.split(",").at(5).toDouble();
            double longitude = s.split(",").at(4).toDouble();
            this->lineStations[lineName].push_back(stationName);

            if (!stationHash.contains(stationName)) {
                this->allStationNames.push_back(stationName);
                this->stationLocations[stationName] = {latitude, longitude};
                stationHash.insert(stationName);
                this->stationName2Hash[stationName] = stationIdx;
                this->hash2StationName[stationIdx] = stationName;
                qint32 lineIdx = lineName2Hash[lineName];
                paths[lineIdx].push_back(stationIdx);
                stationIdx ++;
            }
            else {
                qint32 lineIdx = lineName2Hash[lineName];
                qint32 stationIdx = stationName2Hash[stationName];
                paths[lineIdx].push_back(stationIdx);
            }
        }
    }
//    for (QVector<qint32> path:paths) {
//        qDebug() << "地铁线:" << hash2LineName[path[0]] << " ";
//        for (qint32 i = 1; i < path.size(); i ++) {
//            qDebug() << hash2StationName[path[i]] << " ";
//        }
//        qDebug() << endl;
//    }

}

QStringList Network::getAllStationNames()
{
    return allStationNames;
}

void Network::shortestDistance(QString startStation, QString endStation, qint32 priority)
{
    int n = paths.size();
    //站点的数目
    const int N = this->allStationNames.size() * 2;
    vector<vector<int>> h(N, vector<int>(0));
    vector<int> e, ne, line, w;
    vector<int> dist(N), stops(N), pre(N), cnt(N);
    vector<bool> st(N);
    QVector<QPair<QString, QPair<QString, QString>>> info(N); // 记录换乘信息
    int idx = 0;

    auto add = [&](int a, int b, int c, int d) {
        e.push_back(b);
        w.push_back(c);
        line.push_back(d);
        ne.push_back(h[a][0]);
        h[a][0] = idx++;
    };

    auto dijkstra = [&](int S, int T) {
        fill(dist.begin(), dist.end(), 0x3f3f);
        fill(cnt.begin(), cnt.end(), 0x3f3f);
        fill(st.begin(), st.end(), false);

        dist[S] = cnt[S] = 0;

        priority_queue<PII, vector<PII>, greater<PII>> heap;
        heap.push({0, S});

        //综合考虑站数和换乘数量
        const int H = 4;
        while (!heap.empty()) {
            auto t = heap.top();
            heap.pop();

            int ver = t.second;
            if (ver == T) break;
            if (st[ver]) continue;
            st[ver] = true;
            for (int i = h[ver][0]; i != -1; i = ne[i]) {
                int j = e[i];
                if (dist[j] + cnt[j] * H > dist[ver] + w[i] + (cnt[ver] + 1) * H){
                    dist[j] = dist[ver] + w[i];
                    cnt[j] = cnt[ver] + 1;
                    pre[j] = ver;
                    info[j] = {hash2LineName[line[i]], {hash2StationName[ver],hash2StationName[j]}};
                    heap.push({dist[j] + cnt[j] * H, j});
                }
            }
        }

    };

    auto dijkstra1 = [&](int S, int T) {
        fill(dist.begin(), dist.end(), 0x3f3f3f3f);
        fill(cnt.begin(), cnt.end(), 0x3f3f3f3f);
        fill(st.begin(), st.end(), false);

        dist[S] = cnt[S] = 0;

        priority_queue<PII, vector<PII>, greater<PII>> heap;
        heap.push({0, S});

        while (!heap.empty()) {
            auto t = heap.top();
            heap.pop();

            int ver = t.second;
            if (ver == T) break;
            if (st[ver]) continue;
            st[ver] = true;

            for (int i = h[ver][0]; i != -1; i = ne[i]) {
                int j = e[i];
                if (dist[j] > dist[ver] + w[i]) {
                    dist[j] = dist[ver] + w[i];
                    cnt[j] = cnt[ver] + 1;
                    pre[j] = ver;
                    info[j] = {hash2LineName[line[i]], {hash2StationName[ver],hash2StationName[j]}};
                    heap.push({dist[j], j});
                } else if (dist[j] == dist[ver] + w[i]) {
                    if (cnt[j] > cnt[ver] + 1) {
                        cnt[j] = cnt[ver] + 1;
                        pre[j] = ver;
                        info[j] = {hash2LineName[line[i]], {hash2StationName[ver],hash2StationName[j]}};
                    }
                }
            }
        }
    };

    //换乘少优先
    auto dijkstra2 = [&](int S, int T) {
        fill(dist.begin(), dist.end(), 0x3f3f3f3f);
        fill(cnt.begin(), cnt.end(), 0x3f3f3f3f);
        fill(st.begin(), st.end(), false);

        dist[S] = cnt[S] = 0;

        priority_queue<PII, vector<PII>, greater<PII>> heap;
        heap.push({0, S});

        while (!heap.empty()) {
            auto t = heap.top();
            heap.pop();

            int ver = t.second;
            if (ver == T) break;
            if (st[ver]) continue;
            st[ver] = true;

            for (int i = h[ver][0]; i != -1; i = ne[i]) {
                int j = e[i];
                if (cnt[j] > cnt[ver] + 1) {
                    dist[j] = dist[ver] + w[i];
                    cnt[j] = cnt[ver] + 1;
                    pre[j] = ver;
                    info[j] = {hash2LineName[line[i]], {hash2StationName[ver],hash2StationName[j]}};
                    heap.push({cnt[j], j});
                }
                else if (cnt[j] == cnt[ver] + 1) {
                    if (dist[j] > dist[ver] + w[i]) {
                        dist[j] = dist[ver] + w[i];
                        pre[j] = ver;
                        info[j] = {hash2LineName[line[i]], {hash2StationName[ver],hash2StationName[j]}};
                    }
                }
            }
        }

    };

    fill(h.begin(), h.end(), vector<int>(1, -1));

    for (int i = 0; i < n; i++) {
        int m = paths[i].size() - 1;
        for (int j = 0; j < m; j++) stops[j] = paths[i][j + 1];

        int len;
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < j; k++) {
                if (stops[0] != stops[m - 1]) len = j - k;
                else len = min(j - k, m - 1 - j + k); // 回路取路径短的方向
                add(stops[j], stops[k], len, i);
                add(stops[k], stops[j], len, i);
            }
        }
    }

    int S = stationName2Hash[startStation], T = stationName2Hash[endStation];
    if (priority == 0) dijkstra(S, T);
    else if (priority == 1) dijkstra1(S, T);
    else if (priority == 2) dijkstra2(S, T);

    QVector<QPair<QString, QPair<QString, QString>>> res;

    for (int i = T; i != S; i = pre[i]) res.push_back(info[i]);
    reverse(res.begin(), res.end());
//    qDebug() << dist[T];
//    for (int i = 0; i < res.size(); i++) qDebug() << res[i];

    this->shortestPaths = res;
    this->shortestStationNum = dist[T];
}

QVector<QString> Network::getConnectStations(QString startStation, QString endStation, QString lineName)
{
    QVector<QString> connectStations;

    if (lineName != "四号线") {
        QVector<QString>oneLineStations = this->lineStations[lineName];
        bool flag = false;
        for (QString station:oneLineStations) {
            if (station == startStation) {
                flag = true;
            }
            if (flag) {
                connectStations.append(station);
            }
            if (station == endStation) {
                break;
            }
        }
        if (connectStations.size() == 0) {
            reverse(oneLineStations.begin(), oneLineStations.end());
            bool flag = false;
            for (QString station:oneLineStations) {
                if (station == startStation) {
                    flag = true;
                }
                if (flag) {
                    connectStations.append(station);
                }
                if (station == endStation) {
                    break;
                }
            }
        }
    }
    else {
        QVector<QString>oneLineStations = this->lineStations[lineName];
        oneLineStations.pop_back();
        QVector<QString> leftConnectStations,rightConnectStations;
        int idx = 0;
        for (int i = 0; i < oneLineStations.size(); i ++) {
            if (oneLineStations[i] == startStation) {
                idx = i;
                break;
            }
        }
        int n = oneLineStations.size();
        int l = idx, r = idx;
        for (int i = 0; i < n * 2; i ++) {
            leftConnectStations.push_back(oneLineStations[l]);
            rightConnectStations.push_back(oneLineStations[r]);
            l --; r ++;
            if (l == -1) l = n - 1;
            if (r == n)  r = 0;
            if (leftConnectStations.end() == endStation) {
                return leftConnectStations;
            }
            if (rightConnectStations.end() == endStation) {
                return rightConnectStations;
            }
        }
    }

    return connectStations;
}
