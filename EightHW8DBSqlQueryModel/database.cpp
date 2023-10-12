#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{

    dataBase = new QSqlDatabase();


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


    ///Тут должен быть код ДЗ
    bool status;
    status = dataBase->open();

    if (status){

        quiryModel = new QSqlQueryModel;
        model = new QSqlTableModel(this, *dataBase);
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
void DataBase::RequestToDB(int requestType)
{
    ///Тут должен быть код ДЗ
    QString request = "";

    switch (requestType)
    {
    case requestAllFilms:
    {
        request = "SELECT title, description FROM film";

        model->setTable("film");
        model->select();
        model->setHeaderData(0, Qt::Horizontal, tr("Название фильма"));
        model->setHeaderData(1, Qt::Horizontal, tr("Описание фильма"));
        model->setQuery(request, *dataBase);
        emit sig_SendDataFromDBTableModel(model);
        break;
    }
    case requestComedy:
    {
        request = "SELECT title, description FROM film f "
                  "JOIN film_category fc on f.film_id = fc.film_id "
                  "JOIN category c on c.category_id = fc.category_id "
                  "WHERE c.name = 'Comedy'";
        quiryModel->setQuery(request, *dataBase);
        quiryModel->setHeaderData(0, Qt::Horizontal, tr("Название фильма"));
        quiryModel->setHeaderData(1, Qt::Horizontal, tr("Описание фильма"));
        emit sig_SendDataFromDBQuiryModel(quiryModel);
        break;
    }
    case requestHorrors:
    {
        request = "SELECT title, description FROM film f "
                  "JOIN film_category fc on f.film_id = fc.film_id "
                  "JOIN category c on c.category_id = fc.category_id "
                  "WHERE c.name = 'Horror'";
        quiryModel->setQuery(request, *dataBase);
        quiryModel->setHeaderData(0, Qt::Horizontal, tr("Название фильма"));
        quiryModel->setHeaderData(1, Qt::Horizontal, tr("Описание фильма"));
        emit sig_SendDataFromDBQuiryModel(quiryModel);
        break;
    }

    default:
        break;
    }


}

/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
