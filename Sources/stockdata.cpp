#include "./Headers/stockdata.h"
#include "./Headers/status.h"

stockdata* stockdata::instance = NULL;

/*!
 * \brief creates the object stockdata. A call to the constructor starts to download all assets
 */
stockdata::stockdata()
{
    invalidCalls = 0;
    stockDataMapPair = new QMap<QString, QPair<QVector<dataframe>, bool>>;
    stockDataMapPair->clear();
    pendingAssetApiCallName.clear();
    pendingAssetApiCallMarket.clear();
    pendingAssetApiCallFunction.clear();

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
    currentAssetApiCallName = "";
    currentAssetApiCallIndex = 0;
    currentAssetApiCallMarket = "";

    settingsManager *sd = settingsManager::getInstance();
    QVector<asset> *assets = sd->getAssets();       //This needs to be asked over and over again because it is changing. Deletes take place.
    apiKey = sd->getApiKey();
    while (currentAssetApiCallIndex < assets->size()) {
        nextAsset();
        currentAssetApiCallIndex++;
    }
    status *st = status::getInstance();
    QString status = "All API calls have taken place. Invalid calls: ";     //Zu früh, reply stheht noch offen
    QString invalidCallsStr = QString::number(invalidCalls) + ". Valid calls: " + QString::number(assets->size());
    st->setStatusText(status+invalidCallsStr);
}

/*!
 * \brief gets the pointer to the dataframes structure
 * \returns if the dataframe structure is empty, 0 is returned. It is advised to call the function later
 */
QVector<QVector<dataframe>>* stockdata::getDataframes()
{
    return dataframes;
}

/*!
 * \brief this function gets called by apicall when the download is ready. From here it checks for errors and decodes the message
 * As Input parameters it takes the message and the information of the asset given to api call. If there are multiple apicalls it would
 * not be possible to identify the reply message. A queue would work, but the replies don't come in the order in which they have been called
 * because of the different sizes.
 * \returns no return value
 */
void stockdata::update(QString message, const QString assetName, const QString assetMarket, const QString assetFunction)
{
    if(message.contains("Invalid API call")) {
        status *st = status::getInstance();
        QString statusText = "invalid call for "+assetName+assetMarket;
        st->setStatusText(statusText);
        settingsManager *sd = settingsManager::getInstance();
        sd->removeAsset(assetName);
        invalidCalls++;
    }
    else if(message.contains("call frequency is ")) {//NOT WORKING
        qInfo("5 calls per minute. Call already queued.");
        //call after 1 minute. If it does not work after one minute, then 1 day limit (500) is reached, not implemented
        pendingAssetApiCallName.enqueue(assetName);
        pendingAssetApiCallMarket.enqueue(assetMarket);
        pendingAssetApiCallFunction.enqueue(assetFunction);

        QTimer::singleShot(60000, this, &stockdata::pendingApiCall);
        currentAssetApiCallIndex -=1;
    }
    else if(message.isEmpty()) {
        qInfo("No internet connection");
    }
    else {
        try {
            decodeCorrectReply(message, assetName, assetMarket, assetFunction);
        } catch (...) { //Catch any
            qInfo("The API returned a reply this program cannot interpret. Please use alphavantage.");
            return;
        }
        saveToCsv(message, assetName, assetMarket);
    }
}

/*!
 * saves to csv with naming convention
 * Only if the call was successfull
*/
void stockdata::saveToCsv(QString reply, const QString assetName, const QString assetMarket)
{
    QString fileName;
    if (assetName.isEmpty()) {
        fileName = currentAssetApiCallName+"_"+currentAssetApiCallMarket;
    } else {
        fileName = assetName+"_"+assetMarket;
    }
    QFile csvToSave(fileName+".csv");
    csvToSave.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream out(&csvToSave);
    out << reply;

    csvToSave.close();
}

bool stockdata::readFromCsv(const QString asset, const QString market)
{
    QString fileName = asset+"_"+market;
    fileName = fileName.toUpper();

    QFile csvToRead(fileName+".csv");
    if(!csvToRead.open(QFile::ReadOnly | QFile::Text)) {
        qInfo() << "Can't open";
        csvToRead.close();
        return false;
    } else {
        QTextStream in(&csvToRead);
        QString reply = in.readAll();
        //while (!in.atEnd()) {
        //    QString line = in.readLine()
        //}
        if(decodeCorrectReply(reply)) {
            csvToRead.close();
            return true;        // Means the value is inside now and can be read
        } else {
            csvToRead.close();
            return false;
            //No reading possible, user played with the csv. Delete!
        }
    }
}

