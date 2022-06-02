
#include "./Headers/maintable.h"

maintable::maintable(strategy *tableStrategyPtr, QWidget *parent)
{
    Q_UNUSED(parent)
    tableStrategy = tableStrategyPtr;
    tableWidget = new QTableWidget;
    connect(tableWidget, &QTableWidget::cellDoubleClicked, this, &maintable::cellDoubleClicked);
    createStandardTable();
    if (tableStrategy != nullptr) {
        benchmarkAll();
    }
}

/*!
 * \brief maintable::createStandardTable
 */
void maintable::createStandardTable()
{
    //standard settings
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setAlternatingRowColors(true);
    tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setShowGrid(false);

    settingsManager *sm = settingsManager::getInstance();
    QVector<asset> *assets = sm->getAssets(); //The assets get changed during runtime. Do I handle it?

    stockdata *sd = stockdata::getInstance();
    QVector<QVector<dataframe>> *asset_values = sd->getDataframes();    //If it is empty, it does not matter, it gets handled

    addHorizontalHeaderLabel("Asset Name");
    addHorizontalHeaderLabel("Last given price");
    addHorizontalHeaderLabel("%-change from last period");
    addHorizontalHeaderLabel("Periods of data");
    addHorizontalHeaderLabel("Buy and hold");
    addHorizontalHeaderLabel("Strategy final capital");
    addHorizontalHeaderLabel("Strategyperformance");

    auto keyList = sd->getAllKeys();
    QList<QString>::ConstIterator it = keyList.constBegin();

    int rowCount = tableWidget->rowCount();
    for(; it != keyList.constEnd(); ++it) {
        qInfo() <<*it;
        rowCount++;
        tableWidget->setRowCount(rowCount);
        QPair<QVector<dataframe>, bool> asset = sd->getAssetByNamePair(*it);
        if (asset.second == false) { //This means the asset is still downloading
            //request callback
            tableWidget->setItem(rowCount-1, 2, new QTableWidgetItem("downloading..."));
        }

        else if (asset.first.size() <= 15) { //Checks if there is enough data. If there are less than 15 entries, it is definelty not enough
            tableWidget->setItem(rowCount-1, 3, new QTableWidgetItem(QString().setNum(asset.first.size())));
            tableWidget->item(rowCount-1, 3)->setForeground(QBrush(QColor(255, 0, 0)));
            if (asset.first.size() >= 1) {
                //Last given price
                tableWidget->setItem(rowCount-1, 1, new QTableWidgetItem(QString().setNum(asset.first.last().open_price)));
            }
            if (asset.first.size() >= 2) {
                //Price change since last period
                float changePercent = ((asset.first.last().open_price / asset.first.at(asset.first.size()-2).open_price)-1)*100.0;
                if(changePercent <= 1.0) {
                    tableWidget->setItem(rowCount-1, 2, new QTableWidgetItem(QString().setNum(changePercent)+"%"));
                    tableWidget->item(rowCount-1, 2)->setForeground(QBrush(QColor(255, 0, 0)));
                } else {
                    tableWidget->setItem(rowCount-1, 2, new QTableWidgetItem("+"+QString().setNum(changePercent)+"%"));
                    tableWidget->item(rowCount-1, 2)->setForeground(QBrush(QColor(0, 255, 0)));
                }
            }
            tableWidget->setItem(rowCount-1, 3, new QTableWidgetItem("Not enough periods on record!"));
        }

        else {//This means the asset is ready to be used
            //Last given price
            tableWidget->setItem(rowCount-1, 1, new QTableWidgetItem(QString().setNum(asset.first.last().open_price)));

            //Price change since last period
            float changePercent = ((asset.first.last().open_price / asset.first.at(asset.first.size()-2).open_price)-1)*100.0;
            if(changePercent <= 1.0) {
                tableWidget->setItem(rowCount-1, 2, new QTableWidgetItem(QString().setNum(changePercent, 'g', 2)+"%"));
                tableWidget->item(rowCount-1, 2)->setForeground(QBrush(QColor(255, 0, 0)));
            } else {
                tableWidget->setItem(rowCount-1, 2, new QTableWidgetItem("+"+QString().setNum(changePercent, 'g', 2)+"%"));
                tableWidget->item(rowCount-1, 2)->setForeground(QBrush(QColor(0, 255, 0)));
            }

            //BuyNhold Perfomance
            changePercent = benchmark::buyNholdPerformance(&asset.first);
            if(changePercent <= 1.0) {
                tableWidget->setItem(rowCount-1, 4, new QTableWidgetItem(QString().setNum(changePercent)+"%"));
                tableWidget->item(rowCount-1, 4)->setForeground(QBrush(QColor(255, 0, 0)));
            } else {
                tableWidget->setItem(rowCount-1, 4, new QTableWidgetItem("+"+QString().setNum(changePercent)+"%"));
                tableWidget->item(rowCount-1, 4)->setForeground(QBrush(QColor(0, 255, 0)));
            }

            //Num of Periods
            tableWidget->setItem(rowCount-1, 3, new QTableWidgetItem(QString().setNum(asset.first.size())));
        }
        //Asset Name: for both, downloaded and undownloaded assets
        tableWidget->setItem(rowCount-1, 0, new QTableWidgetItem(*it));
    }

/*
    for(int c = 0; c < assets->size(); c++) //This is not efficient to get called in a loop in
    {
        //Check if data available missing!
        rowCount++;
        tableWidget->setRowCount(rowCount);
        tableWidget->setItem(c,0,new QTableWidgetItem(assets->at(c).name+"/"+assets->at(c).market));
        if(c < asset_values->size()) {
            QVector<dataframe> dataframes = asset_values->at(c);          //To not to play with the pointer this much and to be able to reverse it
            std::reverse(dataframes.begin(), dataframes.end());             // Never forget to revers, maybe put in stockdata
            int df_index = dataframes.size()-1; //demands more than 1 period!!!
            //Price change since last period
            float changePercent = ((dataframes.at(df_index).open_price / dataframes.at(df_index-1).open_price)-1)*100.0;
            if(changePercent <= 1.0) {
                tableWidget->setItem(c, 2, new QTableWidgetItem(QString().setNum(changePercent, 'g', 2)+"%"));
                tableWidget->item(c, 2)->setForeground(QBrush(QColor(255, 0, 0)));
            } else {
                tableWidget->setItem(c, 2, new QTableWidgetItem("+"+QString().setNum(changePercent, 'g', 2)+"%"));
                tableWidget->item(c, 2)->setForeground(QBrush(QColor(0, 255, 0)));
            }

            //Current price
            tableWidget->setItem(c,1,new QTableWidgetItem(QString::number(dataframes.at(df_index).open_price)));

            //Buy and Hold
            changePercent = benchmark::buyNholdPerformance(&dataframes);
            if(changePercent <= 1.0) {
                tableWidget->setItem(c, 4, new QTableWidgetItem(QString().setNum(changePercent)+"%"));
                tableWidget->item(c, 4)->setForeground(QBrush(QColor(255, 0, 0)));
            } else {
                tableWidget->setItem(c, 4, new QTableWidgetItem("+"+QString().setNum(changePercent)+"%"));
                tableWidget->item(c, 4)->setForeground(QBrush(QColor(0, 255, 0)));
            }

            //Num of Periods
            tableWidget->setItem(c, 3, new QTableWidgetItem(QString().setNum(dataframes.size())));
        } else {    //Data is not downloaded yet
            tableWidget->setItem(c,1,new QTableWidgetItem("downloading..."));
            tableWidget->setItem(c,2,new QTableWidgetItem("downloading..."));
            tableWidget->setItem(c,3,new QTableWidgetItem("downloading..."));
        }

        if(assets->size() == asset_values->size()) {
            qInfo("this table is complete.");
        }
    }*/
}

