#include "./Headers/chartwindow.h"

chartwindow::chartwindow(strategy *strat, QString title, int row, QWidget *parent) :  //Make a calculation only option for queing
    QWidget(parent),
    ui(new Ui::chartwindow)
{
    this->strategyPtr = strat;
    ui->setupUi(this);
    if (strategyPtr == nullptr) {
        this->setWindowTitle(title+" without Strategy");
    }
    this->setWindowTitle(title);
    benchmarkWithChart(title, row);
}

void chartwindow::benchmarkWithChart(QString title, int row)
{
    QLineSeries *series = new QLineSeries();
    QCategoryAxis *axisX = new QCategoryAxis();
    QPen pen(QRgb(0x000000));
    QChart *chart = new QChart();


    stockdata *sd = stockdata::getInstance();
    QPair<QVector<dataframe>, bool> asset = sd->getAssetByNamePair(title);
    QVector<dataframe> dataframes = asset.first;          //To not to play with the pointer this much and to be able to reverse it
    ui->minTextEdit->setPlainText("0");
    ui->maxTextEdit->setPlainText(QString::number(dataframes.length()));
    ui->maxTextLabel->setText("max:" +QString::number(dataframes.length()));
    seriesOpen = new QScatterSeries();
    seriesClose = new QScatterSeries();
    seriesHighlight = new QScatterSeries();
    //seriesHighlight
    seriesOpen->setName("Opened Positions");
    seriesClose->setName("Closed Positions");
    seriesOpen->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    seriesClose->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    seriesOpen->setMarkerSize(2.0);
    seriesClose->setMarkerSize(2.0);
    seriesHighlight->setMarkerSize(10.0);
    seriesOpen->append(0,0);
    seriesClose->append(0,0);
    seriesHighlight->append(0,0);
    float maximumValue = 0.0;
    pen.setWidth(5);
    series->setPen(pen);
    //Set Y-Values
    series->append(0, 0);
    for(int c = 0; c < dataframes.size(); c++) {
        float a = dataframes.at(c).close_price;
        series->append(c, a);
        if(dataframes.at(c).close_price > maximumValue) {
            maximumValue = dataframes.at(c).close_price;
        }
    }
    seriesClose->append(dataframes.size(),maximumValue);
    seriesOpen->append(dataframes.size(),maximumValue);
    seriesHighlight->append(dataframes.size(),maximumValue);
    //Set Categories
    for(int c = 0; c < dataframes.size(); c++) {
        if(c%8==0) {
            QString b = dataframes.at(c).time;
            b.remove(0, 2);
            axisX->append(b, c);
        }
    }
    axisX->setTitleText("Date");
    axisX->setTitleVisible(true);
    //int tick = asset_values->at(row).size()/10;
    axisX->setTickCount(100);
    axisX->setStartValue(4);
    //axisX->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    axisX->setLabelsAngle(90);
    //axisX->append("one year", 52);

    QLineSeries *seriesTwo;
    strategy::sma primaryMovingAverage;
    //Overrides the data and gets the current moving average
    if (strategyPtr != nullptr) {
        primaryMovingAverage = strategyPtr->getMainSma(&dataframes, true);
        //Draws the SMA from the Strategy
        seriesTwo = new QLineSeries();
        QString movingAverageStr = QString::number(primaryMovingAverage.number);
        seriesTwo->setName(movingAverageStr+" SMA");
        seriesTwo->append(0, 0);
        //that is bad to many calls.
        for(int c = primaryMovingAverage.offset; c < primaryMovingAverage.data.size()+primaryMovingAverage.offset; c++) {
            seriesTwo->append(c, primaryMovingAverage.data.at(c-primaryMovingAverage.offset));
        }
        seriesTwo->append(primaryMovingAverage.data.size()+primaryMovingAverage.offset, maximumValue);

        benchmark bench;
        signalPoints allSignals = bench.getSignals(*strategyPtr, &dataframes);

        bool open = false;
        for(int c = 0; c < allSignals.indices.size(); c++) {
            if(open) {     //If in long -> close pos with result
                //calc
                open = false;
                seriesClose->append(allSignals.indices.at(c), allSignals.value.at(c));
            } else {        //If not in pos -> open pos
                open = true;
                seriesOpen->append(allSignals.indices.at(c), allSignals.value.at(c));
            }
        }
        fillTradesList(allSignals);
    }

    chart->addSeries(seriesClose);
    chart->addSeries(seriesOpen);
    chart->addSeries(seriesHighlight);
    chart->createDefaultAxes();
    chart->addSeries(series);
    if (strategyPtr != nullptr) {
        chart->addSeries(seriesTwo);
        chart->setAxisX(axisX, seriesTwo);
    }
    chart->setAxisX(axisX, series);
    //chart->legend()->show();
    chart->setAnimationOptions(QChart::AllAnimations);
    chartView = new QChartView(chart, ui->chart_widget);
    chartView->setRubberBand(QChartView::HorizontalRubberBand);
    chartView->chart()->setTheme(QChart::ChartThemeBlueCerulean);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(ui->chart_widget->size());

    //Remove the axis setter points
    seriesHighlight->remove(0,0);
    seriesClose->remove(0,0);
    seriesOpen->remove(0,0);
    series->remove(0, 0);
    if (strategyPtr != nullptr) {
        seriesTwo->remove(0, 0);
        seriesTwo->remove(primaryMovingAverage.data.size()+primaryMovingAverage.offset, maximumValue);
    }
    seriesClose->remove(dataframes.size(),maximumValue);
    seriesOpen->remove(dataframes.size(),maximumValue);
    seriesHighlight->remove(dataframes.size(),maximumValue);
}

