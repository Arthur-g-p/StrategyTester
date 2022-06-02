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
    QChartView *oscChartView = new QChartView;
    explicit chartwindow(strategy *strat, QString title = "", QWidget *parent = nullptr);
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
    void addHorizontalHeaderLabel(QString headerLabel);
    void fillDetails(const signalPoints allSignals, const QVector<dataframe> *data);
    void fillTradesList(const signalPoints allSignals, const QVector<dataframe> *data);
    void benchmarkWithChart(QString title);
    void getDrawDown(const QVector<dataframe> data, float &maximumDrawDown, float &maximumDrawUp) const;
};

#endif // CHARTWINDOW_H
