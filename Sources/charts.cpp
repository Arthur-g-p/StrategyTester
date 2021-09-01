#include "./Headers/charts.h"

charts::charts(QString title)
{
    QLineSeries *series = new QLineSeries();
    series->append(0,1);
    series->append(1,2);
    series->append(2,4);
    series->append(3,8);
    series->append(4,8);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle(title);


    QPen pen(QRgb(0x000000));
    pen.setWidth(5);
    series->setPen(pen);


    QCategoryAxis *axisX = new QCategoryAxis();
    axisX->append("1885",0);
    axisX->append("1886",1);
    axisX->append("1887",2);
    axisX->append("1888",3);
    axisX->append("1889",7);
    axisX->append("2000",4);
    chart->setAxisX(axisX, series);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
}

QChartView* charts::getChartView()
{
    return chartView;
}
