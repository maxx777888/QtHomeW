#ifndef QCUSTBARCHART_H
#define QCUSTBARCHART_H

#include <QObject>
#include "qcustomplot.h"

class qCustBarChart : public QObject
{
    Q_OBJECT
public:

    qCustBarChart(QCustomPlot *cPlot);
    //void ClearGraph(QCustomPlot* cPlot);
    void UpdateGraph(QCustomPlot *cPlot, int yScale);

    void BuildAxeY(QCustomPlot *cPlot, QStringList xAxeData);

    void CrearBarChart(QCustomPlot *cPlot);

    void plot(const QVector<double> &xData, const QVector<double> &yData);
    void rebuildBarChar(QCustomPlot *cPlot);


private:

    QCPBars *ptrBar;

};

#endif // QCUSTBARCHART_H