QList<QString> stockdata::getAllKeys() const
{
    return stockDataMapPair->keys();
}

QPair<QVector<dataframe>, bool> stockdata::getAssetByNamePair(const QString asset, const QString market)
{

    auto it = stockDataMapPair->find(asset.toUpper()+market.toUpper());
    if (it != stockDataMapPair->end()) {
        return *it;
    }
    QPair<QVector<dataframe>, bool> not_existent (QVector<dataframe>(), false);
    return not_existent;
}

QPair<QVector<dataframe>, bool> stockdata::getAssetByNamePair(const QString assetAndMarket)
{
    auto it = stockDataMapPair->find(assetAndMarket.toUpper());
    if (it != stockDataMapPair->end()) {
        return *it;
    } /*else {
        qInfo("There is no asset with this name!");
        if(!readFromCsv(asset, market)) {            //Go check csv
            qInfo("It is not even saved on the disk!");
            //api call from here?
            return (QVector<dataframe>)0;   //Bad, remove soon because it can be guarenteed that the data exists. Still check in th caller
        } else {

        }
    }*/ // Should I check the disk? I think this is only necessary for add asset
    QPair<QVector<dataframe>, bool> not_existent (QVector<dataframe>(), false);
    return not_existent;
}

/*!
 * \brief this function decodes the reply, if it is not an error message, and saves the asset information in the stockDataMapPair
 * \returns no return value
 */
bool stockdata::decodeCorrectReply(QString reply, const QString assetName, const QString assetMarket, const QString assetIndex)
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
    //Make an exception catch and return false
    std::reverse(completeAsset.begin(), completeAsset.end()); //Reveres for more intuitive access

    QPair<QVector<dataframe>, bool> finished(completeAsset, true); // problem still not solved, it needs to know where the caller comes from. function paramter

    if (!assetName.isEmpty()) {
        stockDataMapPair->insert(assetName+assetMarket, finished); //Updates
    } else {
        stockDataMapPair->insert(currentAssetApiCallName+currentAssetApiCallMarket, finished);
    }
    return true;
}

/*!
 * \brief this function starts the next api call in the queue. If there are no calls left, it does nothing
 * \returns no return value
 */
void stockdata::apiCall(const QString customApiCallName, const QString customAssetApiCallMarket, const QString customAssetApiCallFunction)
{
    QPair<QVector<dataframe>, bool> unfished(QVector<dataframe>(), false);
    if (customApiCallName.isEmpty()) {
        apicall *call = new apicall(apiKey, currentAssetApiCallName,
                                currentAssetApiCallMarket,
                                currentAssetApiCallFunction);

        stockDataMapPair->insert(currentAssetApiCallName+currentAssetApiCallMarket, unfished);
        call->attach(this);
    } else {
        apicall *call = new apicall(apiKey, customApiCallName,
                                customAssetApiCallMarket,
                                customAssetApiCallFunction);

        stockDataMapPair->insert(customApiCallName+customAssetApiCallMarket, unfished); //Keep the override, maybe I want to reload
        call->attach(this);
    }
}

void stockdata::pendingApiCall()
{
    apiCall(pendingAssetApiCallName.dequeue(), pendingAssetApiCallMarket.dequeue(), pendingAssetApiCallFunction.dequeue());
}

void stockdata::nextAsset()
{
    settingsManager *sd = settingsManager::getInstance();
    QVector<asset> *assets = sd->getAssets();       //This needs to be asked over and over again because it is changing. Deletes take place.
    currentAssetApiCallName = assets->at(currentAssetApiCallIndex).name;
    currentAssetApiCallMarket = assets->at(currentAssetApiCallIndex).market;
    currentAssetApiCallFunction = assets->at(currentAssetApiCallIndex).function;
    if(!readFromCsv(currentAssetApiCallName, currentAssetApiCallMarket)) {      // Data is not stored
        apiCall();
        //Dont I need to remove the pointer?
    } else {
        //With this way, u can insert data into the program without making it exist in the alphavantage api. But it needs the right format
        //Also should check if the data is too old from time to time. Maybe even just append new data and dont delete old
    }
}
