#include "./Headers/add_asset.h"
#include "ui_add_asset.h"

add_asset::add_asset(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_asset)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
}

add_asset::~add_asset()
{
    delete ui;
}

/*!
 * \fn
 *
 * */
void add_asset::on_Ok_button_clicked()
{
    settingsManager *sm = settingsManager::getInstance();
    QString market = ui->Market_text->text();
    QString asset = ui->Asset_text->text();
    if(ui->crypto_box->isChecked()) {
        sm->addAsset(asset, true, market);
    } else {
        sm->addAsset(asset, false, market);
    }
    this->close();
}


void add_asset::on_Cancel_button_clicked()
{
    this->close();
}