//Goal: FillTradesList(crossings, finance_settings) //finance_settings: Reinvest, capital amount, and other settings, short activated, fees
void chartwindow::fillTradesList(const signalPoints allSignals)
{
    QLineSeries *series = new QLineSeries();
    QLineSeries *series_index = new QLineSeries();
    QLineSeries *portfolio = new QLineSeries();

    bool shorting = false;
    bool longing = true;
    float capital = 100.0;

    bool open = false;
    bool openShort = false;
    float openPos = 0.0;

    portfolio->append(0,capital);
    series->append(0, capital);
    series_index->append(0, capital);
    int rowCount = ui->performanceTable->rowCount();

    for(int c = 0; c < allSignals.value.size(); c++) {
        if(open) {     //If in long -> close pos with result -> open short
            open = false;
            if(longing) {   //close long
                float changePercent = allSignals.value.at(c)/openPos;
                capital*=changePercent;
                if(changePercent <= 1.0) { //save results from long pos
                    changePercent = (1-changePercent)*100.0; //wrong calc?
                    ui->performanceTable->setItem(rowCount-1, 3, new QTableWidgetItem("-"+QString().setNum(changePercent, 'g', 3)));
                    ui->performanceTable->item(rowCount-1, 3)->setForeground(QBrush(QColor(255, 0, 0)));
                } else {
                    changePercent = (changePercent-1)*100.0;
                    ui->performanceTable->setItem(rowCount-1, 3, new QTableWidgetItem("+"+QString().setNum(changePercent, 'g', 3)));
                    ui->performanceTable->item(rowCount-1, 3)->setForeground(QBrush(QColor(0, 255, 0)));
                }
                ui->performanceTable->setItem(rowCount-1, 2, new QTableWidgetItem(QString::number(allSignals.value.at(c))));
                ui->performanceTable->setItem(rowCount-1, 4, new QTableWidgetItem(QString::number(capital)));
            }
            if(shorting) {  //open short
                openPos = allSignals.value.at(c);
                rowCount++;
                ui->performanceTable->setRowCount(rowCount);
                ui->performanceTable->setItem(rowCount-1, 0, new QTableWidgetItem("Short"));
                ui->performanceTable->setItem(rowCount-1, 1, new QTableWidgetItem(QString::number(openPos)));
                openShort = true;
            }

            //update chart
            series->append(c, capital);
            series_index->append(allSignals.indices.at(c), capital);
        } else {        //If not in pos -> open pos -> close short
            if(shorting) {
                if(openShort) {  //close short
                    float changePercent = openPos/allSignals.value.at(c);
                    capital*=changePercent;
                    if(changePercent <= 1.0) { //save results from short pos
                        changePercent = (1-changePercent)*100.0;
                        ui->performanceTable->setItem(rowCount-1, 3, new QTableWidgetItem("-"+QString().setNum(changePercent, 'g', 3)));
                        ui->performanceTable->item(rowCount-1, 3)->setForeground(QBrush(QColor(255, 0, 0)));
                    } else {
                        changePercent = (changePercent-1)*100.0;
                        ui->performanceTable->setItem(rowCount-1, 3, new QTableWidgetItem("+"+QString().setNum(changePercent, 'g', 3)));
                        ui->performanceTable->item(rowCount-1, 3)->setForeground(QBrush(QColor(0, 255, 0)));
                    }
                    ui->performanceTable->setItem(rowCount-1, 2, new QTableWidgetItem(QString::number(allSignals.value.at(c))));
                    ui->performanceTable->setItem(rowCount-1, 4, new QTableWidgetItem(QString::number(capital)));
                    openShort = false;
                }
            }
            open = true;
            if(longing) {   //open long
                openPos = allSignals.value.at(c);
                rowCount++;
                ui->performanceTable->setRowCount(rowCount);
                ui->performanceTable->setItem(rowCount-1, 1, new QTableWidgetItem(QString::number(openPos)));
                ui->performanceTable->setItem(rowCount-1, 0, new QTableWidgetItem("Long"));
            }
            //update chart
            portfolio->append(c, capital);
            series->append(c, capital);
            series_index->append(allSignals.indices.at(c), capital);
        }
    }

    // In order to get the drawdown, drawup etc, I surley need the stockdata.

    qInfo() << "final capital" <<  QString::number(capital);
    series->append(allSignals.value.size(), capital);
    if(!allSignals.indices.isEmpty()) {
        series_index->append(allSignals.indices.last(), capital);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->addSeries(series_index);
    chart->addSeries(portfolio);
    //chart->addSeries(seriesClose);
    //chart->addSeries(seriesOpen);
    //chart->addSeries(seriesHighlight);
    chart->legend()->hide();
    chart->createDefaultAxes();
    chartView = new QChartView(chart, ui->performance_chart_widget);
    chartView->chart()->setTheme(QChart::ChartThemeBlueCerulean);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(ui->performance_chart_widget->size());

    fillDetails(allSignals);
}


void chartwindow::fillDetails(const signalPoints allSignals)
{
    bool shorting = false;
    bool longing = true;

    bool open = false;
    bool openShort = false;
    float openPos = 0.0;
    float startingCapital = 100.0;
    float capital = startingCapital;

    int shortWin = 0;
    int shortLose = 0;
    int longWin = 0;
    int longLose = 0;
    //Compare to Buy and hold
    for(int c = 0; c < allSignals.value.size(); c++) {  //Counts wins and losses
        if(open) {     //If in long -> close pos with result
            //calc
            open = false;
            if(longing) {
                if(openPos <= allSignals.value.at(c)) {
                    longWin++;
                } else {
                    longLose++;
                }
            }
            if(shorting) {  //open short
                openPos = allSignals.value.at(c);
                openShort = true;
            }
        } else {        //If not in pos -> open pos
            if(shorting) {
                if(openShort) {  //close short
                    if(openPos < allSignals.value.at(c)) { //save results from short pos
                        shortLose++;
                    } else {
                        shortWin++;
                    }
                    openShort = false;
                }
            }
            open = true;
            if(longing) {
                openPos = allSignals.value.at(c);
            }
        }
    }

    //Wins and loses
    ui->detailsTable->setItem(0, 0, new QTableWidgetItem(QString::number(longWin+shortWin)));
    ui->detailsTable->setItem(1, 0, new QTableWidgetItem(QString::number(shortLose+shortLose)));
    ui->detailsTable->setItem(0, 1, new QTableWidgetItem(QString::number(longWin)));
    ui->detailsTable->setItem(1, 1, new QTableWidgetItem(QString::number(longLose)));
    ui->detailsTable->setItem(0, 2, new QTableWidgetItem(QString::number(shortWin)));
    ui->detailsTable->setItem(1, 2, new QTableWidgetItem(QString::number(shortLose)));
    //win rates
    if(shorting) {
        if(shortWin == 0) {
            ui->detailsTable->setItem(2, 2, new QTableWidgetItem("0%"));
        } else if(shortLose == 0) {
            ui->detailsTable->setItem(2, 2, new QTableWidgetItem("100%"));
        } else {
            float winrate =  (float)shortWin/((float)shortWin+(float)shortLose);
            ui->detailsTable->setItem(2, 2, new QTableWidgetItem(QString::number(winrate)+"%"));
        }
    }
    if(longing) {
        if(longWin == 0) {
            ui->detailsTable->setItem(2, 1, new QTableWidgetItem("0%"));
        } else if(longLose == 0) {
            ui->detailsTable->setItem(2, 1, new QTableWidgetItem("100%"));
        } else {
            float winrate =  (float)longWin/((float)longWin+(float)longLose);
            ui->detailsTable->setItem(2, 1, new QTableWidgetItem(QString::number(winrate)+"%"));
        }
    }
    if(longWin+shortWin == 0) {
        ui->detailsTable->setItem(2, 0, new QTableWidgetItem("0%"));
    } else if(longLose+shortLose == 0) {
        ui->detailsTable->setItem(2, 0, new QTableWidgetItem("100%"));
    } else {
        float winrate =  ((float)longWin+(float)shortWin)/((float)longWin+(float)longLose+(float)shortLose+(float)shortWin);
        ui->detailsTable->setItem(2, 0, new QTableWidgetItem(QString::number(winrate)+"%"));
    }

    //buy and hold comparison
    if(!allSignals.value.isEmpty()) {
        float change = allSignals.value.last()/allSignals.value.at(0);  //Das ist nicht der letzte wert sondern nur das letzte Signal
        ui->detailsTable->setItem(4, 0, new QTableWidgetItem("+"+QString().setNum(change, 'g', 3)));
    }
    float changePercent = capital/startingCapital;  //endcapital is not given!
    if(changePercent <= 1.0) {
       changePercent = (1-changePercent)*100.0;
       ui->detailsTable->setItem(3, 0, new QTableWidgetItem("-"+QString().setNum(changePercent, 'g', 3)));
       ui->detailsTable->item(3, 0)->setForeground(QBrush(QColor(255, 0, 0)));
    } else {
       changePercent = (changePercent-1)*100.0;
       ui->detailsTable->setItem(3, 0, new QTableWidgetItem("+"+QString().setNum(changePercent, 'g', 3)));
       ui->detailsTable->item(3, 0)->setForeground(QBrush(QColor(0, 255, 0)));
    }

}

QVector<float> chartwindow::simpleMovingAverage(unsigned short number, const QVector<dataframe> *data)
{
    //QLineSeries *movingAverageSeries = new QLineSeries();
    //movingAverageSeries->setName(QString::number(number) + " SMA");
    QVector<float> movingAverage;
    movingAverage.clear();
    float sum;
    number = number - 1;
    for(int c = number; c < data->size(); c++) {
        sum = 0.0f;
        for(int i = c-number; i <= c; i++) { //Sum from data[c-20] to data[c]
            sum += data->at(i).close_price;
        }
        float average = sum/(number+1);
        movingAverage.append(average);
    }
    return movingAverage;
}

void chartwindow::on_performanceTable_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentColumn);
    Q_UNUSED(previousColumn);
    QPointF p  = seriesOpen->at(currentRow);
    QPointF pOld  = seriesOpen->at(previousRow);
    QPointF pSell  = seriesClose->at(currentRow);
    QPointF pOldSell  = seriesClose->at(previousRow);
    seriesHighlight->append(p);
    seriesHighlight->append(pSell);
    seriesHighlight->remove(pOldSell);
    seriesHighlight->remove(pOld);
}

void chartwindow::on_recalculateButton_clicked()
{
        //benchmarkWithChart();
}

chartwindow::~chartwindow()
{
    delete ui;
}

/*
Ideas:
    - https://doc.qt.io/qt-5/qtcharts-callout-example.html for data label
    - for showing signals: https://doc.qt.io/qt-5/qtcharts-scatterchart-example.html
            1. add vertical line:
                draw a qimage like in the examples
                draw a normal series with two points (does it work?)
            2. add points:
                draw a point with qimage
            I think both underlie the same method
 * */

