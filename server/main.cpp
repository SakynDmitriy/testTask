#include <QCoreApplication>
#include "hFiles/server.h"

#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
	Server b(argv[1]);
  return a.exec();
}
