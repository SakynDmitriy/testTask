#include "../hFiles/server.h"
#include <QDebug>

#include <QCoreApplication>
Server::Server()
{
    if(!this->listen(QHostAddress::Any, 6000)){
        qDebug() << "server is not started";
    } else {
        qDebug() << "-- server is started";
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
    int temp, temp2;
    char opr;
    in>>temp>>temp2>>opr;
    qDebug() << "-- val1=" << temp
             << "val2="   << temp2;

    qDebug() << "-- operation=" << temp
             << opr
             << temp2;

    switch (opr) {
      case '+':
        temp += temp2;
        break;
      case '-':
        temp -= temp2;
        break;
      case '*':
        temp *= temp2;
        break;
      case '/':
        temp /= temp2;
        break;

      default:
        break;
      }

    SendToClient(temp);
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
