#include "./Headers/benchmark.h"

benchmark::benchmark()
{

}

// only working for ma crossing with the chart yet
signalPoints benchmark::getSignals(strategy *strat, const QVector<dataframe> *data)
{
    signalPoints signalPoints;

    signalPoints = strat->getSignals(data);
    return signalPoints;
}

/**
 * @brief buyNholdPerformance gets the percentage change you would have if you bought at the first entry and hold until now
 */
float benchmark::buyNholdPerformance(const QVector<dataframe> *data)
{
    //2 Options:
    //First buy and last price
    //  Last price is the last given price
    //First price and last price
    int df_index = data->size()-1; //demands more than 1 period!!!
    return ((data->at(df_index).open_price / data->at(0).open_price)-1)*100.0; //-1 correct?
}

float benchmark::getPerformance(signalPoints sngls)//+, moneyManagement
{
    //money management, a lot more options to come
    bool shorting = false;
    bool longing = true;
    float capital = 100.0;

    bool openLong = false;
    bool openShort = false;
    float openPos = 0.0;
    for(int c = 0; c < sngls.value.size(); c++) {
        if(openLong) {     //If in long -> close pos with result -> open short
            openLong = false;
            if(longing) {   //close long
                float changePercent = sngls.value.at(c)/openPos;
                capital*=changePercent;
            }
            if(shorting) {  //open short
                openPos = sngls.value.at(c);
                openShort = true;
            }
        } else {        //If not in pos -> open pos -> close short
            if(shorting) {
                if(openShort) {  //close short
                    float changePercent = openPos/sngls.value.at(c);
                    capital*=changePercent;
                    openShort = false;
                }
            }
            openLong = true;
            if(longing) {   //open long
                openPos = sngls.value.at(c);
            }
        }
    }
    // Alternative Berechnung:
    // prozent unterschiede zwischen einzelnen Stationen kumulieren
    return capital;
    //fillDetails(allSignals);
}
