#include "./Headers/strategy.h"

strategy::strategy(unsigned short number, const QVector<dataframe> *data)
{
    mainSma = simpleMovingAverage(number, data);
}

signalPoints strategy::getSignals(const QVector<dataframe> *data)
{
    signalPoints signalPoints;
    //Simple Chart Cross:
    bool maLower;
    for(int c = mainSma.offset; c < mainSma.data.size()+mainSma.offset; c++) {            //C is the global index
        if(mainSma.data.at(c-mainSma.offset) < data->at(c).close_price) {
            if(!maLower) {
                maLower = true;
                if(c+1 < data->size()) {
                    signalPoints.value.append(data->at(c+1).open_price);
                    signalPoints.indices.append(c);
                }
            }
        } else {
            if(maLower) {
                maLower = false;
                if(c+1 < data->size()) {
                    signalPoints.value.append(data->at(c+1).open_price);
                    signalPoints.indices.append(c);
                }
            }
        }
    }
    return signalPoints;
}

strategy::sma strategy::simpleMovingAverage(unsigned short number, const QVector<dataframe> *data)
{
    sma movingAverage;
    movingAverage.number = number;
    movingAverage.offset = number-1;
    float sum;
    for(int c = movingAverage.offset; c < data->size(); c++) {
        sum = 0.0f;
        for(int i = c-movingAverage.offset; i <= c; i++) {            //Sum from data[c-20] to data[c]
            sum += data->at(i).close_price;
        }
        float average = sum/(movingAverage.number);
        movingAverage.data.append(average);
    }
    return movingAverage;
}

