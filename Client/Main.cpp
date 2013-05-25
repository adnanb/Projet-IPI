#include "Mainwindow.hpp"
#include <QApplication>
#include "Core.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Core::Instance().setWindow(&w);
    Core::Instance().setAuthServerAddress("localhost", 10000);
    Core::Instance().setGameServerAddress("localhost", 10001);

    w.show();

    
    return a.exec();
}
