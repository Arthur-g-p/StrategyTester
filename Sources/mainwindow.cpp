#include "./Headers/mainwindow.h"
#include "./Headers/apicall.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Strategy Tester");
    sb = statusBar();
    stockdata::getInstance();       //To initzialize
    sb->showMessage("downloading...");
}

void MainWindow::on_actionAdd_Asset_triggered()
{
    //Programm crashes when called during download
    newWin = new add_asset();
    newWin->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//electron

void MainWindow::on_tabWidget_currentChanged(int index)
{

}

void MainWindow::on_actionAdd_Strategy_triggered()
{
    ui->tabWidget->addTab(new addtab(), "new tab");
}

