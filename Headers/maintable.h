#ifndef MAINTABLE_H
#define MAINTABLE_H

#include "stockdata.h"
#include "settingsmanager.h"
#include "chartwindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QTableWidget>


class maintable : public QWidget
{
    Q_OBJECT
public:
    explicit maintable(QWidget *parent = nullptr);
    ~maintable() {};

protected:
    QTableWidget *tableWidget;
    QStringList headers;
    void addHorizontalHeaderLabel(QString headerLabel);
    void createStandardTable();
protected slots:
    void cellDoubleClicked(int row, int column);
};

#endif // MAINTABLE_H
