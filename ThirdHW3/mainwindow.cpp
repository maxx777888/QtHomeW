#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_conn_clicked()
{
    ui->lt_conn->setText("Идет подключение к БД...");
}


void MainWindow::on_pb_cancel_clicked()
{
    ui->lt_conn->setText("Отмена подключения...");
}

