#ifndef MAINTABLE_H
#define MAINTABLE_H

#include "stockdata.h"
#include "settingsmanager.h"
#include "chartwindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <string>
#include <QTableWidget>


class maintable : public QWidget
{
    Q_OBJECT
public:
    maintable(strategy *tableStrategyPtr, QWidget *parent = nullptr);  //Default parameters have to come at the last place
    void benchmarkAll();
    QTableWidget* getTableWidget();
    ~maintable() {};
private:
    strategy *tableStrategy;
    QTableWidget *tableWidget;
    QStringList headers;
    void addHorizontalHeaderLabel(QString headerLabel);
    void createStandardTable();

private slots:
    void cellDoubleClicked(int row, int column);
};

#endif // MAINTABLE_H
