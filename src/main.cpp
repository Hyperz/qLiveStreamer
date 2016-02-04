
#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setOrganizationName("Hyperz");
    QApplication::setApplicationName("qLiveStreamer");
    QApplication::setApplicationVersion("0.1.2");

    MainWindow w;
    w.show();
    
    return a.exec();
}
