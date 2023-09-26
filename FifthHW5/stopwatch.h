#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QTimer>



class Stopwatch : public QObject
{
    Q_OBJECT
public:
    explicit Stopwatch(QObject *parent = nullptr);
    void SendStopwatchData();
    int ms, s, m, h;
    int round;
    void startTimer(int mls);
    void stopTimer();

signals:
    void sig_stopwatchData();


private:
    QTimer* timer;


private slots:
    void updateTimer();
};

#endif // STOPWATCH_H
