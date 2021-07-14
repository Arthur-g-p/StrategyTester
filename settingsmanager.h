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

    void loadAssets();
public:
    settingsManager(const settingsManager&) = delete;

    static settingsManager* getInstance();
    void addAsset(QString name, QString function, QString market);
    QVector<asset> *getAssets() const;
    QString getApiKey();

    //void removeAsset(QString name);
};

#endif // SETTINGSMANAGER_H