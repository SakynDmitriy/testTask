#include "../hFiles/server.h"
#include <QDebug>
#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

Server::Server(char* _temp)
{

	if(!_temp) throw std::runtime_error(" need start key (int n) \n ./server (n) \n ");

	float value = atof(_temp);
	if(value != static_cast<int>(value)) throw std::runtime_error(" invalid value ");

	if(!this->listen(QHostAddress::Any, getPort()))
	{
		qDebug() << "server is not started";
	}
	else
	{
		qDebug() << "-- server is started ";
	}
}

void Server::incomingConnection(qintptr socketDescriptor)
{
  socket = new QTcpSocket;
  socket->setSocketDescriptor(socketDescriptor);
  connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
  connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

  qDebug() << "-- client №" << socketDescriptor;
}

void Server::slotReadyRead()
{
  socket = (QTcpSocket*)sender();
  QDataStream in(socket);
  in.setVersion(QDataStream::Qt_6_2);
  if(in.status() == QDataStream::Ok)
  {
    qDebug() << "-- ready for read ";
		int sendInf;
    char opr;
		in>>sendInf>>opr;
		qDebug() << "-- val1=" << sendInf
		         <<    "val2=" << temp;

    qDebug() << "-- operation=" << temp
             << opr
		         << sendInf;
    switch (opr) {
      case '+':
			  sendInf += temp;
        break;
      case '-':
			  sendInf -= temp;
        break;
      case '*':
			  sendInf *= temp;
        break;
      case '/':
			  sendInf /= temp;
        break;

      default:
        break;
      }

		SendToClient(sendInf);
    }
  else
  {
    qDebug()<<"error";
  }
}

void Server::SendToClient(int temp)
{
  Data.clear();
  QDataStream out(&Data, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_6_2);
  out<<temp;
  socket->write(Data);
}

int Server::getPort()
{
	QByteArray dataConfig;
	{
		QString jsonPath = "config.json";
		QFile config(jsonPath);
		if(!config.open(QIODevice::ReadOnly | QIODevice::Text)) throw std::runtime_error(" couldn't open file ");
		dataConfig = config.readAll();
		config.close();
	}

	QJsonDocument config = QJsonDocument::fromJson(dataConfig);
	QJsonObject port = config.object();

	return port.value("port").toInt();
}
