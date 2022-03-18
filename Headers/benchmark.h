#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "Headers/strategy.h"
/**
 * @brief The benchmark class
 * This class takes as input the money management, the strategy and the data and returns the perfomance
 */

class benchmark
{
public:
    benchmark();
    signalPoints getSignals(strategy strat, const QVector<dataframe> *data);
    float getPerformance(signalPoints sngls);
    static float buyNholdPerformance(const QVector<dataframe> *data);
};

#endif // BENCHMARK_H
