#pragma once

#include <string>
#include <QHostAddress>
#include "Network.hpp"
#include "Singleton.hpp"
#include <Mainwindow.hpp>

class Core : public QObject, public Singleton<Core>
{
    MAKE_SINGLETON(Core)
    Q_OBJECT
public:
    void setAuthServerAddress(const std::string address, const unsigned short port);
    void setGameServerAddress(const std::string address, const unsigned short port);
    void login(const std::string login, const std::string password) const;
    void registerToAuth(const std::string login, const std::string password) const;
    void setWindow(MainWindow* window);


    void messageHandler(QVector<QString> message);

signals:
    void registerState(int code);
    void loginState(int code);



private:
    Core();
    ~Core();
    Network m_authServer;
    Network m_gameServer;
    int m_magicCode;
    std::string m_login;
    MainWindow* m_window;

};
