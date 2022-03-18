#ifndef addstrategy_h
#define addstrategy_h

#include <QWidget>
#include <QTabWidget>
#include "strategy.h"

namespace Ui {
class addStrategy;
}

class addStrategy : public QWidget
{
    Q_OBJECT

public:
    explicit addStrategy(QTabWidget *tabWidget, QWidget *parent = nullptr);
    ~addStrategy();

private:
    Ui::addStrategy *ui;
    QTabWidget *tabWidget;
    //strategy resultingStrategy;

    strategy* createStrategyFromDialog();
    
private slots:
    void on_Cancel_button_clicked();
    void on_Ok_button_clicked();
};

#endif // ADDSTRATEGY_H
