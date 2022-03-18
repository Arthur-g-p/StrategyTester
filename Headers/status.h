#ifndef STATUS_H
#define STATUS_H

#include <QString>
#include <QStatusBar>
#include <QDebug>

class status
{
public:
    void setStatusText(QString barText);
    void setStatusBar(QStatusBar *statusBar);
    static status* getInstance();
    status(const status&) = delete;
private:
    static status *instance;
    status();
    QStatusBar *statusBar;
};

#endif // STATUS_H
