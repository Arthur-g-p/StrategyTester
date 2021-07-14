#include "settingsmanager.h"
/*!
 * \class settingsManager
 * \brief settingsManager is a singleton class which gets, manipulates and transforms setting from the config.ini-file
 */
settingsManager::settingsManager()
{
    settings = new QSettings("config.ini", QSettings::IniFormat);
    if(!settings->isWritable()) {
        qInfo("Error using ini file");
    }
    loadAssets();
}

settingsManager* settingsManager::getInstance()
{
    if(instance==nullptr) {
        instance = new settingsManager();
    }
    return instance; //constructor never called
}

QString settingsManager::getApiKey()
{
    return settings->value("api_key", "demo").toString();
}

QVector<asset> *settingsManager::loadAssets()
{
    QStringList childGroups = settings->childGroups();
    for (int i = 0; i < childGroups.size(); i++) {
        settings->beginGroup(childGroups.at(i));
        QString function = settings->value("function").toString();
        QString market = settings->value("market").toString();
        QString name = childGroups.at(i);
        asset a;
        a.name = name;
        a.market = market;
        assets->append(a);
        settings->endGroup();
    }
    return assets; // more error handling
}

QVector<asset> *settingsManager::getAssets() const
{
    return assets;
}


/*!
 * not implemented yet: add market during runtime
 * @arg @name Asset name
*/
void settingsManager::addAsset(QString name, QString function, QString market)
{
    settings->beginGroup(name);
    settings->setValue("function",function);
    settings->setValue("market", market);
    settings->endGroup();
}
