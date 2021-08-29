#ifndef STOCKDATA_H
#define STOCKDATA_H

#include <QDebug>
#include <QString>
#include <QVector>
#include <QTimer>
#include "mainwindow.h"
#include "settingsmanager.h"
#include "apicall.h"

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
    static stockdata* instance;
    stockdata();
    QString currentAssetApiCall;
    unsigned short currentAssetApiCallIndex;
    QVector<QVector<dataframe>> *dataframes;
    void decodeReply(QString reply);
public:
    stockdata(const stockdata&) = delete;

    static stockdata* getInstance();
    typedef enum {
        DIGITAL_CURRENCY_DAILY,
        DIGITAL_CURRENCY_WEEKLY
    } api_function;

     QVector<QVector<dataframe>>* getDataframes();


     void downloadAllAssets();
     void update(QString message);

private slots:
    void nextApiCall();
};

#endif // STOCKDATA_H
