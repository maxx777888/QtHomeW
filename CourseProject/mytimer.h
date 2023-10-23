#ifndef MYTIMERCLASS_H
#define MYTIMERCLASS_H

#include <QObject>
#include <QTimer>
#include "mainwindow.h""

class MyTimer : public QObject
{
    Q_OBJECT

public:
    explicit MyTimer(QObject *parent = nullptr);

private slots:
    void onTimeout();
};

#endif // MYTIMERCLASS_H
