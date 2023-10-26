#include "qcustgraph.h"

qCustGraph::qCustGraph(QCustomPlot* cPlot)
{

    ptrGraph = new QCPGraph(cPlot->xAxis, cPlot->yAxis);

    //В отличии от QtCharts QCustomPlot поддерживает встроенные функции масштабирования.
    cPlot->setInteraction(QCP::iRangeZoom, true);
    cPlot->setInteraction(QCP::iRangeDrag, true);
}

/*!
    @brief Метод добавляет данные на график
*/
void qCustGraph::AddDataToGrahp(QVector<double> x, QVector<double> y)
{
    //Добавляем данные на серию
    ptrGraph->addData(x,y);
}

/*!
    @brief Метод очищает данные хранящиеся в памяти графиков
*/
void qCustGraph::ClearGraph(QCustomPlot* cPlot)
{
    //Очищаем данные
    ptrGraph->data().clear();
    cPlot->graph()->data()->clear();

    //Обновляем отображение графика
    cPlot->replot();

}

/*!
    @brief Слот обновляет отрисовку графика
*/
void qCustGraph::UpdateGraph(QCustomPlot *cPlot){
    //Масштабируем оси
    cPlot->rescaleAxes();
    //Отрисовываем график
    cPlot->replot();
}
