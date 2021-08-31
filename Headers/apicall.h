#ifndef APICALL_H
#define APICALL_H
/* When creating a member of this class, remember to allocate it on the heap
 * otherwise readyread will not be called. This applies to every use of QNetworkRequest
 * */
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include "stockdata.h"

class stockdata; //forward declaration

class apicall : public QObject
{
    Q_OBJECT
public:
    apicall(QString apiKey, QString assetName, QString market, QString function);
    void attach(stockdata *observer);
    void dettach(stockdata *observer);
    void notify(QString message);
private:
    QNetworkAccessManager *qnam;
    stockdata *observerMember;
    QString api_key;
    const QString base_api = "https://www.alphavantage.co/query";
    void requestAsset(QString assetName, QString market, QString function);
private slots:
    void readyread(QNetworkReply *reply);
    void encrypted(QNetworkReply *reply);
};

#endif // APICALL_H
