#include "./Headers/addtab.h"

addtab::addtab(QWidget *parent)
{
    //maintable();
    tableWidget = new QTableWidget;
    connect(tableWidget, &QTableWidget::cellDoubleClicked, this, &addtab::cellDoubleClicked);
    createStandardTable();
    //additonal:
    addHorizontalHeaderLabel("drawdown");
    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->addWidget(tableWidget);
    setLayout(mainlayout);
}
