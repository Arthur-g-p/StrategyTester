#ifndef ADDTAB_H
#define ADDTAB_H

#include "stockdata.h"
#include "settingsmanager.h"
#include "chartwindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QTableWidget>

class addtab : public QWidget
{
    Q_OBJECT
public:
    explicit addtab(QWidget *parent = nullptr);

private:
    QTableWidget *tableWidget;
    QStringList headers;
    void createStandardTable();
    void addHorizontalHeaderLabel(QString headerLabel);

signals:

private slots:
    void cellDoubleClicked(int row, int column);

};

#endif // ADDTAB_H
