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
    struct sma {
        QVector<float> data;
        unsigned int number;
        unsigned int offset;
        bool readyToUse;
    };

    strategy(QString strategyName, unsigned int number);
    QVector<float>* getData(QVector<float>* data);
    sma &getMainSma(const QVector<dataframe> *data = nullptr, bool override_data = false);
private:
    enum strategyType { simpleCrossWithChart, CrossWithObject };
    struct strategyInfo {
        strategyType type;
    };

    sma mainSma;
    strategyInfo type;
    QString strategyName;
    sma simpleMovingAverage(const QVector<dataframe> *data, unsigned int number = NULL);
    //QVector<float> exponentialMovingAverage(const unsigned short number, const QVector<dataframe> *data);
};

#endif // STRATEGY_H


/* Ideas
 * -Ribbons for consecutive selling
 *
 *
 *
 *
 * */
