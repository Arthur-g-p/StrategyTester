#ifndef SUBJECT_H
#define SUBJECT_H

#include "observer.h"
#include <QVector>

class subject
{
public:
    subject();
    virtual void attach(observer *observer) = 0;
    virtual void dettach(observer *observer) = 0;
    virtual void notify(QString message) = 0;
};

#endif // SUBJECT_H
