#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

public slots:
  void slotReadyRead();

private slots:
  void on_pushButton_2_clicked();
  void on_pushButton_3_clicked();

  void on_radioButton_clicked();
  void on_radioButton_2_clicked();
  void on_radioButton_3_clicked();
  void on_radioButton_4_clicked();

private:
	int oprType;
  Ui::MainWindow *ui;
  QTcpSocket *socket;
  QByteArray Data;
	std::pair<QString, int> getConfigInfo();
	void sendToServer(int temp, int opr);
};
#endif // CLIENT_H
