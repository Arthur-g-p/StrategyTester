#include "./Headers/apicall.h"

apicall::apicall(QString apiKey, QString assetName, QString market, QString function)
{
    qnam = new QNetworkAccessManager();
    connect(qnam, &QNetworkAccessManager::finished, this, &apicall::readyread);
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
    if(observerMember != NULL)
    {
        observerMember->update(message);
    }
}

void apicall::requestAsset(QString assetName, QString market, QString function)
{
    QString url = base_api;
    url.append("?function="+function);
    url.append("&symbol="+assetName);
    url.append("&market="+market);
    url.append("&apikey="+api_key);
    url.append("&datatype=csv");

    qnam->get(QNetworkRequest(QUrl(url)));
}

void apicall::readyread(QNetworkReply *reply)
{
    QString output = reply->readAll();
    notify(output);
}
