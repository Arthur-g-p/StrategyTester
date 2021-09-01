#include "./Headers/stockdata.h"

stockdata* stockdata::instance = NULL;

/*!
 * \brief creates the object stockdata. A call to the constructor starts to download all assets
 */
stockdata::stockdata()
{
    currentAssetApiCall = "";
    currentAssetApiCallIndex = 0;
    invalidCalls = 0;
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
    currentAssetApiCall = "";
    currentAssetApiCallIndex = 0;
    nextApiCall();
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

/*!
 * \brief this function gets called by apicall when the download is ready. From here it checks for errors and decodes the message
 * \returns no return value
 */
void stockdata::update(QString message)     //Also check if there is a connection
{
    if(message.contains("Invalid API call")) {
        qInfo("invalid call");
        settingsManager *sd = settingsManager::getInstance();
        sd->removeAsset(currentAssetApiCall);
        currentAssetApiCallIndex -=1;
        invalidCalls++;
    }
    else if(message.contains("call frequency is ")) {
        qInfo("5 calls per minute. Call already queued.");
        //call after 1 minute. If it does not work after one minute, then 1 day limit is reached
        QTimer::singleShot(60000, this, &stockdata::nextApiCall);
        currentAssetApiCallIndex -=1;
        return;
    }
    else {
        decodeCorrectReply(message);
    }
    nextApiCall();
}

/*!
 * \brief this function decodes the reply, if it is not an error message, and saves the asset information in the variable dataframes
 * \returns no return value
 */
void stockdata::decodeCorrectReply(QString reply)
{
    int index = 0;
    QVector <dataframe> completeAsset;
    index = reply.indexOf("\n", 0);
    reply = reply.mid(index+1);
    index = 0;
    int times_loaded = 0;
    dataframe newFrame;
    for(;;) {
        index = reply.indexOf(",", 0);
        if (index != -1) {
            switch (times_loaded) {
                case 0: newFrame.time = reply.left(index); break;
                case 1: newFrame.open_price = reply.left(index).toFloat(); break;
                case 2: newFrame.high_price = reply.left(index).toFloat(); break;
                case 3: newFrame.low_price = reply.left(index).toFloat(); break;
                case 4: newFrame.close_price = reply.left(index).toFloat(); break;
            }
            reply = reply.mid(index+1);
            times_loaded++;
            if (times_loaded >= 5) {
                index = reply.indexOf("\n", 0);
                reply = reply.mid(index+1);
                times_loaded = 0;
                completeAsset.append(newFrame);
            }
        } else { break; }
    }
    dataframes->append(completeAsset);
}

/*!
 * \brief this function starts the next api call in the queue. If there are no calls left, it does nothing
 * \returns no return value
 */
void stockdata::nextApiCall()
{
    settingsManager *sd = settingsManager::getInstance();
    QVector<asset> *assets = sd->getAssets();
    if(currentAssetApiCallIndex >= assets->size()) {
        QString status = "All API calls have taken place. Invalid calls: ";
        QString invalidCallsStr = QString::number(invalidCalls) + ". Valid calls: " + QString::number(assets->size());
        qInfo() << status+invalidCallsStr;
    } else {
        currentAssetApiCall = assets->at(currentAssetApiCallIndex).name;
        apicall *call = new apicall(sd->getApiKey(), currentAssetApiCall,
                                    assets->at(currentAssetApiCallIndex).market,
                                    assets->at(currentAssetApiCallIndex).function);
        call->attach(this);
    }
    currentAssetApiCallIndex++;
}
