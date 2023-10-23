#include "database.h"

database::database(QObject *parent)
    : QObject{parent}
{

    dataBase = new QSqlDatabase();


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

    if (status){

        //quiryModel = new QSqlQueryModel;

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
