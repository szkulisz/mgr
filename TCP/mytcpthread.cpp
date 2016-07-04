#include "mytcpthread.h"
#include <pthread.h>
#include <QThread>
#include <iostream>

#define CHECK(sts,msg)  \
    if (sts == -1) {      \
    perror(msg);        \
    exit(-1);           \
  }

MyTCPThread::MyTCPThread(qintptr ID, QObject *parent) :
    QThread(parent),
    mSocketDescriptor(ID)
{
}

void MyTCPThread::run()
{
    // thread starts here
    int sts;
    struct sched_param param;
    sts = sched_getparam(0, &param);
    CHECK(sts,"sched_getparam");
    param.sched_priority = 0;
    sts = sched_setscheduler(0, SCHED_OTHER, &param);
    CHECK(sts,"sched_setscheduler");
    std::cout << "MyTCPThread has ID: " << QThread::currentThreadId() << " and priority: " << param.sched_priority << std::endl;

    mSocket = new QTcpSocket();

    // set the ID
    if(!mSocket->setSocketDescriptor(mSocketDescriptor))
    {
        // something's wrong, we just emit a signal
        emit error(mSocket->error());
        return;
    }

    // connect socket and signal
    // note - Qt::DirectConnection is used because it's multithreaded
    //        This makes the slot to be invoked immediately, when the signal is emitted.

    connect(mSocket, SIGNAL(readyRead()), this, SLOT(readyReadSlot())/*, Qt::DirectConnection*/);
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    // We'll have multiple clients, we want to know which is which
     std::cout << mSocketDescriptor << " Client connected" << std::endl;

    // make this thread a loop,
    // thread will stay alive so that signal/slot to function properly
    // not dropped out in the middle when thread dies

     exec();
}

void MyTCPThread::write(QString message)
{
    std::cout << "MyTCPThread has ID: " << QThread::currentThreadId() << std::endl;
    mSocket->write(message.toLocal8Bit());
}

void MyTCPThread::readyReadSlot()
{
    // get the information
    QString message = mSocket->readAll();

    emit readyRead(message);
}

void MyTCPThread::disconnected()
{
     std::cout << mSocketDescriptor << " Disconnected" << std::endl;


    mSocket->deleteLater();
    exit(0);
}
