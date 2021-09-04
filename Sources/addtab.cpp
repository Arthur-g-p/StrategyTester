#include "./Headers/addtab.h"

addtab::addtab(QWidget *parent) : QWidget(parent)
{
    tableWidget = new QTableWidget;
    connect(tableWidget, &QTableWidget::cellDoubleClicked, this, &addtab::cellDoubleClicked);
    //I think I will have to create a class later on that inits the "mainTable" every time for me.
    //It will return a pointer and I can add my stuff there. But the controll will be kept at the mainTable class
    createStandardTable();
    //additonal:
    addHorizontalHeaderLabel("drawdown");
    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->addWidget(tableWidget);
    setLayout(mainlayout);
}

void addtab::createStandardTable()
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
    QVector<QVector<dataframe>> *asset_values = sd->getDataframes();

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

void addtab::addHorizontalHeaderLabel(QString headerLabel)      //why is not there such a function in qt?
{
    if(headerLabel.isEmpty()) {
        return;
    }
    headers.append(headerLabel);
    tableWidget->setColumnCount(headers.size());
    tableWidget->setHorizontalHeaderLabels(headers);
}

void addtab::cellDoubleClicked(int row, int column)
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
