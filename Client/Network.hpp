#pragma once

#include <QSslSocket>
#include <QDataStream>

class Network : public QObject
{
    Q_OBJECT
public:
    Network();
    Network(const std::string address, const quint16 port);
    ~Network();

    void setHostAddress(const std::string address, const unsigned short port);
    void connectToHost() const;
    void disconnectFromHost() const;
    void sendToHost(std::string message) const;

public slots:
    void slotReadyRead();
    void displayError(QAbstractSocket::SocketError) const;

private:
    QString m_hostAddress;
    unsigned short m_hostPort;
    QSslSocket* m_sslSocket;
    quint16 m_blockSize;
    QByteArray m_data;
};
