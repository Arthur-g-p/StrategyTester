#ifndef STOCKDATA_H
#define STOCKDATA_H

#include <QDebug>
#include <QString>
#include <QVector>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "mainwindow.h"
#include "settingsmanager.h"
#include "observer.h"
#include "apicall.h"

struct dataframe {
    QString time;
    float open_price;
    float high_price;
    float low_price;
    float close_price;
};

class stockdata : public observer, public QObject
{
    Q_OBJECT
private:
    static stockdata* instance;
    stockdata();

    QNetworkAccessManager *qnam = new QNetworkAccessManager();
    QVector<QVector<dataframe>> *dataframes;

public:
    stockdata(const stockdata&) = delete;

    static stockdata* getInstance();
    typedef enum {
        DIGITAL_CURRENCY_DAILY,
        DIGITAL_CURRENCY_WEEKLY
    } api_function;

     QVector<QVector<dataframe>>* getDataframes();


     void downloadAllAssets();
     void update();

public slots:
    void requestAsset(QString symbol, QString market, api_function function = DIGITAL_CURRENCY_WEEKLY);

private slots:
    void readyread(QNetworkReply *reply);
};

#endif // STOCKDATA_H
