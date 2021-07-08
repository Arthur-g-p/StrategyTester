#include "mainwindow.h"

#include <QApplication>

#include <QtCharts/QLineSeries>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QCategoryAxis>

//QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show(); //starts window
    return a.exec();
}
