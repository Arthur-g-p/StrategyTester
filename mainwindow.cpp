#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sb = statusBar();
    stockdata *sd = stockdata::getInstance();
    sb->showMessage("downloading...");
}


void MainWindow::startUpReady()
{
    //sb->showMessage("download complete");
}

void MainWindow::fillMainTable()
{
    settingsManager *sm = settingsManager::getInstance();
    QVector<asset> *assets = sm->getAssets();

    stockdata *sd = stockdata::getInstance();
    //QVector<dataframe> *asset_values = sd->getDataframes();
    sd->getDataframes();
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(2);
    QStringList headers;
    headers <<"Asset Name"<<"Todays price"<<"Percent since yesterday";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    int rowCount = ui->tableWidget->rowCount();
    for(int c = 0; c < assets->size(); c++)
    {
        rowCount++;
        ui->tableWidget->setRowCount(rowCount);
        //QTableWidgetItem *assetName = new QTableWidgetItem;
        //assetName->setText(assets->at(c).name+"/"+assets->at(c).market);
        ui->tableWidget->setItem(c,2,new QTableWidgetItem("BR"));
        ui->tableWidget->setItem(c,1,new QTableWidgetItem("100"));
        ui->tableWidget->setItem(c,0,new QTableWidgetItem(assets->at(c).name+"/"+assets->at(c).market));
    }
    //this->setCentralWidget(ui->textField);
    //this->setCentralWidget(ui->graphicsView);
    //stockdata::getInstance();
    //chart->chartView->setParent(ui->horizontalFrame); //call to deleted constructor
}

void MainWindow::on_actionAdd_Asset_triggered()
{
    fillMainTable();
    //chart = new charts("BTC");
    QMessageBox::warning(this, "File", "triggerd");
    //ui->textField->insertPlainText("fuck");
    setWindowTitle("ok");
    //this->setCentralWidget(ui->graphicsView);
    //QWidget::show(ui->graphicsView);
    //data->request("BTC");

    newWin = new add_asset();
    newWin->show();
}


MainWindow::~MainWindow()
{
    delete ui;
}

//electron
