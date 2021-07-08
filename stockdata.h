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

struct dataframe {
    QString time;
    float open_price;
    float high_price;
    float low_price;
    float close_price;
};

class stockdata : public QObject
{
    Q_OBJECT
private:
    static stockdata instance;
    stockdata();

    QNetworkAccessManager *qnam = new QNetworkAccessManager();
    QString api_key;
    const QString base_api = "https://www.alphavantage.co/query";
    QVector<dataframe> dataframes;

public:
    stockdata(const stockdata&) = delete;

    static stockdata& getInstance();
    typedef enum {
        DIGITAL_CURRENCY_DAILY,
        DIGITAL_CURRENCY_WEEKLY
    } api_function;

    QVector<dataframe>* getDataframes();


public slots:
    void requestAsset(QString symbol, api_function function = DIGITAL_CURRENCY_WEEKLY);

private slots:
    void readyread(QNetworkReply *reply);
};
#endif // STOCKDATA_H
