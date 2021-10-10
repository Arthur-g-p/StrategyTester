#include "./Headers/maintable.h"

maintable::maintable(QWidget *parent)
{

}


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
    addHorizontalHeaderLabel("Todays Price");
    addHorizontalHeaderLabel("Percent since last period");

    int rowCount = tableWidget->rowCount();
    for(int c = 0; c < assets->size(); c++) //This is not efficient to get called in a loop
    {
        rowCount++;
        tableWidget->setRowCount(rowCount);
        tableWidget->setItem(c, 2, new QTableWidgetItem("..."));

        if(c < asset_values->size()) {
            tableWidget->setItem(c,1,new QTableWidgetItem(QString::number(asset_values->at(c).at(0).open_price)));
            tableWidget->setItem(c,0,new QTableWidgetItem(assets->at(c).name+"/"+assets->at(c).market));
        } else {
            tableWidget->setItem(c,1,new QTableWidgetItem("downloading..."));
            tableWidget->setItem(c,0,new QTableWidgetItem(assets->at(c).name+"/"+assets->at(c).market));
        }
        if(assets->size() == asset_values->size()) {
            qInfo("this table is complete.");
        }
    }
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

void maintable::cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    qInfo() << "open chart..";
    chartwindow *chartWin;
    stockdata *sd = stockdata::getInstance();
    QVector<QVector<dataframe>> *asset_values = sd->getDataframes();
    if(row < asset_values->size()) {
        settingsManager *sm = settingsManager::getInstance();
        QVector<asset> *assets = sm->getAssets();
        chartWin = new chartwindow(nullptr, assets->at(row).name+"/"+assets->at(row).market, row);
        chartWin->show();
        return;
    }
    qInfo() << "the data is not available yet";
    //sb->showMessage("not availabe yet...");
}
