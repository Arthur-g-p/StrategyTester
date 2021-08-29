#ifndef ADD_ASSET_H
#define ADD_ASSET_H

#include <QWidget>
#include "settingsmanager.h"

namespace Ui {
class add_asset;
}

class add_asset : public QWidget
{
    Q_OBJECT

public:
    explicit add_asset(QWidget *parent = nullptr);
    ~add_asset();

private slots:
    void on_Cancel_button_clicked();
    void on_Ok_button_clicked();

private:
    Ui::add_asset *ui;
};

#endif // ADD_ASSET_H
