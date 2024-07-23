#include "../hFiles/client.h"
#include "./ui_client.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextEdit>

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
	std::pair<QString, int> ip_port = getConfigInfo();
	socket->connectToHost(ip_port.first, ip_port.second);
}


void MainWindow::on_pushButton_3_clicked()
{
	float value= ui->textEdit->toPlainText().toFloat();
	if(oprType != ' ' && value == ui->textEdit->toPlainText().toInt())
  {
		sendToServer(ui->textEdit->toPlainText().toInt(), oprType);
  }
  else
  {
		ui->textBrowser->append(" value is float ");
    ui->textBrowser->append(" choose the operation ");
  }
}

void MainWindow::sendToServer(int temp, char opr)
{
  ui->textBrowser->clear();
  Data.clear();
  QDataStream out(&Data, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_6_2);
	out<<temp<<opr;
  socket->write(Data);
}

std::pair<QString, int> MainWindow::getConfigInfo()
{
	QByteArray dataConfig;
	{
		QFile config("config.json");
		if(!config.open(QIODevice::ReadOnly | QIODevice::Text)) throw std::runtime_error(" couldn't open file ");

		dataConfig = config.readAll();
		config.close();
	}

	QJsonDocument config = QJsonDocument::fromJson(dataConfig);
	QJsonObject ip_port = config.object();

	qDebug()<<ip_port.value("ip").toString()<<ip_port.value("port").toInt();

	return std::make_pair(ip_port.value("ip").toString(),
	                      ip_port.value("port").toInt());
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
