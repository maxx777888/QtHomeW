#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Объект для работы с графиками
    graphWindow = new Graph(this);

    //Выключаем возможности кнопок до подключения к БД
    ui->pb_getGraphWindowOpen->setEnabled(false);
    ui->pb_getFlightInto->setEnabled(false);

    ui->lb_ConStatus->setStyleSheet("QLabel { color : red; }");

    //Объекты для работы с БД
    dataBase = new database(this);//Объект базы данных
    msg = new QMessageBox(this);//Объект окна ошибки

    //Подключение к базе данных
    dataBase->AddDataBase(POSTGRE_DRIVER, DB_NAME);
    connect(dataBase, &database::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);

    //Запросы к БД и получение ответов от БД
    connect(dataBase, &database::sig_SendDataFromDBQuiryModel, this, &MainWindow::ScreenDataFromDBQueryModel);
    //Сигналы запросов для получения информации по рейсам
    connect(dataBase, &database::sig_SendRequstFlightInfo, this, &MainWindow::ScreenFlightInfo);

    //Сигнал для включения выключения функционала MainWindow, когда окно статистики с графиками открыто
    connect(graphWindow, &Graph::sig_graphWindowClose, this, &MainWindow::rcvGraphWindowClosed);

    //Подключение между запросом к БД с месячной статистикой за каждый день и выводом месячного графика
    connect(dataBase, &database::sig_SendReqMonthStatForGraph, graphWindow, &Graph::rcvMonthlyStatForGraph);
    //Получает сигнал от comboBox что текст поменялся и нужно запустить запрос к БД
    connect(graphWindow, &Graph::sig_newMonthGraphData, this, &MainWindow::rcvDataFromMonthlyTextChange);



    //Получает сигнал от БД что запрос по годовой статистики выполнен и отправляет на отрисовку годового графика
    connect(dataBase, &database::sig_SendReqYearStatForBarChart, graphWindow, &Graph::rcvYearStatForBarChart);


    //Объект и коннекшин для запуска таймера
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::TimerSlot);

    //Запуск метода соединения с БД
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
        ui->lb_ConStatus->setStyleSheet("QLabel { color : black; }");
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
    auto future = QtConcurrent::run(conn);
}
//Метод выводит в comboBox список аэропортов
void MainWindow::ScreenDataFromDBQueryModel(QSqlQueryModel *model)
{
    ui->cb_AirPortList->setModel(model);

    ui->cb_AirPortList->setModelColumn(0);
}
//Отрисовка полученной информации от БД по статистике рейсов (Прилетов или Вылетов) за день
void MainWindow::ScreenFlightInfo(QSqlQueryModel *model)
{
    ui->tb_FlightInfo->setModel(model);
    ui->tb_FlightInfo->resizeRowsToContents();
    ui->tb_FlightInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tb_FlightInfo->show();
}
//Слот для открытия функционала MainWindow после загрытия окна с графиками
void MainWindow::rcvGraphWindowClosed()
{
    ui->centralwidget->setEnabled(true);

}
//Получает сигнал от графа и нужно это передать дальше в класс БД
void MainWindow::rcvDataFromMonthlyTextChange(int monNu)
{
    //monthNumberOfGraphSaved = monNu;
    auto req = [&]{dataBase->MonthlyRequestToDB(airportCode, monNu);};

    if (!isGraphQtConcurrentRunning) {
        isGraphQtConcurrentRunning = true;
        auto future = QtConcurrent::run(req);
        future.waitForFinished();
        isGraphQtConcurrentRunning = false;
    }
}
//Запускаем в отдельном потоке запрос к БД по годичной статистике
void MainWindow::SendRequestForYearStatBarChart(QString airportCode)
{
    auto req = [&]{dataBase->YearRequestToDB(airportCode);};

    if (!isBarChartQtConcurrentRunning) {
        isBarChartQtConcurrentRunning = true;
        auto future = QtConcurrent::run(req);
        future.waitForFinished();
        isBarChartQtConcurrentRunning = false;
    }
}

//Метод выводит сообщение о подключение к БД в удачном случае и запускает таймер в негативном случае
void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if(status){
        ui->lb_ConStatus->setText("Подключено");
        ui->lb_ConStatus->setStyleSheet("QLabel { color : green; }");
        ui->pb_getGraphWindowOpen->setEnabled(true);
        ui->pb_getFlightInto->setEnabled(true);
    }
    else{//Если нет подключения выводим окно с ошибкой подключения

        msg->setIcon(QMessageBox::Critical);
        ui->lb_ConStatus->setStyleSheet("QLabel { color : red; }");
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
    airportName = ui->cb_AirPortList->model()->index(index, 0).data().toString();
}
//Обработчик кнопки получения информации, получает номер рейса, время и аэропорт
void MainWindow::on_pb_getFlightInto_clicked()
{
    QString dateF = ui->de_DateFlight->date().toString("yyyy-MM-dd");


    if (ui->rb_Arr->isChecked()){

        auto req = [&]{dataBase->RequestToDBToGetAirportName(airportCode, dateF, requestArrival);};

        if (!isQTConcurrentRunning) {
            isQTConcurrentRunning = true;
            auto future = QtConcurrent::run(req);
            future.waitForFinished();
            isQTConcurrentRunning = false;
        }

    } else if (ui->rb_Dep->isChecked()){

        auto req = [&]{dataBase->RequestToDBToGetAirportName(airportCode, dateF, requestDeparture);};

        if (!isQTConcurrentRunning) {
            isQTConcurrentRunning = true;
            auto future = QtConcurrent::run(req);
            future.waitForFinished();
            isQTConcurrentRunning = false;
        }
    }
}

//Обработчик кнопки для открытия окна с графиками
void MainWindow::on_pb_getGraphWindowOpen_clicked()
{
    graphWindow->setWindowTitle("Статистика " + airportName + " код ICAO: " + airportCode);
    ui->centralwidget->setEnabled(false);
    rcvDataFromMonthlyTextChange(1);
    SendRequestForYearStatBarChart(airportCode);


    graphWindow->show();
}

