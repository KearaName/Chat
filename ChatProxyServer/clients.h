#ifndef CLIENTS_H
#define CLIENTS_H


#include <QtNetwork>
#include <QList>

class Clients
{

public:
    explicit Clients(QHostAddress IP, quint16 P, QString R)
    {
        IPAddress = IP;
        port = P;
        Room = R;
    }

    QHostAddress IPAddress;
    quint16 port;
    QString Room;
};

#endif // CLIENTS_H
