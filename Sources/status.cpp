#include "./Headers/status.h"

status* status::instance = NULL;

status::status()
{
    this->statusBar = nullptr;
}

status *status::getInstance()
{
    if(instance==NULL) {
        instance = new status();
    }
    return instance;
}

void status::setStatusBar(QStatusBar *statusBar)
{
    this->statusBar = statusBar;
}

void status::setStatusText(QString barText)
{
    if (!barText.isEmpty()) {
        qInfo() << "Status text: "+barText;
        statusBar->showMessage(barText);
    }
}
