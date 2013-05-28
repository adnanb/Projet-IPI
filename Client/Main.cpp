#include "Mainwindow.hpp"
#include <QApplication>
#include "Core.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Core::Instance().setWindow(&w);
    Core::Instance().setAuthServerAddress("192.168.68.190", 10000);
    Core::Instance().setGameServerAddress("192.168.68.190", 10001);

    w.show();

    
    return a.exec();
}
