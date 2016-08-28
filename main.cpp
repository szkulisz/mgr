#include <QCoreApplication>
#include "program.h"
#include <pthread.h>
#include <QThread>

#define CHECK(sts,msg)  \
    if (sts == -1) {      \
    perror(msg);        \
    exit(-1);           \
  }


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    system("/home/szymon/mgr/init.sh");
    system("config-pin P9.28 spi");
    system("config-pin P9.29 spi");
    system("config-pin P9.30 spi");
    system("config-pin P9.31 spi");
    system("config-pin P9.42 spics");

    int sts;
    struct sched_param param;
    sts = sched_getparam(0, &param);
    CHECK(sts,"sched_getparam");
    param.sched_priority = (sched_get_priority_max(SCHED_FIFO)-1);
    sts = sched_setscheduler(0, SCHED_FIFO, &param);
    CHECK(sts,"sched_setscheduler");
    std::cout << "Process has ID: " << QThread::currentThreadId() << " and priority: " << param.sched_priority << std::endl;

    Program program;

    return a.exec();
//    return 0;
}



