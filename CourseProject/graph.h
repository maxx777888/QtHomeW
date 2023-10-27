#ifndef GRAPH_H
#define GRAPH_H

#include <QMainWindow>
#include <QVector>
#include "qcustgraph.h"
#include "qcustbarchart.h"


namespace Ui {
class Graph;
}

class Graph : public QMainWindow
{
    Q_OBJECT

public:
    explicit Graph(QWidget *parent = nullptr);
    ~Graph();

public slots:
    void rcvMonthlyStatForGraph(QVector<double> &xAxe, QVector<double> &yAxe);//Слот для отрисовки месячного графика
    void rcvYearStatForBarChart(QVector<double> &xAxe, QVector<double> &yAxe);//Слот для отрисовки годового графика


private slots:
    void on_pb_GraphWindowClose_clicked();//Обработчик кнопки закрытия графика
    //Изменение месяца в QComboBox для отображения всех месяцев
    void on_cb_month_currentTextChanged(const QString &arg1);
    //Метод для обработки смены вкладок с типами графиков
    void on_tb_Group_currentChanged(int index);

private:
    Ui::Graph *ui;
    qCustGraph *monthlyGraph;
    qCustBarChart *yearGraph;

    QStringList monthsList = {"Январь", "Февраль", "Март", "Апрель", "Май",
                          "Июнь", "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"};



signals:
    void sig_graphWindowClose();//Сигнал о закрытии окна с графиками
    void sig_newMonthGraphData(int monNu);//Сигнал для мес. стат.
    void sig_yearWindowBarChartOpened();//Сигнал для год. стат.
    //void sig_yearStatBarGraph();
};

#endif // GRAPH_H
