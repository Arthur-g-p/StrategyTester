#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QSettings>

struct asset {
    QString name;
    QString market;
};

class settingsManager
{
private:
    static settingsManager* instance;
    settingsManager();

    QSettings *settings;
    QVector<asset> *assets;

public:
    settingsManager(const settingsManager&) = delete;

    static settingsManager* getInstance();
    QVector<asset> *loadAssets();
    void addAsset(QString name, QString function, QString market);
    //void removeAsset(QString name);
    QVector<asset> *getAssets() const;
    QString getApiKey();
};

#endif // SETTINGSMANAGER_H
