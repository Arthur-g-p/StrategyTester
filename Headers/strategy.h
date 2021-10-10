#ifndef STRATEGY_H
#define STRATEGY_H

#include "stockdata.h"

struct signalPoints {
    QVector<float> value;
    QVector<int> indices;
    int count;
};

class strategy
{
public:
    strategy(unsigned short number, const QVector<dataframe> *data);
    QVector<float>* getData(QVector<float>* data);
    signalPoints getSignals(const QVector<dataframe> *data);
private:
    enum strategyType { simpleCrossWithChart, CrossWithObject };
    struct strategyInfo {
        strategyType type;
    };
    struct sma {
        QVector<float> data;
        unsigned int number;
        unsigned int offset;
    };

    sma mainSma;
    strategyInfo type;
    sma simpleMovingAverage(const unsigned short number, const QVector<dataframe> *data);
    //QVector<float> exponentialMovingAverage(const unsigned short number, const QVector<dataframe> *data);
};

#endif // STRATEGY_H
