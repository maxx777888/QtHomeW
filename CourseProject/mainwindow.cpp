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
    //Запросы к БД и получение ответов от БД
    connect(dataBase, &database::sig_SendDataFromDBQuiryModel, this, &MainWindow::ScreenDataFromDBQueryModel);

    connect(dataBase, &database::sig_SendRequstFlightInfo, this, &MainWindow::ScreenFlightInfo);


    //Объект и коннекшин для запуска таймера
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::TimerSlot);

    dbAbleConnection();
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
//Метод выводит в comboBox список аэропортов
void MainWindow::ScreenDataFromDBQueryModel(QSqlQueryModel *model)
{
    if(model->rowCount() == 0) {
        ui->cb_AirPortList->close();
    }
    ui->cb_AirPortList->setModel(model);

    ui->cb_AirPortList->setModelColumn(0);




}

void MainWindow::ScreenFlightInfo(QSqlQueryModel *model)
{
    if(model->rowCount() == 0) {
        ui->tb_FlightInfo->close();
    }
    ui->tb_FlightInfo->setModel(model);
    ui->tb_FlightInfo->resizeRowsToContents();
    ui->tb_FlightInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tb_FlightInfo->show();
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

//Метод получает при выборе аэропорта индекс и сохраняет в строке код-аэропорта
void MainWindow::on_cb_AirPortList_currentIndexChanged(int index)
{
    airportCode = ui->cb_AirPortList->model()->index(index, 1).data().toString();
}


void MainWindow::on_pushButton_clicked()
{
    QString dateF = ui->de_DateFlight->date().toString("yyyy-MM-dd");


    if (ui->rb_Arr->isChecked()){

//        auto req = [&]{dataBase->RequestToDB(airportCode, dateF, requestArrival);};
//        QtConcurrent::run(req);

        auto req = [&]{dataBase->RequestToDB(airportCode, dateF, requestArrival);};
        try {
            QtConcurrent::run(req);
            qDebug() << "No Errors arrivel";
        } catch (const std::exception& e) {
            qDebug() << "Error: " << e.what();
        }

    } else if (ui->rb_Dep->isChecked()){

//        auto req = [&]{dataBase->RequestToDB(airportCode, dateF, requestDeparture);};
//        QtConcurrent::run(req);

        auto req = [&]{dataBase->RequestToDB(airportCode, dateF, requestDeparture);};
        try {
            QtConcurrent::run(req);
            qDebug() << "No Errors departure ";
        } catch (const std::exception& e) {
            qDebug() << "Error: " << e.what();
        }
    }

}

