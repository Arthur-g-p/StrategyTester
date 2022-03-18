#include "./Headers/addstrategy.h"
#include "ui_addstrategy.h"
#include "Headers/addtab.h"


addStrategy::addStrategy(QTabWidget *tabWidget, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addStrategy)
{
    ui->setupUi(this);
    this->tabWidget = tabWidget;
    connect(ui->buttonOk, &QPushButton::clicked, this, &addStrategy::on_Ok_button_clicked);
    connect(ui->buttonCancel, &QPushButton::clicked, this, &addStrategy::on_Cancel_button_clicked);
}

void addStrategy::on_Ok_button_clicked()
{
    strategy *createdStrategy = createStrategyFromDialog(); //checks input and prints hints
    if (createdStrategy != nullptr) {   //check if input is okay
        QString strategyName = ui->strategyName->toPlainText();
        tabWidget->addTab(new addtab(createdStrategy), strategyName);
        this->close();
    }
}

void addStrategy::on_Cancel_button_clicked()
{
    this->close();
}


strategy* addStrategy::createStrategyFromDialog()
{
    QString hintText = "";
    QString strategyName = ui->strategyName->toPlainText();
    int movingAverageNumber = ui->movingAverageNumber->toPlainText().toInt();
    int movingAverageNumber2 = ui->movingAverageNumber2->toPlainText().toInt();
    if (movingAverageNumber == 0 || movingAverageNumber2 == 0) {
        hintText = "Moving averages cannot be 0!";
        movingAverageNumber = 20;
    }
    if (strategyName.size() == 0) {
        hintText += "\nStrategies should have a name!";
    }
    if (hintText.size() > 0) {
        ui->hint_label->setEnabled(true);
        ui->hint_label->setText(hintText);
        return nullptr;
    }
    strategy *createdStrategy = new strategy(strategyName, movingAverageNumber);
    return createdStrategy;
}

addStrategy::~addStrategy()
{
    //Never called
    qInfo("done");
    delete ui;
}