void maintable::benchmarkAll()
{
    stockdata *sd = stockdata::getInstance();

    auto keyList = sd->getAllKeys();
    QList<QString>::ConstIterator it = keyList.constBegin();

    int rowCount = tableWidget->rowCount();
    for(rowCount = 0; it != keyList.constEnd(); ++it) {
        qInfo() <<*it;
        QPair<QVector<dataframe>, bool> asset = sd->getAssetByNamePair(*it);
        if (asset.second) { //This means the asset is availabe
            std::string zero = std::string("...");// + std::to_string(c);
            tableWidget->setItem(rowCount, 5, new QTableWidgetItem(&zero[0]));
            QTableWidgetItem *currentItem = tableWidget->item(rowCount, 5);

            //calculating the performance
            tableStrategy->getIndicators(&asset.first, true);   //Inits the indicators because of the true
            benchmark bench;
            signalPoints allSignals = bench.getSignals(tableStrategy, &asset.first);
            float capital = bench.getPerformance(allSignals);
            currentItem->setText(QString::number(capital));

            //Strategyperformance
            float changePercent = ((capital/100.0)-1)*100.0;    //This 100.0 may change, its just the starting capital
            if(changePercent <= 1.0) {
                tableWidget->setItem(rowCount, 6, new QTableWidgetItem(QString().setNum(changePercent)+"%"));
                tableWidget->item(rowCount, 6)->setForeground(QBrush(QColor(255, 0, 0)));
            } else {
                tableWidget->setItem(rowCount, 6, new QTableWidgetItem("+"+QString().setNum(changePercent)+"%"));
                tableWidget->item(rowCount, 6)->setForeground(QBrush(QColor(0, 255, 0)));
            }
        }
        rowCount++;
    }
    /*
    for(int c = 0; c < assets->size(); c++)
    {
        std::string zero = std::string("...");// + std::to_string(c);
        tableWidget->setItem(c, 5, new QTableWidgetItem(&zero[0]));
        QTableWidgetItem *currentItem = tableWidget->item(c, 5);
        if (c < asset_values->size()) {
            QVector<dataframe> dataframes = asset_values->at(c);          //To not to play with the pointer this much and to be able to reverse it
            std::reverse(dataframes.begin(), dataframes.end());             // Never forget to revers, maybe put in stockdata
            tableStrategy->getMainSma(&dataframes, true);
            benchmark bench;
            signalPoints allSignals = bench.getSignals(*tableStrategy, &dataframes);
            float capital = bench.getPerformance(allSignals);
            currentItem->setText(QString::number(capital));

            //Strategyperformance
            float changePercent = ((capital/100.0)-1)*100.0;    //This 100.0 may change, its just the starting capital
            if(changePercent <= 1.0) {
                tableWidget->setItem(c, 6, new QTableWidgetItem(QString().setNum(changePercent, 'g', 2)+"%"));
                tableWidget->item(c, 6)->setForeground(QBrush(QColor(255, 0, 0)));
            } else {
                tableWidget->setItem(c, 6, new QTableWidgetItem("+"+QString().setNum(changePercent, 'g', 2)+"%"));
                tableWidget->item(c, 6)->setForeground(QBrush(QColor(0, 255, 0)));
            }
        }
        if(assets->size() == asset_values->size()) {
            qInfo("this table is complete.");
        }
    }*/
}

QTableWidget *maintable::getTableWidget()
{
    return tableWidget;
}

void maintable::cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    qInfo() << "open chart..";
    chartwindow *chartWin;
    stockdata *sd = stockdata::getInstance();

    QString assetName = tableWidget->item(row,0)->text();       //Gets the asset name
    QPair<QVector<dataframe>, bool> asset = sd->getAssetByNamePair(assetName);
    if (asset.second) { //This means the asset is availabe
        chartWin = new chartwindow(tableStrategy, assetName);
        chartWin->show();
    } else {
        qInfo() << "the data is not available yet";
    }

    //sb->showMessage("not availabe yet...");
}

void maintable::addHorizontalHeaderLabel(QString headerLabel)      //why is not there such a function in qt?
{
    if(headerLabel.isEmpty()) {
        return;
    }
    headers.append(headerLabel);
    tableWidget->setColumnCount(headers.size());
    tableWidget->setHorizontalHeaderLabels(headers);
}
