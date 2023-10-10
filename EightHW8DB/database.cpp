#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{

    dataBase = new QSqlDatabase();
    //Объект QSqlQuery отвечает за формирование запросов к БД
    modelQuery = new QSqlQueryModel();
    //QTableWidget необходим для формирования отображения ответа и передачи его в форму.
    tableView = new QTableView();


}

DataBase::~DataBase()
{
    delete dataBase;
}

/*!
 * \brief Метод добавляет БД к экземпляру класса QSqlDataBase
 * \param driver драйвер БД
 * \param nameDB имя БД (Если отсутствует Qt задает имя по умолчанию)
 */
void DataBase::AddDataBase(QString driver, QString nameDB)
{

    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);

}

/*!
 * \brief Метод подключается к БД
 * \param для удобства передаем контейнер с данными необходимыми для подключения
 * \return возвращает тип ошибки
 */
void DataBase::ConnectToDataBase(QVector<QString> data)
{

    dataBase->setHostName(data[hostName]);
    dataBase->setDatabaseName(data[dbName]);
    dataBase->setUserName(data[login]);
    dataBase->setPassword(data[pass]);
    dataBase->setPort(data[port].toInt());
    bool status;
    status = dataBase->open();

    ///Тут должен быть код ДЗ
    if (status){
        tableModel = new QSqlTableModel(this, dataBase);
    }



    emit sig_SendStatusConnection(status);

}
/*!
 * \brief Метод производит отключение от БД
 * \param Имя БД
 */
void DataBase::DisconnectFromDataBase(QString nameDb)
{

    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();

}
/*!
 * \brief Метод формирует запрос к БД.
 * \param request - SQL запрос
 * \return
 */
void DataBase::RequestToDB(QString request)//Добавил код из примера
{

    ///Тут должен быть код ДЗ


    *modelQuery = new QSqlQueryModel(this, *dataBase);
    //*modelQuery = QSqlQueryModel(*dataBase);

    QSqlError err;
    if(modelQuery->lastError().isValid()){
        err = modelQuery->lastError();
    }

    emit sig_SendStatusRequest(err);

}


void DataBase::ReadAnswerFromDB()// Добавляет заголовки в таблице
{

//        tableWidget->setColumnCount(2);
//        tableWidget->setRowCount(0);
//        QStringList hdrs;
//        hdrs << "Название фильма" << "Описание фильма";
//        tableWidget->setHorizontalHeaderLabels(hdrs);

//        uint32_t conterRows = 0;

//        while(simpleQuery->next()){
//            QString str;
//            tableWidget->insertRow(conterRows);

//            for(int i = 0; i<tableWidget->columnCount(); ++i){

//                str = simpleQuery->value(i).toString();
//                QTableWidgetItem *item = new QTableWidgetItem(str);
//                tableWidget->setItem(tableWidget->rowCount() - 1, i, item);

//            }
//            ++conterRows;
//        }

    //modelQuery->setQuery(request);
    modelQuery->setHeaderData(0, Qt::Horizontal, tr("Название фильма"));
    modelQuery->setHeaderData(1, Qt::Horizontal, tr("Описание фильма"));

        emit sig_SendDataFromDB(tableWidget);


}


/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
