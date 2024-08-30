#include <QSqlDatabase>
class DatabaseConnection {
public:
    static DatabaseConnection& getInstance() {
        if (!instance) {
            instance = new DatabaseConnection();
        }
        return *instance;
    }

    QSqlDatabase openConnection() {
        if (db == nullptr){
            db = QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName("D:/Data/GoogleShare/QT/Project/metro/metro.db");
        }
        return db;
    }

    void closeConnection() {
        // 关闭数据库连接逻辑
    }

    QSqlDatabase& getDatabase() {
        return db;
    }

private:
    static DatabaseConnection* instance; // 静态实例指针
    QSqlDatabase db; // 数据库连接

    DatabaseConnection() {} // 私有构造函数
    ~DatabaseConnection() {
        // 析构函数中关闭数据库连接
        closeConnection();
    }

    // 禁止复制和赋值操作
    DatabaseConnection(const DatabaseConnection&);
    DatabaseConnection& operator=(const DatabaseConnection&);
};

// 初始化静态成员变量
DatabaseConnection* DatabaseConnection::instance = nullptr;

