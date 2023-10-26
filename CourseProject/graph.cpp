#include "graph.h"
#include "ui_graph.h"


Graph::Graph(QWidget *parent) :
    QMainWindow(parent, Qt::Window|Qt::WindowMinMaxButtonsHint),
    ui(new Ui::Graph)
{
    ui->setupUi(this);

    //Добавляем называние месяцев в QComboBox
    ui->cb_month->addItems(monthsList);
    ui->tb_Group->setCurrentIndex(1);


    //Создаем объект класса qcustgraph
    monthlyGraph = new qCustGraph(ui->customPlotMonth);
    //Создаем объект класса qcustbarchart
    yearGraph = new qCustBarChart(ui->customPlotYear);

}

Graph::~Graph()
{
    delete ui;
    delete monthlyGraph;
    delete yearGraph;
}

//Метод отрисовывает линейный месячный график
void Graph::rcvMonthlyStatForGraph(QVector<double> xAxe, QVector<double> yAxe)
{
    monthlyGraph->ClearGraph(ui->customPlotMonth);
    monthlyGraph->AddDataToGrahp(xAxe,yAxe);
    monthlyGraph->UpdateGraph(ui->customPlotMonth);
}

//Метод отрисовывает годовую столбиковую диаграмму
void Graph::rcvYearStatForBarChart(QVector<double> xAxe, QVector<double> yAxe)
{
    yearGraph->CrearBarChart(ui->customPlotYear);
    yearGraph->BuildAxeY(ui->customPlotYear, monthsList);
    auto maxNumIt = std::max_element(yAxe.begin(), yAxe.end());
    int maxNum = *maxNumIt;
    yearGraph->UpdateGraph(ui->customPlotYear, maxNum);
    yearGraph->plot(xAxe,yAxe);
    yearGraph->rebuildBarChar(ui->customPlotYear);
}

//Обработчик кнопки закрытия графика
void Graph::on_pb_GraphWindowClose_clicked()
{
    emit sig_graphWindowClose();
    this->close();
    ui->cb_month->setCurrentIndex(0);
}


//Изменение месяца в QComboBox для отображения всех месяцев
void Graph::on_cb_month_currentTextChanged(const QString &arg1)
{
    QString monthName = ui->cb_month->currentText();

    int monthNum = monthsList.indexOf(monthName) + 1; // Номер месяца

    emit sig_newMonthGraphData(monthNum);

}



//Метод для обработки смены вкладок с типами графиков
void Graph::on_tb_Group_currentChanged(int index)
{
    if (index == 0) {


    } else if (index == 1) {
        ui->cb_month->setCurrentIndex(0);
    }
}

