#include "./Headers/strategycrossing.h"


strategyCrossing::strategyCrossing(QString strategyName,
                                   unsigned int number, QString name,
                                   unsigned int number2, QString name2)
    : strategy(strategyName)
{
    indicator primaryIndicator;
    primaryIndicator.name = name;
    primaryIndicator.parameter = number;
    primaryIndicator.offset = 0;            //The offset later gets determined
    allIndicators.append(primaryIndicator);

    if (name2 != "Chart") {
        indicator secondaryIndicator;
        secondaryIndicator.parameter = number2;
        secondaryIndicator.name = name2;
        secondaryIndicator.offset = 0;
        allIndicators.append(secondaryIndicator);
    } //If the "crossingpartner" is the chart, dont add a second indicator

    //allIndicators[0].name = "Does it work?"; //Why does this not work if allIndicators is a pointer?

    indicatorsInitzialized = false;
}

QVector<indicator> strategyCrossing::getIndicators(const QVector<dataframe> *data, bool override_data)     //This hast to move up in hierarchy
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

//In a crossing there are ONLY TWO objects that cross.
signalPoints strategyCrossing::getSignals(const QVector<dataframe> *data)
{
    if (!indicatorsInitzialized) {
        Q_ASSERT(true);
        getIndicators(data, true);
    }

    signalPoints signalPoints;
    bool maAbove;

    indicator firstIndicator = allIndicators.at(0);
    indicator secondIndicator;

    if (allIndicators.size() == 1) {
        //Indicator crosses with chart
        //to not make an extra fuction the chart data is converted to an indicator
        secondIndicator.offset = 0;
        QVector<float> onlyOnePrice;
        for(int i = 0; i < data->size(); i++) {
            onlyOnePrice.append(data->at(i).close_price);
        }
        secondIndicator.data = onlyOnePrice;
    } else {
        //Crosses with another indicator
        secondIndicator = allIndicators.at(1);
    }
    int highestOffset = firstIndicator.offset > secondIndicator.offset ? firstIndicator.offset : secondIndicator.offset;
    for(int c = highestOffset; c < firstIndicator.data.size()+firstIndicator.offset; c++) {     // firstIndicator.data.size()+firstIndicator.offset correct?
        if(firstIndicator.data.at(c-firstIndicator.offset) > secondIndicator.data.at(c-secondIndicator.offset)) {
            if(!maAbove) {
                maAbove = true;
                if(c < data->size()) {
                    signalPoints.value.append(data->at(c).close_price);
                    signalPoints.indices.append(c);
                }
            }
        } else {
            if(maAbove) {
                maAbove = false;
                if(c < data->size()) {
                    signalPoints.value.append(data->at(c).close_price);
                    signalPoints.indices.append(c);
                }
            }
        }
    }
    return signalPoints;
}
