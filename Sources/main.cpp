#include "./Headers/mainwindow.h"

#include <QApplication>

//QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show(); //starts window
    return a.exec();
}
