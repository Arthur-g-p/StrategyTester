#ifndef CHARTS_H
#define CHARTS_H

#include "stockdata.h"

#include <QApplication>

#include <QtCharts/QLineSeries>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QCategoryAxis>


class charts : public QObject
{
    Q_OBJECT
public:
    charts(QString title);
    QChartView *getChartView();
    QChartView *chartView = new QChartView;
    QChartView second;
    //void add_series(dataframe s);
    //void add
private:
    QChart *chart = new QChart();
};

#endif // CHARTS_H
