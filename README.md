## 基于Qt5的上海地铁Metro换乘系统

> Tips:
> 0. 由于项目中文件地址使用绝对路径
> 1. 地铁文件信息在utils.cpp 文件中，需要更改为你自己设备上的地址
> 2. 数据库文件地址在maincontroller.cpp文件中，同样需要更改为你自己设备上的地址


### 主要内容

- 交互界面由Qt5编写，地图绘制采用QPainter；
- 多特征最短路采用堆优化版本Dijkstra来高效实现；
-  数据库采用SQLite；





![1](./assert/1.jpg)

![2](./assert/2.jpg)

![3](./assert/3.jpg)

![4](./assert/4.jpg)





### 参考文献

https://github.com/lankoestee/MetroSearch 

https://www.acwing.com/problem/content/1626/

