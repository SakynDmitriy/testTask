#include "../hFiles/client.h"
#include "./ui_client.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextEdit>
#include "../../protoDir/transMessg.pb.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  socket = new QTcpSocket(this);
	oprType = 0;
  connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
  connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::slotReadyRead()
{
	Data = socket->readAll();
	using namespace transMssg;
	DataPock somevalue;
	somevalue.ParseFromArray(Data.data(), Data.size());
	ui->textBrowser->append(QString::number(somevalue.value()));
}

void MainWindow::sendToServer(int temp, int opr)
{
	using namespace transMssg;
	DataPock toServer;
	toServer.set_value(temp);
	toServer.set_operand(opr);
	std::string srlzString;
	toServer.SerializeToString(&srlzString);
	ui->textEdit->clear();
  ui->textBrowser->clear();
  Data.clear();
  QDataStream out(&Data, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_6_2);
	Data = QByteArray(srlzString.c_str(), srlzString.length());
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

void MainWindow::on_pushButton_2_clicked()
{
	std::pair<QString, int> ip_port = getConfigInfo();
	socket->connectToHost(ip_port.first, ip_port.second);
}


void MainWindow::on_pushButton_3_clicked()
{
	float value= ui->textEdit->toPlainText().toFloat();
	if(oprType != 0 && value == ui->textEdit->toPlainText().toInt())
	{
		sendToServer(ui->textEdit->toPlainText().toInt(), oprType);
	}
	else
	{
		ui->textBrowser->append(" value is float ");
		ui->textBrowser->append(" choose the operation ");
	}
}

void MainWindow::on_radioButton_2_clicked()
{
	oprType = 2;
}

void MainWindow::on_radioButton_4_clicked()
{
	oprType = 4;
}

void MainWindow::on_radioButton_clicked()
{
	oprType = 1;
}

void MainWindow::on_radioButton_3_clicked()
{
	oprType = 3;
}
