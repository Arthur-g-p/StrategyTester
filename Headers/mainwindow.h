#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stockdata.h"
#include "settingsmanager.h"
#include "add_asset.h"
#include "ui_mainwindow.h"
#include "chartwindow.h"
#include "addtab.h"
#include "status.h"
#include "addstrategy.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    static void startUpReady();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    add_asset *newAssetWin;
    addStrategy *newStrategyWin;
    chartwindow *chartWin;
    QStatusBar *sb;
    //instances of stockdata has do be saved on the heap otherwise the slots get deleted when they lose the scope

private slots:
    void on_actionAdd_Asset_triggered();
    void on_tabWidget_currentChanged(int index);
    void on_actionAdd_Strategy_triggered();
};
#endif // MAINWINDOW_H
