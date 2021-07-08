#include "mainwindow.h"
#include "ui_mainwindow.h"

//settingsManager* settingsManager::instance = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sb = statusBar();
    sb->showMessage("downloading...");
}


void MainWindow::startUpReady()
{
    //sb->showMessage("download complete");
}

void MainWindow::fillMainTable()
{
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(2);
    QStringList headers;
    headers <<"Asset Name"<<"Todays price"<<"Percent since yesterday";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    settingsManager* sd = settingsManager::getInstance();
    //QVector<dataframe> *a = sd->getDataframes();
    /*
    for(int c = 0; c < a->size(); c++)
    {
        QString tmp = a->at(c).time;
    }*/
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText("First Entry");
    ui->tableWidget->setItem(0,0,new QTableWidgetItem("BR"));
    ui->tableWidget->setItem(0,1,new QTableWidgetItem("100"));
    ui->tableWidget->setItem(0,2,item);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //Themes einstellen.
        //ui->tableWidget->setItem(1,1,item);
        //    ui->tableWidget->setItem(1,2,item);
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
    ui->horizontalFrame->setFrameRect(QRect(0,0,500,500));
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
