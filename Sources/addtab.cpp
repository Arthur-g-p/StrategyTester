#include "./Headers/addtab.h"

addtab::addtab(strategy *tableStrategyPtr, QWidget *parent)
{
    Q_UNUSED(parent)
    tabtable = new maintable(tableStrategyPtr, parent);

    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->addWidget(tabtable->getTableWidget());
    setLayout(mainlayout);
}
