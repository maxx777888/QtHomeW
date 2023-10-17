#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pb_stop->setEnabled(false);
    ui->te_result->setReadOnly(true);

    udpWorker = new UDPworker(this);
    udpWorker->InitSocket();

    //Инициализируем udpTextWorker и подключаем к прослушиванию порта 1234
    udpTextWorker = new UDPworker(this);
    udpTextWorker->InitTextSocket();
    //Соединяем udpWorker с методом вывода данных таймера на  экран
    connect(udpWorker, &UDPworker::sig_sendTimeToGUI, this, &MainWindow::DisplayTime);
    //Соединяем udpWorker с методом вывода данных своего сообщения на экран
    connect(udpTextWorker, &UDPworker::sig_sendTextToGui, this, &MainWindow::DisplayText);

    /*
     * Необходимо раз в секунду отправлять данные. Опишем это в лямбда функции и
     * соеденим с сигналом timeout. В функции будем получать дату и время и
     * отправлять их при помощи SendDatagram
    */
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]{

        QDateTime dateTime = QDateTime::currentDateTime();

        QByteArray dataToSend;
        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

        outStr << dateTime;

        udpWorker->SendDatagram(dataToSend);
        timer->start(TIMER_DELAY);

    });

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_start_clicked() //Обработчик кнопки "Начать передачу"
{
    timer->start(TIMER_DELAY);
    ui->pb_start->setEnabled(false);
    ui->pb_stop->setEnabled(true);
}


void MainWindow::DisplayTime(QDateTime data)//Метод выводит сообщение таймера на экран
{
    counterPck++;
    if(counterPck % 20 == 0){
        ui->te_result->clear();
    }

    ui->te_result->append("Текущее время: " + data.toString() + ". "
                "Принято пакетов " + QString::number(counterPck));


}

void MainWindow::DisplayText(QString senderText, QString sizeText)//Метод выводит сообщение от самого себя
{
    ui->te_result->append(QString("Принято сообщение от адреса %1, размер сообщения (байт): %2")
                              .arg(senderText)
                              .arg(sizeText));
}


void MainWindow::on_pb_stop_clicked()//Обработчик кнопки "Остановить передачу"
{
    timer->stop();
    ui->pb_start->setEnabled(true);
    ui->pb_stop->setEnabled(false);
}


void MainWindow::on_pb_clearResult_clicked() //Обработчик кнопки "Очистить все окна"
{
    ui->te_result->clear();
    ui->te_getText->clear();
    counterPck = 0;
}


void MainWindow::on_pb_sendDataG_clicked() //Обработчик кнопки "Отправить датаграмму"
{
    QString text = ui->te_getText->toPlainText();

    QByteArray datagram = text.toUtf8();
    udpTextWorker->SendTextDatagram(datagram);
}

