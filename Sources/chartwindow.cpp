#include "./Headers/chartwindow.h"

chartwindow::chartwindow(strategy *strat, QString title, QWidget *parent) :  //Make a calculation only option for queing
    QWidget(parent),
    ui(new Ui::chartwindow)
{
    this->strategyPtr = strat;
    ui->setupUi(this);
    if (strategyPtr == nullptr) {
        this->setWindowTitle(title+" without Strategy");
    } else {
        this->setWindowTitle(title);
    }
    benchmarkWithChart(title);
}

void chartwindow::benchmarkWithChart(QString title)
{
    QLineSeries *series = new QLineSeries();
    QCategoryAxis *axisX = new QCategoryAxis();
    QPen pen(QRgb(0x000000));

    QChart *chart = new QChart();
    QChart *oscChart = new QChart();



    stockdata *sd = stockdata::getInstance();
    QPair<QVector<dataframe>, bool> asset = sd->getAssetByNamePair(title);
    QVector<dataframe> dataframes = asset.first;          //To not to play with the pointer

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
    series->append(0, 0);       // Sets the dimensions of the chart

    //gets the maximum value. It is important to set the dimensions of the chart
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

    QVector<QLineSeries*> lineSeriesIndicators; //For indicators
    QVector<indicator> allIndicators;
    QLineSeries *currentSeries;
    if (strategyPtr != nullptr) {
        //Overrides the data and gets the current moving average
        allIndicators = strategyPtr->getIndicators(&dataframes, true);
        for (int c = 0; c < allIndicators.size(); c++) {
            int parameter1 = allIndicators.at(c).parameter;
            int offset = allIndicators.at(c).offset;

            //Draws the indicator from the Strategy
            currentSeries = new QLineSeries();
            QString movingAverageStr = QString::number(parameter1);
            currentSeries->setName(movingAverageStr+allIndicators.at(c).name);
            currentSeries->append(0, 0);

            //that is bad tOo many calls.
            int maxIndex = allIndicators.at(c).data.size()+offset;
            for(int i = offset; i < maxIndex; i++)
            {
                currentSeries->append(i, allIndicators.at(c).data.at(i-offset));
            }
            currentSeries->append(allIndicators.at(c).data.size()+offset, maximumValue);
            lineSeriesIndicators.append(currentSeries);
        }

        benchmark bench;
        signalPoints allSignals = bench.getSignals(strategyPtr, &dataframes);

        //These are the buy and sell signals
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
        //fillTradesList(allSignals, &dataframes);
        // does not catch 0 trades!
    }

    chart->addSeries(seriesClose);
    chart->addSeries(seriesOpen);
    chart->addSeries(seriesHighlight);
    chart->createDefaultAxes();
    chart->addSeries(series);
    chart->setAxisX(axisX, series);
    if (strategyPtr != nullptr) {
        for (int c = 0; c < lineSeriesIndicators.size(); c++) {
            if (allIndicators[c].oscillator) {
                oscChart->addSeries(lineSeriesIndicators[c]);
                oscChart->setAxisX(axisX, lineSeriesIndicators[c]);
                chartView = new QChartView(oscChart, ui->oscillator_chart_widget);
                chartView->chart()->setTheme(QChart::ChartThemeBlueCerulean);
            } else {
                chart->addSeries(lineSeriesIndicators[c]);
                chart->setAxisX(axisX, lineSeriesIndicators[c]);
            }
        }
    }
    //chart->legend()->show();
    chart->setAnimationOptions(QChart::AllAnimations);
    chartView = new QChartView(chart, ui->chart_widget);
    chartView->setRubberBand(QChartView::HorizontalRubberBand);
    chartView->chart()->setTheme(QChart::ChartThemeBlueCerulean);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(ui->chart_widget->size());

    //Removes the axis setter points
    seriesHighlight->remove(0,0);
    seriesClose->remove(0,0);
    seriesOpen->remove(0,0);
    series->remove(0, 0);
    seriesClose->remove(dataframes.size(),maximumValue);
    seriesOpen->remove(dataframes.size(),maximumValue);
    seriesHighlight->remove(dataframes.size(),maximumValue);

    if (strategyPtr != nullptr) { //Assumes that "allIndicators" is defined!
        for (int c = 0; c < lineSeriesIndicators.size(); c++) {
            lineSeriesIndicators[c]->remove(0, 0);
            lineSeriesIndicators[c]->remove(allIndicators.at(c).data.size()+allIndicators.at(c).offset, maximumValue);
        }
    }
}

