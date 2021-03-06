#include "./Headers/settingsmanager.h"
/*!
 * \class settingsManager
 * \brief settingsManager is a singleton class which gets, manipulates and transforms setting from the config.ini-file
 */

settingsManager* settingsManager::instance = NULL;

settingsManager::settingsManager()
{
    settings = new QSettings("config.ini", QSettings::IniFormat);
    if(settings->childGroups().size() <= 0) {
        qDebug("Access error for ini file or not found");
        qDebug() << "Ini file should be in the application path: " << QCoreApplication::applicationDirPath() << "named config.ini";
        addAsset("BTC","DIGITAL_CURRENCY_WEEKLY", "USD");
        addAsset("ETH","DIGITAL_CURRENCY_WEEKLY", "USD");
        addAsset("DAX","TIME_SERIES_WEEKLY", "EUR"); //Invalid market, look at API
        settings->setValue("api_key","demo");
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
    return instance;
}

/*!
 * \brief gets the api key that is currently saved in the ini file
 * \returns if the API-Key can not be find it returns "demo"
 */
QString settingsManager::getApiKey()
{
    QString apiKey = settings->value("api_key", "demo").toString();
    if(apiKey == "demo") {
        //QMessageBox::warning(this, "Api Key", "You do not have an Api Key for. Functions may not work properly. You need a key from https://www.alphavantage.co/", QMessageBox::Ok);
        qDebug("you do not have an Api Key for. Functions may not work properly. You need a key from https://www.alphavantage.co/");
    }
    return apiKey;
}

void settingsManager::loadAssets()
{
    QStringList childGroups = settings->childGroups();
    for (int i = 0; i < childGroups.size(); i++) {
        settings->beginGroup(childGroups.at(i));
        asset assetToAdd;
        //QString function = settings->value("function").toString();
        assetToAdd.market = settings->value("market").toString().toUpper();
        assetToAdd.function = settings->value("function").toString().toUpper();
        assetToAdd.name = childGroups.at(i).toUpper();
        assets->append(assetToAdd);
        settings->endGroup();
    }
}


/*!
 * \returns a pointer to the assets
 */
QVector<asset> *settingsManager::getAssets() const
{
    return assets;
}


/*!
 * not implemented yet: add market during runtime, time periods
 * @arg @name Asset name
*/
void settingsManager::addAsset(QString name, bool crypto, QString market)
{
    QString function;
    if(crypto) {
        function = "DIGITAL_CURRENCY_WEEKLY";
    } else {
        function = "TIME_SERIES_WEEKLY";
    }
    settings->beginGroup(name.toUpper());
    settings->setValue("function", function);
    settings->setValue("market", market.toUpper());
    settings->endGroup();
    //reload...
}

void settingsManager::removeAsset(QString name)
{
    settings->remove(name);
    for(int c = 0; c < assets->size(); c++) {
        QString assetName = assets->at(c).name;
        if(assetName == name) {
            assets->removeAt(c);
            break;
        }
    }
}

