#include "./Headers/strategy.h"

strategy::strategy(QString strategyName)
{
    this->strategyName = strategyName;
}

QVector<indicator> strategy::getIndicators(const QVector<dataframe> *data, bool override_data)     //This hast to move up in hierarchy
{
    //override_data needs to be true when benchmarking another dataset
    if (indicatorsInitzialized || override_data == true) {
        if (data) {
            for (int c = 0; c < allIndicators.size(); c++) {
                int parameter1 = allIndicators.at(c).parameter;
                if (allIndicators.at(c).name == "Simple Moving Average") {
                    allIndicators[c].offset = parameter1-1;
                    allIndicators[c].data = simpleMovingAverage(data, parameter1);
                }
                else if (allIndicators.at(c).name == "Exponential Moving Average") {
                    allIndicators[c].offset = parameter1-1;
                   //allIndicators.at(c).data = exponentialMovingAverage(data);
                } else if (allIndicators.at(c).name == "Relative Strength Index") {
                    allIndicators[c].offset = parameter1-1;
                    allIndicators[c].oscillator = true;
                    allIndicators[c].data = relativeStrengthIndex(data, parameter1);
                } else {
                    qDebug() << "Unexpected input";
                    Q_ASSERT(true);
                }
            }
            indicatorsInitzialized = true;
        }
    }
    return allIndicators;
}

QVector<float> strategy::simpleMovingAverage(const QVector<dataframe> *data, unsigned int number)
{
    int offset = number-1;
    QVector<float> history;
    float sum;
    for(int c = offset; c < data->size(); c++) {
        sum = 0.0f;
        for(int i = c-offset; i <= c; i++) {            //Sum from data[c-20] to data[c]
            sum += data->at(i).close_price;
        }
        float average = sum/number;
        history.append(average);
    }
    return history;
}

/*
signalPoints strategy::oscillatorRange(const QVector<dataframe> *data)
{
    if (!indicatorsInitzialized) {
        Q_ASSERT(true);
        getIndicators(data, true);
    }

    signalPoints signalPoints;
    indicator firstIndicator = allIndicators.at(0);
    bool belowLowerTreshold;
    bool aboveUpperTreshold;

    for(int c = firstIndicator.offset; c < data->size()+firstIndicator.offset; c++) {
        if(firstIndicator.data.at(c) <= 20.0f && belowLowerTreshold == false) {
            belowLowerTreshold = true;
            aboveUpperTreshold = false;
            signalPoints.value.append(data->at(c).close_price);
            signalPoints.indices.append(c);
        }
        else if(firstIndicator.data.at(c) >= 80.0f) {
            belowLowerTreshold = false;
            aboveUpperTreshold = true;
            signalPoints.value.append(data->at(c).close_price);
            signalPoints.indices.append(c);
        }
        else {
            //In the "boring-zone"
            belowLowerTreshold = false;
            aboveUpperTreshold = false;
        }
    }
}*/

QVector<float> strategy::relativeStrengthIndex(const QVector<dataframe> *data, const unsigned int periods)
{
    //At least 14 periods!

    //Difference from day before
    QVector<float> upwardMovment;
    QVector<float> downwardMovment;

    float lastAvgUpwardMovment;
    float currentAvgUpwardMovment;
    float lastAvgDownwardMovment;
    float currentAvgDownwardMovment;

    QVector<float> avgUpwardMovment;
    QVector<float> avgDownwardMovment;
    QVector<float> rsi;

    float change = 0;
    for(unsigned int c = 1; c < data->size(); c++) {
        change = data->at(c).close_price - data->at(c-1).close_price;
        if (change < 0) {
            // this is a loss
            downwardMovment.append(change*-1);
            upwardMovment.append(0);
        } else {
            upwardMovment.append(change);
            downwardMovment.append(0);
        }
        if (c == periods) {
            //First Average Gain = Sum of Gains over the past 14 periods / 14.
            currentAvgUpwardMovment = averageOfVector(upwardMovment.mid(0, periods));
            avgUpwardMovment.append(currentAvgUpwardMovment);

            //First Average Loss = Sum of Losses over the past 14 periods / 14
            currentAvgDownwardMovment = averageOfVector(downwardMovment.mid(0, periods));
            avgDownwardMovment.append(currentAvgDownwardMovment);
        }
        else if (c > periods) {
            //Average Gain = [(previous Average Gain) x 13 + current Gain] / 14.
            currentAvgUpwardMovment = (lastAvgUpwardMovment*(periods-1)+upwardMovment.back())/periods;
            avgUpwardMovment.append(currentAvgUpwardMovment);

            //Average Loss = [(previous Average Loss) x 13 + current Loss] / 14.
            currentAvgDownwardMovment = (lastAvgDownwardMovment*(periods-1)+downwardMovment.back())/periods;
            avgDownwardMovment.append(currentAvgDownwardMovment);
        }

        if (c >= periods) {
            // RSI = 100 - (100/(1 + Average Gain / Average Loss)
            rsi.append(100-(100/(1+avgUpwardMovment.back()/avgDownwardMovment.back())));
        }
        lastAvgUpwardMovment = currentAvgUpwardMovment;
        lastAvgDownwardMovment = currentAvgDownwardMovment;
    }
    return rsi;
}

float strategy::averageOfVector(const QVector<float> vec)
{
    float sum = 0.0;
    for (int i = 0; i < vec.size(); i++) {
        sum += vec.at(i);
    }
    return sum/vec.size();
}
