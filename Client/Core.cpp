#include "Core.hpp"
#include <QSslSocket>
#include <string>
#include "StringUtils.hpp"

SINGLETON_IMPL(Core)

Core::Core()
{

}

Core::~Core()
{
}

void Core::setAuthServerAddress(const std::string address, const quint16 port)
{
    m_authServer.setHostAddress(address, port);
}

void Core::setGameServerAddress(const std::string address, const quint16 port)
{
    m_gameServer.setHostAddress(address, port);
}

void Core::login(const std::string login, const std::string password) const
{
    NetstringBuilder str;
    str << "2" << login << password;
    m_authServer.connectToHost();
    m_authServer.sendToHost(str);
}

void Core::registerToAuth(const std::string login, const std::string password) const
{
    NetstringBuilder str;
    str << "0" << login << password;
    m_authServer.connectToHost();
    m_authServer.sendToHost(str);
}

void Core::messageHandler(QVector<QString> message)
{
    QString code = message[0];

    if(code == "1")
    {
        m_authServer.disconnectFromHost();
        emit registerState(message[1].toInt());

    }

    if(code == "3")
    {
        m_authServer.disconnectFromHost();
        qDebug() << message[1];
        if(message[1] == "0")
        {
            m_gameServer.connectToHost();
            NetstringBuilder str;
            str << "6" << message[2].toStdString();
            m_gameServer.sendToHost(str);
        }
        emit loginState(message[1].toInt());
    }

}

void Core::setWindow(MainWindow* window)
{
    m_window = window;
    connect(this, SIGNAL(loginState(int)), m_window, SLOT(slotLoginState(int)));
    connect(this, SIGNAL(registerState(int)), m_window, SLOT(slotRegisterState(int)));
}
