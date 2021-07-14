#include "stockdata.h"

stockdata* stockdata::instance = NULL;

stockdata::stockdata()
{
    connect(qnam, &QNetworkAccessManager::finished, this, &stockdata::readyread);
    api_key = settingsManager::getInstance()->getApiKey();
    dataframes = new QVector<dataframe>;
    requestAsset("BTC");
}

stockdata *stockdata::getInstance()
{
    if(instance==NULL) {
        instance = new stockdata ();
    }
    return instance;
}

QVector<dataframe> *stockdata::getDataframes()
{
    return dataframes;
}

void stockdata::requestAsset(QString symbol, api_function function)
{
    QString url = base_api;
    if(function == DIGITAL_CURRENCY_DAILY) {
        url.append("?function=DIGITAL_CURRENCY_DAILY");
    }
    else if (function == DIGITAL_CURRENCY_WEEKLY) {
        url.append("?function=DIGITAL_CURRENCY_WEEKLY");
    }
    url.append("&symbol="+symbol);
    url.append("&market=USD");
    url.append("&apikey="+api_key);
    url.append("&datatype=csv");

    qnam->get(QNetworkRequest(QUrl(url)));
}

void stockdata::readyread(QNetworkReply *reply)
{
    QString output = reply->readAll();

    int index = 0;
    QVector <dataframe> btc;
    index = output.indexOf("\n", 0);
    output = output.mid(index+1);
    index = 0;
    int times_loaded = 0;
    dataframe new_vec;
    for(;;) {
        index = output.indexOf(",", 0);
        if (index != -1) {
            switch (times_loaded) {
                case 0: new_vec.time = output.left(index); break;
                case 1: new_vec.open_price = output.left(index).toFloat(); break;
                case 2: new_vec.high_price = output.left(index).toFloat(); break;
                case 3: new_vec.low_price = output.left(index).toFloat(); break;
                case 4: new_vec.close_price = output.left(index).toFloat(); break;
            }
            output = output.mid(index+1);
            times_loaded++;
            if (times_loaded >= 5) {
                index = output.indexOf("\n", 0);
                output = output.mid(index+1);
                times_loaded = 0;
                btc.append(new_vec);
            }
        } else { return; }
    }
    dataframes->append(new_vec); //does not store enough information
}
