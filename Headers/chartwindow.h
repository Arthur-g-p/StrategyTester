#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H

#include <QWidget>
#include <QtCharts/QLineSeries>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QCategoryAxis>
#include <QtWidgets/QComboBox>
#include "./Headers/stockdata.h"
#include "ui_chartwindow.h"

namespace Ui {
class chartwindow;
}

class chartwindow : public QWidget
{
    Q_OBJECT

public:
    QChartView *chartView = new QChartView;
    explicit chartwindow(QWidget *parent = nullptr, QString title = "", int row = 0);
    ~chartwindow();

private:
    Ui::chartwindow *ui;
};

#endif // CHARTWINDOW_H
