#include "Core.hpp"
#include <QSslSocket>
#include <string>
#include "StringUtils.hpp"
#include <boost/lexical_cast.hpp>

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
    int code = message[0].toInt();
    message.remove(0);

    switch(code)
    {
    case 1:
        m_authServer.disconnectFromHost();
        emit registerState(message[0].toInt());
        break;

    case 3:
        m_authServer.disconnectFromHost();
        if(message[0] == "0")
        {
            m_gameServer.connectToHost();
            NetstringBuilder str;
            str << "6" << m_login << message[1].toStdString();
            m_gameServer.sendToHost(str);
            emit loginState(2);
        }
        else
        {
            emit loginState(message[0].toInt());
        }
        break;

    case 7:
        if(message[0] == "0")
        {
            emit loginState(0);
        }
        else
        {
            m_gameServer.disconnectFromHost();
            emit loginState(3);
        }
        break;

    case 9:
        emit createState(message[0].toInt());
        break;

    case 11:
        m_window->listState(message);
        break;

    case 24:
        m_window->refreshList(message);
        break;

    case 13:
        //Un joueur vient d'entrer dans la partie
        m_window->addPlayer(message);
        break;

    case 15:
        m_window->waitState(message[0].toInt());
        break;

    case 17:
        m_window->launchGame();
        break;

    //Un joueur a joué
    case 19:
    {
        int x = message[0].toInt();
        int y = message[1].toInt();
        QString player = message[2];
        m_window->play(x, y, player);
        break;
    }

    case 25:
        m_window->setGameInfos(message);
        break;

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
    str << "20";
    m_gameServer.sendToHost(str);
}

void Core::joinGame(std::string name)
{
    NetstringBuilder str;
    str << "10" << name;
    m_gameServer.sendToHost(str);
}

void Core::launchGame()
{
    NetstringBuilder str;
    str << "12";
    m_gameServer.sendToHost(str);
}

std::string Core::getLogin()
{
    return m_login;
}

void Core::play(int x, int y)
{
    NetstringBuilder str;
    std::string _x = boost::lexical_cast<std::string>(x);
    std::string _y = boost::lexical_cast<std::string>(y);
    str << "14" << _x << _y << m_login;
    m_gameServer.sendToHost(str);
}
