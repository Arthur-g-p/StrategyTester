#include "settingsmanager.h"
/*!
 * \class settingsManager
 * \brief settingsManager is a singleton class which gets, manipulates and transforms setting from the config.ini-file
 */

settingsManager* settingsManager::instance = NULL;

settingsManager::settingsManager()
{
    settings = new QSettings("config.ini", QSettings::IniFormat);
    if(!settings->isWritable()) {
        qInfo("Ini file not writable");
    }
    if(!settings->QSettings::NoError) {
        qInfo("Access error for ini file"); //Print path where the ini file needs to be
    }
    assets = new QVector<asset>;
    assets->clear();
    loadAssets();
}

settingsManager* settingsManager::getInstance()
{
    if(instance==NULL) {
        instance = new settingsManager();
    }
    return instance; //constructor never called
}

/*!
 * \brief gets the api key that is currently saved in the ini file
 * \returns if the API-Key can not be find it returns "demo"
 */
QString settingsManager::getApiKey()
{
    return settings->value("api_key", "demo").toString();
}

void settingsManager::loadAssets()
{
    QStringList childGroups = settings->childGroups();
    for (int i = 0; i < childGroups.size(); i++) {
        settings->beginGroup(childGroups.at(i));
        asset assetToAdd;
        //QString function = settings->value("function").toString();
        assetToAdd.market = settings->value("market").toString().toUpper();
        assetToAdd.name = childGroups.at(i).toUpper();
        assets->append(assetToAdd);
        settings->endGroup();
    }
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
