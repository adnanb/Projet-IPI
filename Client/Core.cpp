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

void Core::login(const std::string login, const std::string password)
{
    m_login = login;
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

void Core::gameCreate(const std::string name, const std::string nbPlayers, const std::string size, const std::string nbAlign)
{
    NetstringBuilder str;
    str << "8" << name << nbPlayers << size << nbAlign;
    m_gameServer.sendToHost(str);
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
        if(message[1] == "0")
        {
            m_gameServer.connectToHost();
            NetstringBuilder str;
            str << "6" << m_login << message[2].toStdString();
            m_gameServer.sendToHost(str);
            emit loginState(2);
        }
        else
        {
            emit loginState(message[1].toInt());
        }
    }

    if(code == "7")
    {
        if(message[1] == "0")
        {
            emit loginState(0);
        }
        else
        {
            m_gameServer.disconnectFromHost();
            emit loginState(3);
        }
    }

    if(code == "9")
        emit createState(message[1].toInt());

    if(code == "11")
        m_window->listState(message[1].toInt());

    if(code == "24")
    {
        //On supprime le code du mesage, seul le message nous intéresse
        message.remove(0);
        m_window->refreshList(message);
    }


}

void Core::setWindow(MainWindow* window)
{
    m_window = window;
    connect(this, SIGNAL(loginState(int)), m_window, SLOT(slotLoginState(int)));
    connect(this, SIGNAL(registerState(int)), m_window, SLOT(slotRegisterState(int)));
    connect(this, SIGNAL(createState(int)), m_window, SLOT(slotCreateState(int)));
}

void Core::getGamesList()
{
    NetstringBuilder str;
    str << "19";
    m_gameServer.sendToHost(str);
}

void Core::joinGame(std::string name)
{
    NetstringBuilder str;
    str << "10" << name;
    m_gameServer.sendToHost(str);
}
