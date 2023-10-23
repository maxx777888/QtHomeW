#include "mytimer.h"


MyTimer::MyTimer(QObject *parent) : QObject(parent)
{
    QTimer* timer = new QTimer(this);
    timer->setInterval(5000);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &MyTimer::onTimeout);
    timer->start();
}

void MyTimer::onTimeout()
{

}
