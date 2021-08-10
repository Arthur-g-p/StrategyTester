#ifndef APICALL_H
#define APICALL_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include "subject.h"


class apicall : public subject, public QObject
{
public:
    apicall(QString apiKey, QString assetName, QString market, QString function);
    void attach(observer *observer) override;
    void dettach(observer *observer) override;
    void notify(QString message) override;
private:
    QNetworkAccessManager *qnam = new QNetworkAccessManager();
    observer *observerMember;
    QString api_key;
    const QString base_api = "https://www.alphavantage.co/query";
    void requestAsset(QString assetName, QString market, QString function);
private slots:
    void readyread(QNetworkReply *reply);
};

#endif // APICALL_H
