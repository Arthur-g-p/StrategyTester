#ifndef STRATEGY_H
#define STRATEGY_H

#include "stockdata.h"


struct indicator {
    QString name;
    QVector<float> data;
    bool oscillator = false;
    int parameter;
    int parameter2;
    int parameter3;
    unsigned int offset;
};

struct signalPoints {
    QVector<float> value;
    QVector<int> indices;
    int count;
    QString meaning;    //Optional, maybe future use
};

class strategy
{
public:
    strategy(QString strategyName);

    virtual signalPoints getSignals(const QVector<dataframe> *data) = 0;
    QVector<indicator> getIndicators(const QVector<dataframe> *data = nullptr, bool override_data = false);

    //sma &getMainSma(const QVector<dataframe> *data = nullptr, bool override_data = false);
private:
    /*
    enum strategyType { simpleCrossWithChart, CrossWithObject };
    struct strategyInfo {
        strategyType type;
    };

    strategyInfo type;*/
    QString strategyName;
protected:
    QVector<float> simpleMovingAverage(const QVector<dataframe> *data, unsigned int number);
    //QVector<float> exponentialMovingAverage(const unsigned short number, const QVector<dataframe> *data);
    QVector<float> relativeStrengthIndex(const QVector<dataframe> *data, const unsigned int periods = 14);
    float averageOfVector(const QVector<float> vec);

    QVector<indicator> allIndicators;
    bool indicatorsInitzialized;
};

#endif // STRATEGY_H


/* Ideas
 * -Ribbons for consecutive selling
 *
 *
 *
 *
 * */
