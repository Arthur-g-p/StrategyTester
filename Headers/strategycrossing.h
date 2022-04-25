#ifndef STRATEGYCROSSING_H
#define STRATEGYCROSSING_H

#include "strategy.h"

/*
class indicato
{
public:
    indicato();
    QString name;
    QVector<float> data;
    int parameter;
    int parameter2;
    int parameter3;
    unsigned int offset;
};*/

class movingAverage : public indicator
{

};

class strategyCrossing : public strategy
{
public:
    strategyCrossing(QString strategyName, unsigned int number, QString name, unsigned int number2 = 0, QString name2 = "Chart");
    QVector<indicator> getIndicators(const QVector<dataframe> *data = nullptr, bool override_data = false);
    signalPoints getSignals(const QVector<dataframe> *data);
};

#endif // STRATEGYCROSSING_H
