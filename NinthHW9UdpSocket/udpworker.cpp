#include "udpworker.h"

UDPworker::UDPworker(QObject *parent) : QObject(parent)
{



}


/*!
 * @brief Метод инициализирует UDP сервер таймера
 */
void UDPworker::InitSocket()
{

    serviceUdpSocket = new QUdpSocket(this);

    /*
     * Соединяем присваиваем адрес и порт серверу и соединяем функцию
     * обраотчик принятых пакетов с сокетом
     */
    serviceUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT);


    connect(serviceUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);


}
/*!
 * @brief Метод инициализирует UDP сервер на прием сообщений самому себе
 */
void UDPworker::InitTextSocket()
{
    textUdpSocket = new QUdpSocket(this);
    textUdpSocket->bind(QHostAddress::LocalHost, TEXT_PORT);
    connect(textUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);
}

/*!
 * @brief Метод осуществляет обработку принятой датаграммы
 */
void UDPworker::ReadDatagram(QNetworkDatagram datagram)
{

    QByteArray data;
    data = datagram.data();


    if(sender() == serviceUdpSocket){ //Если сообщение пришло от таймера

        QDataStream inStr(&data, QIODevice::ReadOnly);
        QDateTime dateTime;
        inStr >> dateTime;

        emit sig_sendTimeToGUI(dateTime);

    } else if (sender() == textUdpSocket) { //Если сообщение пришло от самого себя


        QString stringSenderAddress = datagram.senderAddress().toString();
        int sizeData = datagram.data().size();

        QString sizeOfSentText = QString::number(sizeData);

        emit sig_sendTextToGui(stringSenderAddress, sizeOfSentText);
    }

}
/*!
 * @brief Метод осуществляет опередачу датаграммы таймера
 */
void UDPworker::SendDatagram(QByteArray data)
{
    /*
     *  Отправляем данные на localhost и задефайненный порт
     */
    serviceUdpSocket->writeDatagram(data, QHostAddress::LocalHost, BIND_PORT);

}
/*!
 * @brief Метод осуществляет опередачу датаграммы сообщения от самого себя
 */
void UDPworker::SendTextDatagram(QByteArray data)
{
    textUdpSocket->writeDatagram(data, QHostAddress::LocalHost, TEXT_PORT);
}

/*!
 * @brief Метод осуществляет чтение датаграм из сокета
 */
void UDPworker::readPendingDatagrams( void )
{
    /*
     *  Производим чтение принятых датаграмм
     */

    if(sender() == serviceUdpSocket){ //Если сообщение пришло от таймера

        while(serviceUdpSocket->hasPendingDatagrams()){
            QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();
            ReadDatagram(datagram);
        }

    } else if (sender() == textUdpSocket){ //Если сообщение пришло от самого себя

        while(textUdpSocket->hasPendingDatagrams()){
            QNetworkDatagram datagram = textUdpSocket->receiveDatagram();
            ReadDatagram(datagram);
    }
    }

}
