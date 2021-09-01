#include "./Headers/mainwindow.h"
#include "./Headers/apicall.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sb = statusBar();
    stockdata *sd = stockdata::getInstance();
    connect(ui->tableWidget, &QTableWidget::cellDoubleClicked, this, &MainWindow::cellDoubleClicked);
    sb->showMessage("downloading...");
}


void MainWindow::startUpReady()
{
    //sb->showMessage("download complete");
}

void MainWindow::fillMainTable()
{
    settingsManager *sm = settingsManager::getInstance();
    QVector<asset> *assets = sm->getAssets(); //The assets get changed during runtime. Do I handle it?

    stockdata *sd = stockdata::getInstance();
    QVector<QVector<dataframe>> *asset_values = sd->getDataframes();

    ui->tableWidget->setColumnCount(3);
    QStringList headers;
    headers <<"Asset Name"<<"Todays price"<<"Percent since yesterday";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    int rowCount = ui->tableWidget->rowCount();
    for(int c = 0; c < assets->size(); c++)
    {
        rowCount++;
        ui->tableWidget->setRowCount(rowCount);
        ui->tableWidget->setItem(c, 2, new QTableWidgetItem("..."));

        if(c < asset_values->size()) {
            ui->tableWidget->setItem(c,1,new QTableWidgetItem(QString::number(asset_values->at(c).at(0).open_price)));
            ui->tableWidget->setItem(c,0,new QTableWidgetItem(assets->at(c).name+"/"+assets->at(c).market));
        } else {
            ui->tableWidget->setItem(c,1,new QTableWidgetItem("downloading..."));
            ui->tableWidget->setItem(c,0,new QTableWidgetItem(assets->at(c).name+"/"+assets->at(c).market));
        }
        if(assets->size() == asset_values->size()) {
            sb->showMessage("download complete");
        }
    }
    //SelectedRow = ui->MYQTableViewWidget->selectedItems().at(row)->data(col).toString();
    //this->setCentralWidget(ui->textField);
    //this->setCentralWidget(ui->graphicsView);
    //stockdata::getInstance();
    //chart->chartView->setParent(ui->horizontalFrame); //call to deleted constructor
}

void MainWindow::on_actionAdd_Asset_triggered()
{
    fillMainTable();
    QMessageBox::warning(this, "File", "triggerd");
    setWindowTitle("ok");
    //Programm crashes when called during download
    newWin = new add_asset();
    newWin->show();
}

void MainWindow::cellDoubleClicked(int row, int column)
{

    qInfo() << "open chart..";
    settingsManager *sm = settingsManager::getInstance();
    QVector<asset> *assets = sm->getAssets();
    stockdata *sd = stockdata::getInstance();
    QVector<QVector<dataframe>> *asset_values = sd->getDataframes();
    if(row < asset_values->size()) {
        chartWin = new chartwindow(nullptr, assets->at(row).name+"/"+assets->at(row).market, row);
        chartWin->show();
    }
    sb->showMessage("not availabe yet...");
}

MainWindow::~MainWindow()
{
    delete ui;
}

//electron
