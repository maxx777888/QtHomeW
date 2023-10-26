#ifndef QCUSTGRAPH_H
#define QCUSTGRAPH_H

#include <QObject>
#include "qcustomplot.h"

class qCustGraph : public QObject
{
    Q_OBJECT
public:
    qCustGraph(QCustomPlot* cPlot);
    void AddDataToGrahp(QVector<double> x, QVector<double> y);
    void ClearGraph(QCustomPlot* cPlot);
    void UpdateGraph(QCustomPlot* cPlot);


private:

    QCPGraph* ptrGraph;

};

#endif // QCUSTGRAPH_H
