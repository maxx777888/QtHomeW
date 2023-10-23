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

/*
 * Уже почти ночью пришла такая мысль, чтобы проверку на работу потока
 * С помощью булевой переменной isQTConcurrentRunning провераю если поток еще в работе
 * и не запускаю новыю запрос пока не закончит работу старый
 * Вроде бы сейчас приложение не вылетает,
 * ное могли бы вы оценить эту задумку и сказать это правильное решение?
 * Или я еще где-то ошибаюсь?
 */
void MainWindow::on_pushButton_clicked()
{
    QString dateF = ui->de_DateFlight->date().toString("yyyy-MM-dd");


    if (ui->rb_Arr->isChecked()){

       auto req = [&]{dataBase->RequestToDB(airportCode, dateF, requestArrival);};
//        QtConcurrent::run(req);

//        auto req = [&]{dataBase->RequestToDB(airportCode, dateF, requestArrival);};
//        try {
//            auto future =QtConcurrent::run(req);
//            if (future.isRunning()) {
//                qDebug() << "Arrival Task is still running";
//            } else if (future.isFinished()) {
//                qDebug() << "Arrival Task has finished";
//            }

//        } catch (const std::exception& e) {
//            qDebug() << "Error: " << e.what();
//        }

        if (!isQTConcurrentRunning) {
            isQTConcurrentRunning = true;
            auto future = QtConcurrent::run(req);
            future.waitForFinished();
            isQTConcurrentRunning = false;
        }

    } else if (ui->rb_Dep->isChecked()){

//        auto req = [&]{dataBase->RequestToDB(airportCode, dateF, requestDeparture);};
//        QtConcurrent::run(req);

        auto req = [&]{dataBase->RequestToDB(airportCode, dateF, requestDeparture);};
//        try {
//            auto future =QtConcurrent::run(req);
//            if (future.isRunning()) {
//                qDebug() << "Departure Task is still running";
//            } else if (future.isFinished()) {
//                qDebug() << "Departure Task has finished";
//            }
//        } catch (const std::exception& e) {
//            qDebug() << "Error: " << e.what();
//        }

        if (!isQTConcurrentRunning) {
            isQTConcurrentRunning = true;
            auto future = QtConcurrent::run(req);
            future.waitForFinished();
            isQTConcurrentRunning = false;
        }
    }

}

