#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>

class Server : public QTcpServer
{
	Q_OBJECT

public:
	Server(char* _temp);
	QTcpSocket * socket;

public slots:
	void incomingConnection(qintptr socketDescriptor);
	void slotReadyRead();

private:
	QByteArray Data;
	int temp;
	void SendToClient(int temp);
	int getPort();
};

#endif // SERVER_H
