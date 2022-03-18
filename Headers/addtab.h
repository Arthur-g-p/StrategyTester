#ifndef ADDTAB_H
#define ADDTAB_H

#include "Headers/maintable.h"
#include "Headers/strategy.h"

class addtab : public QWidget
{
public:
    explicit addtab(strategy *tableStrategyPtr, QWidget *parent = nullptr);
    ~addtab() {};
private:
    maintable *tabtable;
};

#endif // ADDTAB_H
