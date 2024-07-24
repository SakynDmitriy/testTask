#include <QCoreApplication>
#include "hFiles/server.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
	Server b(argv[1]);
  return a.exec();
}
