#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    stopWatch = new Stopwatch(this);
    ui->lb_secTime->setText("00:00:00");
    ui->pb_startStop->setCheckable(true);
    ui->pb_round->setEnabled(false);

    QObject::connect(stopWatch, &Stopwatch::sig_stopwatchData, this, &MainWindow::RcvStopwatchData);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_startStop_toggled(bool checked)
{
    if (ui->pb_startStop->isChecked())
    {
        ui->pb_startStop->setText("Стоп");
        ui->pb_round->setEnabled(true);

        stopWatch->SendStopwatchData();
        //stopWatch->startTimer(10);

        stopWatch->SendStopwatchData();
        ui->lb_secTime->setText(QString::number(stopWatch->h) + "' : '" + QString::number(stopWatch->m) + "' : '" +
                                QString::number(stopWatch->s) + " : " + QString::number(stopWatch->ms));

    } else {
        ui->pb_startStop->setText("Старт");
        ui->pb_round->setEnabled(false);
    }
}

void MainWindow::RcvStopwatchData()
{


}

