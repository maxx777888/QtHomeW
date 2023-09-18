#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pb_conn_clicked()
{
    ui->lb_connection->setText("Идет подключение к БД...");
}


void Dialog::on_pb_cancelConn_clicked()
{
    ui->lb_connection->setText("Нет подключения к базе ");
}

