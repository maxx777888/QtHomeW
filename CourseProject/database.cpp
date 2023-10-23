#include "database.h"

database::database(QObject *parent)
    : QObject{parent}
{
    //Создаем объект БД
    dataBase = new QSqlDatabase();
    //Создаем объек запроса к БД изначального списка аэропортов
    airportNamesQuery = new QSqlQueryModel;
    flightInfoQuery = new QSqlQueryModel;



}

database::~database()
{
    delete dataBase;
}

/*!
 * \brief Метод добавляет БД к экземпляру класса QSqlDataBase
 * \param driver драйвер БД
 * \param nameDB имя БД (Если отсутствует Qt задает имя по умолчанию)
 */
void database::AddDataBase(QString driver, QString nameDB)
{
    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);
}

/*!
 * \brief Метод подключается к БД
 * \param для удобства передаем контейнер с данными необходимыми для подключения
 * \return возвращает тип ошибки
 */
void database::ConnectToDataBase()
{


    dataBase->setHostName(hostName);
    dataBase->setDatabaseName(dbName);
    dataBase->setUserName(login);
    dataBase->setPassword(password);
    dataBase->setPort(portNum.toInt());

    bool status;
    status = dataBase->open();

    if (status){ //Если есть подключение сразу запрашиваем список аэропортов

        airportNamesQuery->setQuery(airportListRequest, *dataBase);
        emit sig_SendDataFromDBQuiryModel(airportNamesQuery);

    }

    emit sig_SendStatusConnection(status);

}
//Метод отключения от БД
void database::DisconnectFromDataBase()
{
    *dataBase = QSqlDatabase::database(DB_NAME);
    dataBase->close();
}

//Метод показывает ошибку подлючения к БД
QSqlError database::GetLastError()
{
    return dataBase->lastError();
}

//Метод осуществляет запросы к БД
void database::RequestToDB(QString airportCode, QString dateFl,int req)
{
    //Запрос прибывающих рейсов
    QString rqArrival = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as \"Name\" FROM bookings.flights f JOIN bookings.airports_data ad ON ad.airport_code = f.departure_airport WHERE scheduled_arrival::date ='"+dateFl+"' AND f.arrival_airport ='"+airportCode+"'";

    //Запрос вылетающих рейсов
    QString rqDeparture = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' as \"Name\" FROM bookings.flights f JOIN bookings.airports_data ad ON ad.airport_code = f.arrival_airport WHERE scheduled_departure::date ='"+dateFl+"' AND f.departure_airport ='" + airportCode + "'";


    if (req == requestArrival) {
        flightInfoQuery->setQuery(rqArrival, *dataBase);
        int ansA = flightInfoQuery->rowCount();
        if(flightInfoQuery->rowCount() != 0) {

            flightInfoQuery->setHeaderData(0, Qt::Horizontal, tr("№ Рейса"));
            flightInfoQuery->setHeaderData(1, Qt::Horizontal, tr("Время прибытия"));
            flightInfoQuery->setHeaderData(2, Qt::Horizontal, tr("Аэропорт вылета"));

            emit sig_SendRequstFlightInfo(flightInfoQuery);
        }

    }
    if (req == requestDeparture) {

        flightInfoQuery->setQuery(rqDeparture, *dataBase);
        int ansD = flightInfoQuery->rowCount();
        if(flightInfoQuery->rowCount() != 0) {

            flightInfoQuery->setHeaderData(0, Qt::Horizontal, tr("№ Рейса"));
            flightInfoQuery->setHeaderData(1, Qt::Horizontal, tr("Время прибытия"));
            flightInfoQuery->setHeaderData(2, Qt::Horizontal, tr("Аэропорт вылета"));

            emit sig_SendRequstFlightInfo(flightInfoQuery);
        }
    }





}
