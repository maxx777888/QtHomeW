#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>


#define BIND_PORT 12345 //Порт для таймера
#define TEXT_PORT 1234  //Порт для передачи сообщения

class UDPworker : public QObject
{
    Q_OBJECT
public:
    explicit UDPworker(QObject *parent = nullptr);
    void InitSocket( void );
    void InitTextSocket( void );
    void ReadDatagram( QNetworkDatagram datagram);
    void SendDatagram(QByteArray data );
    void SendTextDatagram(QByteArray data );


private slots:
    void readPendingDatagrams(void);

private:
    QUdpSocket* serviceUdpSocket;
    QUdpSocket* textUdpSocket;

signals:
    void sig_sendTimeToGUI(QDateTime data);
    void sig_sendTextToGui(QString senderText, QString sizeText);

};

#endif // UDPWORKER_H
