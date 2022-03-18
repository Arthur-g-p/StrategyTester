#include "./Headers/apicall.h"

apicall::apicall(QString apiKey, QString assetName, QString market, QString function)
{
    this->assetName = assetName;
    this->market = market;
    this->function = function;
    qnam = new QNetworkAccessManager();
    connect(qnam, &QNetworkAccessManager::finished, this, &apicall::readyread);
    api_key = apiKey;
    requestAsset(assetName, market, function);
}

void apicall::attach(stockdata *observer)
{
    observerMember = observer;
}

void apicall::notify(QString message)
{
    if(observerMember != NULL)
    {
        observerMember->update(message, assetName, market, function);
    }
}

void apicall::requestAsset(QString _assetName, QString _market, QString function)
{
    QString url = base_api;
    url.append("?function="+function);
    url.append("&symbol="+_assetName);
    url.append("&market="+_market);
    url.append("&apikey="+api_key);
    url.append("&datatype=csv");

    qnam->get(QNetworkRequest(QUrl(url)));      //Calls and waits for a response. The response comes in at readyread
}

void apicall::readyread(QNetworkReply *reply)
{
    QString output = reply->readAll();
    notify(output);
}
