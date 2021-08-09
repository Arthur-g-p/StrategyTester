#include "stockdata.h"

stockdata* stockdata::instance = NULL;

stockdata::stockdata()
{
    connect(qnam, &QNetworkAccessManager::finished, this, &stockdata::readyread);
    dataframes = new QVector<QVector<dataframe>>;
    dataframes->clear();
    downloadAllAssets();
}

stockdata *stockdata::getInstance()
{
    if(instance==NULL) {
        instance = new stockdata ();
    }
    return instance;
}


void stockdata::downloadAllAssets()
{
    settingsManager *sm = settingsManager::getInstance();
    QVector<asset> *assets = sm->getAssets();
    QString apiKey = sm->getApiKey();
    for(int c = 0; c < assets->size(); c++) {
        //requestAsset(assets->at(0).name, assets->at(0).market);
        apicall ac(apiKey, assets->at(c).name, assets->at(c).market, "0");
        //request and wait for message to come back. Queue
    }
}

/*!
 * \brief gets the pointer to the dataframes structure
 * \returns if the dataframe structure is empty, 0 is returned. It is advised to call the function later
 */
QVector<QVector<dataframe>>* stockdata::getDataframes()
{
    if(dataframes->empty()) {
        return 0;
    } else {
        return dataframes;
    }
}

void stockdata::requestAsset(QString symbol, QString market, api_function function)
{
    QString url = base_api;
    if(function == DIGITAL_CURRENCY_DAILY) {
        url.append("?function=DIGITAL_CURRENCY_DAILY");
    }
    else if (function == DIGITAL_CURRENCY_WEEKLY) {
        url.append("?function=DIGITAL_CURRENCY_WEEKLY"); //Not everthing is digital
    }
    url.append("&symbol="+symbol);
    url.append("&market="+market);
    url.append("&apikey="+api_key);
    url.append("&datatype=csv");

    qnam->get(QNetworkRequest(QUrl(url)));
}

void stockdata::update()
{
    //Handle request
    //remove bad requests
    //save data in dataframe
    //multiple returnvalues requierd
}

void stockdata::readyread(QNetworkReply *reply)
{
    QString output = reply->readAll();
    // errors ocurring: -5 calls per minute, 500 calls per day --> max frequency. In this case wait and retry. (Invalid tries count)
    if(output.contains("Invalid API call")) {
        //Delete this asset
        qInfo("invalid call");
    }
    if(output.contains("call frequency is ")) {
        qInfo("5 calls per minute. Call already queued.");
    }
    // - invalid call. In this case remove this asset
    int index = 0;
    QVector <dataframe> completeAsset;
    index = output.indexOf("\n", 0);
    output = output.mid(index+1);
    index = 0;
    int times_loaded = 0;
    dataframe newFrame;
    for(;;) {
        index = output.indexOf(",", 0);
        if (index != -1) {
            switch (times_loaded) {
                case 0: newFrame.time = output.left(index); break;
                case 1: newFrame.open_price = output.left(index).toFloat(); break;
                case 2: newFrame.high_price = output.left(index).toFloat(); break;
                case 3: newFrame.low_price = output.left(index).toFloat(); break;
                case 4: newFrame.close_price = output.left(index).toFloat(); break;
            }
            output = output.mid(index+1);
            times_loaded++;
            if (times_loaded >= 5) {
                index = output.indexOf("\n", 0);
                output = output.mid(index+1);
                times_loaded = 0;
                completeAsset.append(newFrame);
            }
        } else { break; }
    }
    dataframes->append(completeAsset); //does not store enough information
}
