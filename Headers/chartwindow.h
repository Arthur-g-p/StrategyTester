#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H

#include <QWidget>
#include <QtCharts/QLineSeries>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QCategoryAxis>
#include <QtWidgets/QComboBox>
#include <QTableWidget>

#include "./Headers/stockdata.h"
#include "./Headers/strategy.h"
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

private slots:
    void on_performanceTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

private:
    Ui::chartwindow *ui;
    QStringList headers;
    QTableWidget *tableWidget = new QTableWidget;
    QScatterSeries *seriesHighlight;
    QScatterSeries *seriesOpen;
    QScatterSeries *seriesClose;
    QVector<float> simpleMovingAverage(const unsigned short number, const QVector<dataframe> *data);
    void addHorizontalHeaderLabel(QString headerLabel);
    void fillDetails(const signalPoints allSignals);
    void fillTradesList(const signalPoints allSignals);
};

#endif // CHARTWINDOW_H
