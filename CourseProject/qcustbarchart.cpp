#include "qcustbarchart.h"

qCustBarChart::qCustBarChart(QCustomPlot* cPlot)
{
    ptrBar = new QCPBars(cPlot->xAxis, cPlot->yAxis);

    // set dark background gradient:
    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(90, 90, 90));
    gradient.setColorAt(0.38, QColor(105, 105, 105));
    gradient.setColorAt(1, QColor(70, 70, 70));
    cPlot->setBackground(QBrush(gradient));

    // prepare x axis with country labels:
    cPlot->xAxis->setBasePen(QPen(Qt::white));
    cPlot->xAxis->setTickPen(QPen(Qt::white));
    cPlot->xAxis->grid()->setVisible(true);
    cPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    cPlot->xAxis->setTickLabelColor(Qt::white);
    cPlot->xAxis->setLabelColor(Qt::white);

    // prepare y axis:
    cPlot->yAxis->setLabel("Кол-во рейсов в месяц");
    cPlot->yAxis->setPadding(5);
    cPlot->yAxis->setBasePen(QPen(Qt::white));
    cPlot->yAxis->setTickPen(QPen(Qt::white));
    cPlot->yAxis->setSubTickPen(QPen(Qt::white));
    cPlot->yAxis->grid()->setSubGridVisible(true);
    cPlot->yAxis->setTickLabelColor(Qt::white);
    cPlot->yAxis->setLabelColor(Qt::white);
    cPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    cPlot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

    cPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);


}


//Метод подготавливает размер оси У
void qCustBarChart::UpdateGraph(QCustomPlot *cPlot, int yScale)
{
    //cPlot->clearGraphs();
    cPlot->yAxis->setRange(0, yScale + ((yScale / 10 < 1) ? 2 : yScale/10));
}
//Метод подготавливает разметку оси Х
void qCustBarChart::BuildAxeY(QCustomPlot *cPlot, QStringList xAxeData)
{
    QVector<double> ticks;

    for(int i = 0; i < xAxeData.size(); i++){
        ticks << i+1;
    }
    cPlot->xAxis->setRange(0, xAxeData.size()+1);
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, xAxeData);
    cPlot->xAxis->setTicker(textTicker);
    cPlot->xAxis->setTickLabelRotation(70);
    cPlot->xAxis->setSubTicks(false);
    cPlot->xAxis->setTickLength(0, 1);
}
//Метод удаляет старый график
void qCustBarChart::CrearBarChart(QCustomPlot *cPlot)
{
    cPlot-> removeGraph(0);
}
//Метод получает данные и строит график
void qCustBarChart::plot(const QVector<double> &xData, const QVector<double> &yData)
{ 
    ptrBar->setData(xData, yData);
    ptrBar->setAntialiased(false);
    ptrBar->setStackingGap(0.5);
    ptrBar->setPen(QPen(QColor(111, 9, 176).lighter(170)));
    ptrBar->setBrush(QColor(111, 9, 176));

}
//Метод регистрирует полученные данные и переносит эти данные на график
void qCustBarChart::rebuildBarChar(QCustomPlot *cPlot)
{
    cPlot->replot();
}


