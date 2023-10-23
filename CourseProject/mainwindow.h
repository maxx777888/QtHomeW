#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtConcurrent>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QTimer>
#include <QDebug>
#include "database.h"


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

private:
    Ui::MainWindow *ui;
    QTimer* timer;//Объек таймера
    database* dataBase;//Объект базы данных
    QMessageBox* msg;//Объект окна ошибки
    int ms = 0, s = 0;//Переменные которые нужны для работы таймера
    QString airportCode = "";
    bool isQTConcurrentRunning = false;

private slots:
    void TimerSlot();//Слот для работы с таймером
    void on_cb_AirPortList_currentIndexChanged(int index);
    void on_pushButton_clicked();
};
#endif // MAINWINDOW_H
