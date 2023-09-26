#include "stopwatch.h"



Stopwatch::Stopwatch(QObject *parent)
    : QObject{parent}
{
    ms = 0;
    s = 0;
    m =0;
    h = 0;
    round = 0;
    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &Stopwatch::updateTimer);

}

void Stopwatch::SendStopwatchData()
{
    emit sig_stopwatchData();
}

void Stopwatch::startTimer(int mls)
{
    timer->start(mls);
}

void Stopwatch::stopTimer()
{
    timer->stop();
}

void Stopwatch::updateTimer()
{
    timer->start(10);
    ms++;
    if(ms >= 100) {
        ms = 0;
        s++;
    }
    if (s >= 60) {
        s = 0;
        m++;
    }
    if (m >= 60) {
        m = 0;
        h++;
    }
}



