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
#include "Headers/benchmark.h"

namespace Ui {
class chartwindow;
}

class chartwindow : public QWidget
{
    Q_OBJECT

public:
    QChartView *chartView = new QChartView;
    explicit chartwindow(strategy *strat, QString title = "", int row = 0, QWidget *parent = nullptr);
    ~chartwindow();

private slots:
    void on_performanceTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_recalculateButton_clicked();

private:
    Ui::chartwindow *ui;
    QStringList headers;
    QTableWidget *tableWidget = new QTableWidget;
    QScatterSeries *seriesHighlight;
    QScatterSeries *seriesOpen;
    QScatterSeries *seriesClose;
    strategy *strategyPtr;
    QVector<float> simpleMovingAverage(const unsigned short number, const QVector<dataframe> *data);
    void addHorizontalHeaderLabel(QString headerLabel);
    void fillDetails(const signalPoints allSignals);
    void fillTradesList(const signalPoints allSignals);
    void benchmarkWithChart(QString title, int row);
};

#endif // CHARTWINDOW_H
