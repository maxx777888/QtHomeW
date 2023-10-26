#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtConcurrent>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QTimer>
#include <QDebug>
#include "database.h"
#include "graph.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void dbAbleConnection();//Метод который запускает подключение к БД в отдельном потоке

public slots:

    void ReceiveStatusConnectionToDB(bool status);//Слот для сообщении о подключении к БД

    void ScreenDataFromDBQueryModel(QSqlQueryModel *model);//Слот для вывода списка аэропортов

    void ScreenFlightInfo(QSqlQueryModel *model);//Слот для вывода запросов вылетов прилетов

    void rcvGraphWindowClosed();//Слот отработает когда граф закроется
    //Слот отработает когда граф закроется
    void rcvDataFromMonthlyTextChange(int monNu);



private:
    Ui::MainWindow *ui;
    Graph *graphWindow;
    QTimer* timer;//Объек таймера
    database* dataBase;//Объект базы данных
    QMessageBox* msg;//Объект окна ошибки
    int ms = 0, s = 0;//Переменные которые нужны для работы таймера
    QString airportCode = "";//Переменная для запроса КОДА аэропорта
    QString airportName = "";//Переменная для получения имени аэропорта
    bool isQTConcurrentRunning = false;//Проверка на завершение работы потока для списка аэропортов
    bool isGraphQtConcurrentRunning = false;//Проверка на завершение работы потока для мес. стат. и вывода графов
    bool isBarChartQtConcurrentRunning = false;//Проверка на завершение работы потока для год. стат. и вывода графов
    //Запускаем обработку запроса к БД по годовой статистике в отдельном потоке
    void SendRequestForYearStatBarChart(QString airportCode);

private slots:
    void TimerSlot();//Слот для работы с таймером
    void on_cb_AirPortList_currentIndexChanged(int index);//ComboBox названий аэропортов
    void on_pb_getFlightInto_clicked();//Кнопка получения информации рейсов
    void on_pb_getGraphWindowOpen_clicked();

//signals:
//    void sig_getYearStatForBarChar(QString airportCode);
};
#endif // MAINWINDOW_H
