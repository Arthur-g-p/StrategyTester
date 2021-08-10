#include "stockdata.h"

stockdata* stockdata::instance = NULL;

stockdata::stockdata()
{
    connect(qnam, &QNetworkAccessManager::finished, this, &stockdata::readyread);
    currentAssetApiCall = "";
    currentAssetApiCallIndex = 0;
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
        apicall ac(apiKey, assets->at(c).name, assets->at(c).market, "tbd");
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

void stockdata::update(QString message)
{
    if(message.contains("Invalid API call")) {
        //Delete this asset
        qInfo("invalid call");
        settingsManager *sd = settingsManager::getInstance();
        sd->removeAsset(currentAssetApiCall);
        currentAssetApiCallIndex -=1;
    }
    else if(message.contains("call frequency is ")) {
        qInfo("5 calls per minute. Call already queued.");

    }
    //Handle request
    //remove bad requests
    //save data in dataframe
    //multiple returnvalues requierd
}

void stockdata::readyread(QNetworkReply *reply)
{

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

void stockdata::nextApiCall()
{
    currentAssetApiCallIndex++;
    settingsManager *sd = settingsManager::getInstance();
    QVector<asset> *assets = sd->getAssets();
    if(currentAssetApiCallIndex >= assets->size()) {
        //finished
        qInfo("All assets downloaded");
    } else {
        currentAssetApiCall = assets->at(currentAssetApiCallIndex).name;
        apicall(sd->getApiKey(), currentAssetApiCall, assets->at(currentAssetApiCallIndex).market, "tbd");
    }
}
