#ifndef DATABASE_H
#define DATABASE_H

#include <QMainWindow>
#include <QObject>
#include <QtConcurrent>
#include <QSqlDataBase>
#include <QSqlError>

#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "MyDB"

class database: public QObject
{
    Q_OBJECT
public:
    explicit database(QObject *parent = nullptr);
    ~database();

    void ConnectToDataBase();
    void AddDataBase(QString driver, QString nameDB = "");
    void DisconnectFromDataBase();
    QSqlError GetLastError(void);

private:
    QSqlDatabase* dataBase;

    QString hostName = "981757-ca08998.tmweb.ru";
    QString dbName = "demo";
    QString login = "netology_usr_cpp";
    QString password = "CppNeto3";
    QString portNum = "5432";

signals:
    void sig_SendStatusConnection(bool);
};

#endif // DATABASE_H
