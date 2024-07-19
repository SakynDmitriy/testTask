#include "../hFiles/client.h"
#include "./ui_client.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  socket = new QTcpSocket(this);
  oprType = ' ';
  connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
  connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::slotReadyRead()
{
  QDataStream in(socket);
  in.setVersion(QDataStream::Qt_6_2);
  if(in.status()==QDataStream::Ok)
  {
    int temp = 0;
    in >> temp;
    ui->textBrowser->append(QString::number(temp));
  }
}


void MainWindow::on_pushButton_2_clicked()
{
  socket->connectToHost("127.0.0.1", 6000);
}


void MainWindow::on_pushButton_3_clicked()
{
  if(oprType != ' ')
  {
    sendToServer(ui->textEdit->toPlainText().toFloat(),
                 ui->textEdit_2->toPlainText().toFloat(), oprType);
  }
  else
  {
    ui->textBrowser->append(" choose the operation ");
  }
}

void MainWindow::sendToServer(int temp, int temp2, char opr)
{
  ui->textBrowser->clear();
  Data.clear();
  QDataStream out(&Data, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_6_2);
  out<<temp<<temp2<<opr;
  socket->write(Data);
}

void MainWindow::on_radioButton_2_clicked()
{
  oprType = '+';
}

void MainWindow::on_radioButton_4_clicked()
{
  oprType = '*';
}

void MainWindow::on_radioButton_clicked()
{
  oprType = '/';
}

void MainWindow::on_radioButton_3_clicked()
{
  oprType = '-';
}
