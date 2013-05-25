#include "Network.hpp"
#include <QSslSocket>
#include <QDataStream>
#include <QMessageBox>
#include "StringUtils.hpp"
#include "Core.hpp"

//class Core;

Network::Network():
    m_blockSize(0),
    m_data("")
{
    m_sslSocket = new QSslSocket();
    m_sslSocket->setPeerVerifyMode(QSslSocket::VerifyNone);
    m_sslSocket->setProtocol(QSsl::TlsV1_0);
    connect(m_sslSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect(m_sslSocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
}

Network::Network(const std::string address, const quint16 port):
    m_hostAddress(m_hostAddress.fromStdString(address)),
    m_hostPort(port)
{
    Network();
}

Network::~Network()
{
    delete m_sslSocket;
}

void Network::setHostAddress(const std::string address, const quint16 port)
{
    m_hostAddress = m_hostAddress.fromStdString(address);
    m_hostPort = port;
}

void Network::connectToHost() const
{
    if(m_sslSocket->state() == QAbstractSocket::UnconnectedState)
        m_sslSocket->connectToHostEncrypted(m_hostAddress, m_hostPort);
}

void Network::disconnectFromHost() const
{
    if(m_sslSocket->state() != QAbstractSocket::UnconnectedState)
        m_sslSocket->disconnectFromHost();
}

void Network::sendToHost(std::string message) const
{
    m_sslSocket->write(message.c_str());
}

void Network::displayError(QAbstractSocket::SocketError error) const
{
    switch (error)
    {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(NULL, tr("Error"),
                                 tr("Hôte non trouvé. Vérifiez le nom de l'hôte et ses paramètres."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(NULL, tr("Error"),
                                 tr("Connexion refusée. Le serveur est-il démarré ?"));
        break;
    default:
        QMessageBox::information(NULL, tr("Error"),
                                 tr("L'erreur suivante s'est produite : %1.")
                                 .arg(m_sslSocket->errorString()));
    }
}

//Bonjour, vous avez un nouveau message !
void Network::slotReadyRead()
{
    int size = m_sslSocket->bytesAvailable();
    m_data.append(m_sslSocket->readAll());

    if(!m_blockSize)
    {
        if(!m_data.contains(':'))
            return;

        int pos = m_data.indexOf(":");
        m_blockSize = atoi(m_data.left(pos));
        m_data = m_data.mid(pos+1);
        size -= pos+1;
    }

    if(size < m_blockSize)
        return;

    //A ce stade nous avons le message en entier
    Core::Instance().messageHandler(StringExtract(m_data.data()));
    m_data.clear();
    m_blockSize = 0;
}

