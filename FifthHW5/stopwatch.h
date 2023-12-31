#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QTimer>



class Stopwatch : public QObject
{
    Q_OBJECT
public:
    explicit Stopwatch(QObject *parent = nullptr);


    void startTimer();
    void stopTimer();

    void resetTimer();
    void addRound();


signals:
    void sig_sWatch(QString ms, QString s, QString m, QString h);
    void sig_addRound(QString round, QString s);


private:
    QTimer* timer;
    int roundTime = 0;
    int roundS = 0;
    int ms, s, m, h;
    int round;



private slots:
    void updateTimer();

};

#endif // STOPWATCH_H
