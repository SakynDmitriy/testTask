#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server();
    QTcpSocket * socket;

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();

private:
    QByteArray Data;
    void SendToClient(int temp);
};

#endif // SERVER_H
