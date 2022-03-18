#ifndef STOCKDATA_H
#define STOCKDATA_H

#include <QDebug>
#include <QString>
#include <QVector>
#include <QTimer>
#include <QFile>
#include <QDataStream>
#include <QFile>
#include <QMap>
#include <QQueue>
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
    QString currentAssetApiCallName;
    QString currentAssetApiCallMarket;
    QString currentAssetApiCallFunction;
    unsigned short currentAssetApiCallIndex;
    QString apiKey;

    unsigned short invalidCalls;
    QVector<QVector<dataframe>> *dataframes;
    QMap<QString, QPair<QVector<dataframe>, bool>> *stockDataMapPair; //The bool is added to ask if the data has been downloaded

    QQueue<QString> pendingAssetApiCallName;
    QQueue<QString> pendingAssetApiCallMarket;
    QQueue<QString> pendingAssetApiCallFunction;

    bool readFromCsv(const QString asset, const QString market);
    bool decodeCorrectReply(QString reply, const QString assetName = "", const QString assetMarket = "", const QString assetIndex = "");
    void saveToCsv(QString reply, const QString assetName = "", const QString assetMarket = "");
    void apiCall(const QString customApiCallName = "", const QString customAssetApiCallMarket = "", const QString customAssetApiCallFunction = "");

public:
    stockdata(const stockdata&) = delete; //Singleton must-have?
    static stockdata* getInstance();
    typedef enum {
        DIGITAL_CURRENCY_DAILY,
        DIGITAL_CURRENCY_WEEKLY
    } api_function;

    QVector<QVector<dataframe>>* getDataframes();

    void downloadAllAssets();
    void update(QString message, const QString assetName, const QString assetMarket, const QString assetIndex);     //This is a function that gets called from apicall. Callback


    QList<QString> getAllKeys() const;
    QPair<QVector<dataframe>, bool> getAssetByNamePair(const QString asset, const QString market);
    QPair<QVector<dataframe>, bool> getAssetByNamePair(const QString assetAndMarket); //function overload
    void nextAsset();
private slots:
    void pendingApiCall();
};

#endif // STOCKDATA_H
