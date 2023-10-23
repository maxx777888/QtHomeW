#ifndef DATABASE_H
#define DATABASE_H

#include <QMainWindow>
#include <QObject>
#include <QtConcurrent>
#include <QSqlDataBase>
#include <QSqlError>
//Second Part
#include <QSqlQueryModel>

#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "MyDB"

//Типы запросов
enum requestType{

    requestArrival = 1,
    requestDeparture  = 2,


};

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

    void RequestToDB(QString airportCode, QString dateFl,int req);//Метод осуществляет запросы к БД

private:
    QSqlDatabase* dataBase;
    QSqlQueryModel* airportNamesQuery;
    QSqlQueryModel* flightInfoQuery;

    QString hostName = "981757-ca08998.tmweb.ru";
    QString dbName = "demo";
    QString login = "netology_usr_cpp";
    QString password = "CppNeto3";
    QString portNum = "5432";
    //Запрос на список аэропортов
    QString airportListRequest = "SELECT airport_name->>'ru' as \"airportName\", airport_code FROM bookings.airports_data;";

signals:
    void sig_SendStatusConnection(bool);//Сигнал о подключение к БД
    void sig_SendDataFromDBQuiryModel(QSqlQueryModel *model); //Сигнал по запросу к БД
    void sig_SendRequstFlightInfo(QSqlQueryModel *model);
};

#endif // DATABASE_H
