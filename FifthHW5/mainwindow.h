#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stopwatch.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pb_startStop_toggled(bool checked);
    void RcvSwatchData(QString ms, QString s, QString m, QString h);
    void RcvRoundData(QString round, QString s);

    void on_pb_clearAll_clicked();

    void on_pb_round_clicked();

private:
    Ui::MainWindow *ui;
    Stopwatch* stopWatch;
};
#endif // MAINWINDOW_H
