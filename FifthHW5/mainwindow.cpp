#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    stopWatch = new Stopwatch(this);
    ui->lb_secTime->setText("00 : 00 : 00 : 00");
    ui->pb_startStop->setCheckable(true);
    ui->pb_round->setEnabled(false);


    QObject::connect(stopWatch, &Stopwatch::sig_sWatch, this, &MainWindow::RcvSwatchData);
    QObject::connect(stopWatch, &Stopwatch::sig_addRound, this, &MainWindow::RcvRoundData);
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
        ui->pb_clearAll->setEnabled(false);
        stopWatch->startTimer(10);


    } else {

        ui->pb_startStop->setText("Старт");
        ui->pb_round->setEnabled(false);
        ui->pb_clearAll->setEnabled(true);
        stopWatch->stopTimer();
    }
}



void MainWindow::RcvSwatchData(QString ms, QString s, QString m, QString h)
{
    ui->lb_secTime->setText(h + " : " + m + " : " + s + " : " + ms);
}

void MainWindow::RcvRoundData(QString round, QString s)
{
    ui->tb_roundTime->append("Круг " + round  + " Время: " + s + " сек.");
}


void MainWindow::on_pb_clearAll_clicked()
{
    ui->lb_secTime->setText("00 : 00 : 00 : 00");
    ui->tb_roundTime->clear();
    stopWatch->resetTimer();
}


void MainWindow::on_pb_round_clicked()
{
    stopWatch->addRound();
}

