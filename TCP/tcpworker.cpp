#include "tcpworker.h"
#include <pthread.h>
#include <QThread>
#include <iostream>

#define CHECK(sts,msg)  \
    if (sts == -1) {      \
    perror(msg);        \
    exit(-1);           \
  }

TCPWorker::TCPWorker(qintptr ID, QObject *parent) :
    QObject(parent),
    mSocketDescriptor(ID)
{


}

void TCPWorker::write(QString message)
{
//    std::cout << "TCPWorker writes in: " << QThread::currentThreadId() << std::endl;
    mSocket->write(message.toLocal8Bit());
}

void TCPWorker::readyReadSlot()
{
//    std::cout << "TCPWorker reads in: " << QThread::currentThreadId() << std::endl;
    // get the information
    QString message = mSocket->readAll();

    emit readyRead(message);
}

void TCPWorker::disconnected()
{
   std::cout << mSocketDescriptor << " Disconnected" << std::endl;

   mSocket->deleteLater();
   emit finished();
}

void TCPWorker::onThreadStart()
{
    // thread starts here
    int sts;
    struct sched_param param;
    sts = sched_getparam(0, &param);
    CHECK(sts,"sched_getparam");
    param.sched_priority = 0;
    sts = sched_setscheduler(0, SCHED_OTHER, &param);
    CHECK(sts,"sched_setscheduler");
//    std::cout << "TCPWorker runs in: " << QThread::currentThreadId() << " with priority: " << param.sched_priority << std::endl;

    mSocket = new QTcpSocket();

    // set the ID
    if(!mSocket->setSocketDescriptor(mSocketDescriptor))
    {
        // something's wrong, we just emit a signal
        emit error(mSocket->error());
        return;
    }

    // connect socket and signal
    connect(mSocket, SIGNAL(readyRead()), this, SLOT(readyReadSlot())/*, Qt::DirectConnection*/);
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    // We'll have multiple clients, we want to know which is which
    std::cout << mSocketDescriptor << " Client connected" << std::endl;
}

