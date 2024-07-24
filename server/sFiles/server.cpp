#include "../hFiles/server.h"
#include <QDebug>
#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include "../../protoDir/transMessg.pb.h"

Server::Server(char* _temp)
{

	if(!_temp) throw std::runtime_error(" need start key (int n) \n ./server (n) \n ");

	float value = atof(_temp);
	temp = static_cast<int>(value);
	if(value != temp) throw std::runtime_error(" invalid value ");

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
	using namespace transMssg;
	DataPock fromClient;

	QDataStream in(socket);
	in.setVersion(QDataStream::Qt_6_2);
  if(in.status() == QDataStream::Ok)
  {		
		Data = socket->readAll();
		fromClient.ParseFromArray(Data.data(), Data.size());

		qDebug() << "-- ready for read ";
		int sendInf = fromClient.value();
		int opr = fromClient.operand();

		qDebug() << "-- val1=" << sendInf
		         <<    "val2=" << temp;

		switch (opr) {
		  case 2:
			  sendInf += temp;
        break;
		  case 3:
			  sendInf -= temp;
        break;
		  case 4:
			  sendInf *= temp;
        break;
		  case 1:
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

void Server::SendToClient(int val)
{
	using namespace transMssg;
	DataPock somevalue;

	somevalue.set_value(val);
	std::string srlzString;
	somevalue.SerializeToString(&srlzString);

	Data.clear();
	Data = QByteArray(srlzString.c_str(), srlzString.length());

  socket->write(Data);
}

int Server::getPort()
{
	QByteArray dataConfig;
	{
		//-----------  config.json for server located in main dir ------------------
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