//Goal: FillTradesList(crossings, finance_settings) //finance_settings: Reinvest, capital amount, and other settings, short activated, fees
void chartwindow::fillTradesList(const signalPoints allSignals, const QVector<dataframe> *data)
{
    QLineSeries *series = new QLineSeries();
    QLineSeries *series_index = new QLineSeries();
    QLineSeries *portfolio = new QLineSeries();

    bool shorting = false;
    bool longing = true;
    float capital = 100.0;      //starting capital

    bool open = false;          //keep false
    bool openShort = false;     //keep false
    float openPos = 0.0;        // anchor

    float averageLongOutcome = 0.0;

    portfolio->append(0,0);
    series->append(0, capital);
    series_index->append(0, capital);
    int rowCount = ui->performanceTable->rowCount();

    /*
    //for(int c = 0; c < allSignals.value.size(); c++) {
    //    if(open) {     //If in long -> close pos with result -> open short
    //        open = false;
    //        if(longing) {   //close long
    //            float changePercent = allSignals.value.at(c)/openPos;
    //            capital*=changePercent;
    //            if(changePercent <= 1.0) { //save results from long pos
    //                changePercent = (1-changePercent)*100.0; //wrong calc?
    //                ui->performanceTable->setItem(rowCount-1, 3, new QTableWidgetItem("-"+QString().setNum(changePercent, 'g', 3)));
    //                ui->performanceTable->item(rowCount-1, 3)->setForeground(QBrush(QColor(255, 0, 0)));
    //            } else {
    //                changePercent = (changePercent-1)*100.0;
    //                ui->performanceTable->setItem(rowCount-1, 3, new QTableWidgetItem("+"+QString().setNum(changePercent, 'g', 3)));
    //                ui->performanceTable->item(rowCount-1, 3)->setForeground(QBrush(QColor(0, 255, 0)));
    //            }
    //            ui->performanceTable->setItem(rowCount-1, 2, new QTableWidgetItem(QString::number(allSignals.value.at(c))));
    //            ui->performanceTable->setItem(rowCount-1, 4, new QTableWidgetItem(QString::number(capital)));
    //        }
    //        if(shorting) {  //open short
    //            openPos = allSignals.value.at(c);
    //            rowCount++;
    //            ui->performanceTable->setRowCount(rowCount);
    //            ui->performanceTable->setItem(rowCount-1, 0, new QTableWidgetItem("Short"));
    //            ui->performanceTable->setItem(rowCount-1, 1, new QTableWidgetItem(QString::number(openPos)));
    //            openShort = true;
    //        }
    //
    //        //update chart
    //        series->append(c, capital);
    //        series_index->append(allSignals.indices.at(c), capital);
    //    } else {        //If not in pos -> open pos -> close short
    //        if(shorting) {
    //            if(openShort) {  //close short
    //                float changePercent = openPos/allSignals.value.at(c);
    //                capital*=changePercent;
    //                if(changePercent <= 1.0) { //save results from short pos
    //                    changePercent = (1-changePercent)*100.0;
    //                    ui->performanceTable->setItem(rowCount-1, 3, new QTableWidgetItem("-"+QString().setNum(changePercent, 'g', 3)));
    //                    ui->performanceTable->item(rowCount-1, 3)->setForeground(QBrush(QColor(255, 0, 0)));
    //                } else {
    //                    changePercent = (changePercent-1)*100.0;
    //                    ui->performanceTable->setItem(rowCount-1, 3, new QTableWidgetItem("+"+QString().setNum(changePercent, 'g', 3)));
    //                    ui->performanceTable->item(rowCount-1, 3)->setForeground(QBrush(QColor(0, 255, 0)));
    //                }
    //                ui->performanceTable->setItem(rowCount-1, 2, new QTableWidgetItem(QString::number(allSignals.value.at(c))));
    //                ui->performanceTable->setItem(rowCount-1, 4, new QTableWidgetItem(QString::number(capital)));
    //                openShort = false;
    //            }
    //        }
    //        open = true;
    //        if(longing) {   //open long
    //            openPos = allSignals.value.at(c);
    //            rowCount++;
    //            ui->performanceTable->setRowCount(rowCount);
    //            ui->performanceTable->setItem(rowCount-1, 1, new QTableWidgetItem(QString::number(openPos)));
    //            ui->performanceTable->setItem(rowCount-1, 0, new QTableWidgetItem("Long"));
    //        }
    //        //update chart
    //        portfolio->append(c, capital);
    //        series->append(c, capital);
    //        series_index->append(allSignals.indices.at(c), capital);
    //    }
    //}
    */

    // In order to get the drawdown, drawup etc, I surley need the stockdata.
    unsigned int signalIndex = 0;
    for (int c = 0; c < data->size(); c++) {
        if (open) { //Currently in a position
            float changePercentPrevDay = data->at(c).close_price/data->at(c-1).close_price;
            capital*=changePercentPrevDay;
            portfolio->append(c, capital);
            if (c == allSignals.indices.at(signalIndex)) { //sell pos
                // get the drawdown for this peroid
                int startingIndex = allSignals.indices.at(signalIndex-1);
                int numOfElements = allSignals.indices.at(signalIndex) - startingIndex;
                float mdd = 0;
                float maxDrawUp = 0;
                if (numOfElements > 1) {
                    getDrawDown(data->mid(startingIndex, numOfElements), mdd, maxDrawUp);
                    ui->performanceTable->setItem(rowCount-1, 6, new QTableWidgetItem("+"+QString::number(maxDrawUp)+"%"));
                    ui->performanceTable->item(rowCount-1, 6)->setForeground(QBrush(QColor(0, 255, 0)));

                    ui->performanceTable->setItem(rowCount-1, 5, new QTableWidgetItem("-"+QString::number(mdd)+"%"));
                    ui->performanceTable->item(rowCount-1, 5)->setForeground(QBrush(QColor(255, 0, 0)));
                }

                // results of closing trade
                float changePercent = data->at(c).close_price/openPos;
                series->append(c, capital);
                signalIndex++;
                open = false;
                if(changePercent <= 1.0) { //save results from long pos
                    changePercent = (1-changePercent)*100.0;
                    ui->performanceTable->setItem(rowCount-1, 3, new QTableWidgetItem("-"+QString().setNum(changePercent, 'g', 3)));
                    ui->performanceTable->item(rowCount-1, 3)->setForeground(QBrush(QColor(255, 0, 0)));
                } else {
                    changePercent = (changePercent-1)*100.0;
                    ui->performanceTable->setItem(rowCount-1, 3, new QTableWidgetItem("+"+QString().setNum(changePercent, 'g', 3)));
                    ui->performanceTable->item(rowCount-1, 3)->setForeground(QBrush(QColor(0, 255, 0)));
                }
                ui->performanceTable->setItem(rowCount-1, 2, new QTableWidgetItem(QString::number(data->at(c).close_price)));
                ui->performanceTable->setItem(rowCount-1, 4, new QTableWidgetItem(QString::number(capital)));
            }

        } else {   //Currently on cash
            if (c == allSignals.indices.at(signalIndex)) {                //Buy pos
                signalIndex++;
                openPos = data->at(c).close_price;
                open = true;
                rowCount++;
                ui->performanceTable->setRowCount(rowCount);
                ui->performanceTable->setItem(rowCount-1, 1, new QTableWidgetItem(QString::number(openPos)));
                ui->performanceTable->setItem(rowCount-1, 0, new QTableWidgetItem("Long"));
            }
            portfolio->append(c, capital);
            series->append(c, capital);
            //series_index->append(allSignals.indices.at(c), capital);
        }
    }

    qInfo() << "final capital" <<  QString::number(capital);


    QChart *chart = new QChart();
    chart->addSeries(series);
    //chart->addSeries(series_index);
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

    fillDetails(allSignals, data);

    // sets details calculated in this function
    auto portfolioList = portfolio->points();
    //getDrawDown(portfolioList.mid(1), aa, bb);
}

