#include "./Headers/mainwindow.h"
#include "./Headers/apicall.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Strategy Tester");
    status *statusInstance = status::getInstance();         //To initzialize
    sb = statusBar();
    //ui->menubar->installEventFilter(StatusTipFilter())
    statusInstance->setStatusBar(sb);
    //Hovering overrides status bar. Eventfilter needed

    stockdata::getInstance();               //To initzialize
    statusInstance->setStatusText("downloading...");    //Das downloaden ist nur eine Annahme! Man sollte dass nicht printen
    //sb->showMessage("downloading...");

    ui->tabWidget->addTab(new addtab(nullptr), "No Strategy");
}

void MainWindow::on_actionAdd_Asset_triggered()
{
    //Programm crashes when called during download
    newAssetWin = new add_asset();
    newAssetWin->show();
}


void MainWindow::on_tabWidget_currentChanged(int index)
{

}

void MainWindow::on_actionAdd_Strategy_triggered()
{
    newStrategyWin = new addStrategy(ui->tabWidget);
    newStrategyWin->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//electron
