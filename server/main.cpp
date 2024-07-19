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


  long id;
  errno = 0;
  id = syscall(SYS_gettid);
  if(0 != errno)
  {
      perror("syscall(SYS_gettid) failed");
      abort();
  }
  fprintf(stdout, "thread id %li", id);
Server b;
  return a.exec();
}
