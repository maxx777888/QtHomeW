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

    void ConnectToDataBase();//Метод подключается к БД
    void AddDataBase(QString driver, QString nameDB = "");//Метод добавляет БД к экземпляру класса QSqlDataBase
    void DisconnectFromDataBase();//Метод отключения от БД
    QSqlError GetLastError(void);//Метод показывает ошибку подлючения к БД
    //Метод осуществляет запросы к БД по поводу ежедневных рейсов
    void RequestToDBToGetAirportName(const QString &airportCode, const QString &dateFl,const int &req);
    //Метод осуществляет запросы к БД для сбора месячной статистики
    void MonthlyRequestToDB(const QString &airportCode, const int &month);
    //Метод осуществляет запросы к БД для сбора годовой статистики
    void YearRequestToDB(const QString &airportCode);

private:
    QSqlDatabase* dataBase;
    QSqlQueryModel* airportNamesQuery;
    QSqlQueryModel* flightInfoQuery;
    QSqlQueryModel* monthStatQuery;
    QSqlQueryModel* yearStatQuery;

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
    void sig_SendRequstFlightInfo(QSqlQueryModel *model);//Сигнал по запросу стат рейсов
    void sig_SendReqMonthStatForGraph(QVector<double> &xAxe, QVector<double> &yAxe);//Сигнал по запросу мес. статистики
    void sig_SendReqYearStatForBarChart(QVector<double> &xAxe, QVector<double> &yAxe);//Сигнал по запросу годовой статистики
};

#endif // DATABASE_H
