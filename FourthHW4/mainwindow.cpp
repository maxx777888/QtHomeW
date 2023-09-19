#include "mainwindow.h"
#include ".\ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent, Qt::Window)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->gb_radio->setTitle("Choose Right/Wrong");
    ui->rb_one->setChecked(true);
    ui->rb_one->setText("Right");
    ui->rb_two->setText("Wrong");
    ui->pb_button->setCheckable(true);
    ui->pb_button->setText("Progress Up");
    ui->pb_progress->setValue(50);
    ui->pb_progress->setMinimum(0);
    ui->pb_progress->setMaximum(100);

    for (int i = 0; i < 10; i++){
        ui->cmb_combo->addItem("Item " + QString::number(i+1));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}





void MainWindow::on_pb_button_toggled(bool checked)
{
    if (ui->pb_progress->value() >= 100){
        ui->pb_progress->setValue(0);
    } else {
        ui->pb_progress->setValue(ui->pb_progress->value() + 10);
    }
}

