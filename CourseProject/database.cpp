#include "database.h"

database::database(QObject *parent)
    : QObject{parent}
{
    //Создаем объект БД
    dataBase = new QSqlDatabase();
    //Создаем объек запроса к БД изначального списка аэропортов
    airportNamesQuery = new QSqlQueryModel(this);
    flightInfoQuery = new QSqlQueryModel(this);
    monthStatQuery = new QSqlQueryModel(this);
    yearStatQuery = new QSqlQueryModel(this);
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

//Метод подключается к БД
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

//Метод осуществляет запросы к БД по поводу ежедневных рейсов
void database::RequestToDBToGetAirportName(QString airportCode, QString dateFl,int req)
{
    //Запрос прибывающих рейсов
    QString rqArrival = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' "
                        "as \"Name\" FROM bookings.flights f JOIN bookings.airports_data ad "
                        "ON ad.airport_code = f.departure_airport "
                        "WHERE scheduled_arrival::date ='"+dateFl+"' AND f.arrival_airport ='"+airportCode+"'";

    //Запрос вылетающих рейсов
    QString rqDeparture = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' "
                          "as \"Name\" FROM bookings.flights f JOIN bookings.airports_data ad "
                          "ON ad.airport_code = f.arrival_airport "
                          "WHERE scheduled_departure::date ='"+dateFl+"' AND f.departure_airport ='" + airportCode + "'";


    if (req == requestArrival) {
        flightInfoQuery->setQuery(rqArrival, *dataBase);
        if(flightInfoQuery->rowCount() != 0) {

            flightInfoQuery->setHeaderData(0, Qt::Horizontal, tr("№ Рейса"));
            flightInfoQuery->setHeaderData(1, Qt::Horizontal, tr("Время вылета"));
            flightInfoQuery->setHeaderData(2, Qt::Horizontal, tr("Аэропорт отправления"));

            emit sig_SendRequstFlightInfo(flightInfoQuery);
        }

    }
    if (req == requestDeparture) {

        flightInfoQuery->setQuery(rqDeparture, *dataBase);
        if(flightInfoQuery->rowCount() != 0) {

            flightInfoQuery->setHeaderData(0, Qt::Horizontal, tr("№ Рейса"));
            flightInfoQuery->setHeaderData(1, Qt::Horizontal, tr("Время вылета"));
            flightInfoQuery->setHeaderData(2, Qt::Horizontal, tr("Аэропорт назначения"));

            emit sig_SendRequstFlightInfo(flightInfoQuery);
        }
    }

}
//Метод обрабатывает информацию от БД полученную по месячной статистики
void database::MonthlyRequestToDB(QString airportCode, int month)
{
    //Подготовка строки запроса к БД
    int year;
    if (month > 8) {
        year = 2016;
    } else if (month <=8) {
        year = 2017;
    }
    QDate date(year, month, 1); // Создание объекта QDate
    int daysInMonth = date.daysInMonth(); // Получение количества дней в месяце
    QString reqDaysInMonthStr;
    QString reqStartMonthStr;
    if(month < 10) {
      reqDaysInMonthStr = QString::number(year) + "-0" + QString::number(month) + "-" + QString::number(daysInMonth);
      reqStartMonthStr =  QString::number(year) + "-0" + QString::number(month) + "-01";
    } else {
      reqDaysInMonthStr = QString::number(year) + "-" + QString::number(month) + "-" + QString::number(daysInMonth);
      reqStartMonthStr =  QString::number(year) + "-" + QString::number(month) + "-01";
    }

    QString req = "SELECT count(flight_no), date_trunc('day', scheduled_departure) "
                  "as \"Day\" from bookings.flights f "
                  "WHERE(scheduled_departure::date > date('"+reqStartMonthStr+"') "
                  "and scheduled_departure::date <= date('"+reqDaysInMonthStr+"')) "
                  "and (departure_airport = '"+airportCode+"' or arrival_airport = '"+airportCode+"') "
                  "GROUP BY \"Day\"" ;

    monthStatQuery->setQuery(req, *dataBase);
    if (monthStatQuery->rowCount() != 0){

      int count = monthStatQuery->rowCount();
      QVector<double> countArray(count);
      QVector<QString> dayArray(count);

      for (int i = 0; i < count; ++i) {
            countArray[i] = monthStatQuery->data(monthStatQuery->index(i, 0)).toInt();
            dayArray[i] = monthStatQuery->data(monthStatQuery->index(i, 1)).toString();
      }
      //Отбираем массив с данными в формате QString и выбираем только месяц для переноса в формат Double
      QVector<double> dayOfMonthArray(count);
      for (int i = 0; i < count; ++i) {
            QString helper = dayArray[i].mid(8, 2);
            dayOfMonthArray[i] = helper.toDouble();
      }

      emit sig_SendReqMonthStatForGraph(dayOfMonthArray, countArray);
    }

}
//Метод обрабатывает запрос к БД для получения данных по годичной статистике
void database::YearRequestToDB(QString airportCode)
{
    QString req = "SELECT count(flight_no), date_trunc('month', scheduled_departure) "
                  "as \"Month\" from bookings.flights f "
                  "WHERE(scheduled_departure::date > date('2016-08-31') and "
                  "scheduled_departure::date <= date('2017-08-31')) "
                  "and ( departure_airport = '"+airportCode+"' or arrival_airport = '"+airportCode+"') "
                  "GROUP BY \"Month\"";

    yearStatQuery->setQuery(req, *dataBase);
    if (yearStatQuery->rowCount() != 0){

      int count = yearStatQuery->rowCount();
      QVector<double> countArray(count);
      QVector<QString> monthArray(count);

      for (int i = 0; i < count; ++i) {
            countArray[i] = yearStatQuery->data(yearStatQuery->index(i, 0)).toInt();
            monthArray[i] = yearStatQuery->data(yearStatQuery->index(i, 1)).toString();
      }
      //Отбираем массив с данными в формате QString и выбираем только месяц для переноса в формат Double
      QVector<double> monthOFYearArray(count);
      for (int i = 0; i < count; ++i) {
            QString helper = monthArray[i].mid(5, 2);
            monthOFYearArray[i] = helper.toDouble();
      }
      //Отсортировываем данные в массивах, что данные шли от января по декабрь по порядку
      int h1 = 0; //Тех. помощник 1
      int h2 = 8; //Тех. помощник 2
      QVector<double> countArraySorted(count);
      QVector<double> monthOFYearArraySorted(count);
      for(int i = 0; i < count; ++i){
            if (i > 3){
                countArraySorted[h1] = countArray[i];
                monthOFYearArraySorted[h1] = monthOFYearArray[i];
                h1++;
            } else if (i <= 3) {
                countArraySorted[h2] = countArray[i];
                monthOFYearArraySorted[h2] = monthOFYearArray[i];
                h2++;
            }


      }
      emit sig_SendReqYearStatForBarChart(monthOFYearArraySorted, countArraySorted);
    }
}




