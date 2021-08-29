#include "./Headers/apicall.h"

apicall::apicall(QString apiKey, QString assetName, QString market, QString function)
{
    connect(qnam, &QNetworkAccessManager::encrypted, this, &apicall::encrypted);
    api_key = apiKey;
    requestAsset(assetName, market, function);
}

void apicall::attach(stockdata *observer)
{
    observerMember = observer;
}

void apicall::dettach(stockdata *observer)
{
    //observers->remove(*observer);
}

void apicall::notify(QString message)
{
    observerMember->update(message);
    //observer *obs = observerMember;
    //obs->update(message);
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

    QNetworkReply *reply = qnam->get(QNetworkRequest(QUrl(url)));
    connect(reply, &QNetworkReply::readyRead, this, &apicall::readyread);
}

void apicall::readyread(QNetworkReply *reply)
{
    QString output = reply->readAll();
    notify(output);
}

void apicall::encrypted(QNetworkReply *reply)
{
    qInfo("encrypted");
}
