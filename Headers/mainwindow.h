#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stockdata.h"
#include "settingsmanager.h"
#include "add_asset.h"
#include "ui_mainwindow.h"
#include "chartwindow.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QTableView>

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
    add_asset *newWin;
    chartwindow *chartWin;
    QStatusBar *sb;
    void initTable();
    void fillMainTable();
    //instancees of stockdata has do be saved on the heap otherwise the slots get deleted when they lose the scope


private slots:
    void on_actionAdd_Asset_triggered();
    void cellDoubleClicked(int row, int column);
};
#endif // MAINWINDOW_H
