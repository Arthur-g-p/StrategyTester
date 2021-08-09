#ifndef SUBJECT_H
#define SUBJECT_H

#include "observer.h"
#include <QVector>

class subject
{
public:
    subject();
    virtual void attach(observer *observer);
    virtual void dettach(observer *observer);
    virtual void notify();
};

#endif // SUBJECT_H
