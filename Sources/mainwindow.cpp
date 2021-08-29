#include "./Headers/mainwindow.h"
#include "./Headers/apicall.h"

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
    QVector<asset> *assets = sm->getAssets(); //The assets get changed during runtime. Do I handle it?

    stockdata *sd = stockdata::getInstance();
    QVector<QVector<dataframe>> *asset_values = sd->getDataframes();

    apicall testCall("demo", "BTC", "USD", "tbd");

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
        ui->tableWidget->setItem(c,2,new QTableWidgetItem("BR"));
        ui->tableWidget->setItem(c,1,new QTableWidgetItem("to find out")); //sd->getDataframes()->at(c).at(0).time));
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
    QMessageBox::warning(this, "File", "triggerd");
    setWindowTitle("ok");

    newWin = new add_asset();
    newWin->show();
}


MainWindow::~MainWindow()
{
    delete ui;
}

//electron
