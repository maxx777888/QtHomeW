#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Объекты для работы с БД
    dataBase = new database(this);//Объект базы данных
    msg = new QMessageBox(this);//Объект окна ошибки

    //Подключение к базе данных
    dataBase->AddDataBase(POSTGRE_DRIVER, DB_NAME);
    connect(dataBase, &database::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);
    dbAbleConnection();

    //Объект и коннекшин для запуска таймера
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::TimerSlot);
}

MainWindow::~MainWindow()
{
    delete ui;
}
//Метод срабатывает после запуска таймера и пытается запустить метод dbAbleConnection(), который подключается к БД
void MainWindow::TimerSlot()
{
    ms++;//Увеличение милисекунд
    if(ms >= 100) {
        ui->lb_ConStatus->setText("Подключение к базе данных через-> " + QString::number(5 - s) + " сек.");
        ms = 0;
        s++;//Увеличение секунд

    }
    if (s >= 5) {
        s = 0;
        ms = 0;
        timer->stop();
        dbAbleConnection();
    }
}

//Метод подключается к базе данных в отдельном потоке
void MainWindow::dbAbleConnection()
{
    auto conn = [&]{dataBase->ConnectToDataBase();};
    QtConcurrent::run(conn);
}
//Метод выводит сообщение о подключение к БД в удачном случае и запускает таймер в негативном случае
void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if(status){
        ui->lb_ConStatus->setText("Подключено");
    }
    else{//Если нет подключения выводим окно с ошибкой подключения

        msg->setIcon(QMessageBox::Critical);
        msg->setWindowTitle("Ошибка подключения к БД!");
        msg->setText(dataBase->GetLastError().text());
        ui->lb_ConStatus->setText("Отключено");
        msg->exec();

        if (!msg->isVisible()){//Проверка на закрытие окна
            timer->start(10); //Запускае таймер
        }
    }

}

