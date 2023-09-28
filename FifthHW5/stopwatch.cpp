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

void Stopwatch::startTimer()
{
    timer->start(10);
}

void Stopwatch::stopTimer()
{
    timer->stop();
}

void Stopwatch::resetTimer()
{
    this->ms = 0;
    this->s = 0;
    this->m =0;
    this->h = 0;
    this->round = 0;
}

void Stopwatch::addRound()
{
    int s = 0;

    if (this->round == 0) {

        this->round = 1;
        roundS = this->s + this->m*60 + this->h*360;

    } else {

        this->round++;
        s = this->s + this->m*60 + this->h*360;
        roundTime = s - roundS;
        roundS = s;

    }

    emit sig_addRound(QString::number(this->round) ,QString::number(roundTime));

}

void Stopwatch::updateTimer()
{
    QString mls;
    QString sec;
    QString min;
    QString hour;

    //Логика работы секундомера
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
    //Добавление 0 перед числом строки чтобы цифры не скакали на экране
    if (ms < 10){
        mls = "0" + QString::number(ms);
    }else {
        mls = QString::number(ms);
    }

    if (s < 10) {
        sec = "0" + QString::number(s);
    } else {
        sec = QString::number(s);
    }
    if (m < 10) {
        min = "0" + QString::number(m);
    } else {
        min = QString::number(m);
    }
    if (h < 10) {
        hour = "0" + QString::number(h);
    } else {
        hour = QString::number(h);
    }
    emit sig_sWatch(mls, sec, min, hour);
}





