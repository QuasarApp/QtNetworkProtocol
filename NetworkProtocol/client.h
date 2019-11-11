#ifndef CLIENT_H
#define CLIENT_H

#include "basenode.h"

namespace NetworkProtocol {


class NETWORKPROTOCOLSHARED_EXPORT Client: public BaseNode
{
    Q_OBJECT
    Q_PROPERTY(int status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString lastMessage READ lastMessage WRITE setLastMessage NOTIFY lastMessageChanged)

public:
    enum Status {
        Offline,
        Online,
        Logined
    };

    explicit Client(const QHostAddress& address, unsigned short port);
    explicit Client(const QString& address, unsigned short port);

    bool connectClient();
    void setHost(const QHostAddress& address, unsigned short port);
    bool login(const QString& userMail, const QByteArray& rawPath);
    bool syncUserData();

    Q_INVOKABLE int status() const;
    Q_INVOKABLE QString lastMessage() const;

private slots:
    void handleIncomingData(QSharedPointer<AbstractData> obj, const QHostAddress &);
    void setLastMessage(QString lastMessage);
    void socketStateChanged(QAbstractSocket::SocketState);

private:
    Status _status = Offline;
    QHostAddress _address;
    unsigned short _port;

    QString _lastMessage;
    QSharedPointer<UserData> _user;

    void setStatus(Status);

signals:
    void statusChanged(int status);

    void lastMessageChanged(QString lastMessage);
};

}

#endif // CLIENT_H