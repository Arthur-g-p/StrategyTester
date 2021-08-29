#ifndef OBSERVER_H
#define OBSERVER_H

#include <QString>

class observer
{
public:
    observer();
    virtual void update(QString message) = 0;
};

#endif // OBSERVER_H
