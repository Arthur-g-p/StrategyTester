#include "./Headers/strategy.h"

strategy::strategy(QString strategyName, unsigned int number)
{
    this->strategyName = strategyName;
    if (number <= 0) {
        number = 20;
    }
    mainSma.readyToUse = false;
    mainSma.number = number;
}

strategy::sma &strategy::getMainSma(const QVector<dataframe> *data, bool override_data)
{
    //override_data needs to be true when benchmarking another dataset
    if (!mainSma.readyToUse || override_data == true) {
        if (data) {
            mainSma = simpleMovingAverage(data);
        }
    }
    return mainSma;
}


strategy::sma strategy::simpleMovingAverage(const QVector<dataframe> *data, unsigned int number)
{
    if (number == NULL) { //This means calculate with the current number given
        number = mainSma.number;      //es fehlt eine Abfage ob die nummer nicht zu groß ist
    }
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
    movingAverage.readyToUse = true;
    return movingAverage;
}

