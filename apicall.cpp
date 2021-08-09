#include "apicall.h"

apicall::apicall(QString apiKey, QString assetName, QString market, QString function)
{
    connect(qnam, &QNetworkAccessManager::finished, this, &apicall::readyread);
    api_key = apiKey;
    requestAsset(assetName, market, function);
}

void apicall::attach(observer *observer)
{
    observers->append(*observer);
}

void apicall::dettach(observer *observer)
{
    //observers->remove(*observer);
}

void apicall::notify()
{
    for(int c = 0; c < observers->size(); c++) {
        observers->at(c).update;
    }
}

void apicall::requestAsset(QString assetName, QString market, QString function)
{
    QString url = base_api;
    /*
    if(function == DIGITAL_CURRENCY_DAILY) {
        url.append("?function=DIGITAL_CURRENCY_DAILY");
    }
    else if (function == DIGITAL_CURRENCY_WEEKLY) {
        url.append("?function=DIGITAL_CURRENCY_WEEKLY"); //Not everthing is digital
    }*/
    url.append("?function=DIGITAL_CURRENCY_WEEKLY");
    url.append("&symbol="+assetName);
    url.append("&market="+market);
    url.append("&apikey="+api_key);
    url.append("&datatype=csv");

    qnam->get(QNetworkRequest(QUrl(url)));
}

void apicall::readyread(QNetworkReply *reply)
{
    observer *obs = new observer();
    obs->update();
}