void chartwindow::getDrawDown(const QVector<dataframe> data, float &maximumDrawDown, float &maximumDrawUp) const
{
    float peak = 0;
    maximumDrawDown = 0;
    maximumDrawUp = 0;
    /* Drawdown
     * Volatilty = Risk
     * Drawdown tells you how much the price went down from the highest point.
     * Question: But not from your entry point right? It can happen that you sell on the lowest price, but still have a
     * higher drawdown. Does it make sense? It is possible because it tells you how much it droped from the peak.
     * When evaluating, remeber looking at the closes, not the highs and lows!
     * */
    int size = data.size();
    int c = 0;
    for (c = 0; c < size; c++) {
        if (peak < data.at(c).close_price) {
            peak = data.at(c).close_price;
        } else {
            float changePercent = data.at(c).close_price/peak;
            if(changePercent <= 1.0) {
                changePercent = (1-changePercent)*100.0;
                if (maximumDrawDown < changePercent) {
                    maximumDrawDown = changePercent;
                }
            }
        }
    }
    float changePercent = peak/data.first().close_price;   //We know we bought at first. But now we wanna know how much we COULD have made
    if(changePercent <= 1.0) {
        changePercent = (1-changePercent)*100.0;
        maximumDrawUp = changePercent;
    } else {
        changePercent = (changePercent-1)*100.0;
        maximumDrawUp = changePercent;
    }
}

void chartwindow::fillDetails(const signalPoints allSignals, const QVector<dataframe> *data)
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

