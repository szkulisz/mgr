#include "mytcpserver.h"
#include "tcpworker.h"
#include <iostream>
#include <QThread>

MyTCPServer::MyTCPServer(QObject *parent) :
    QTcpServer(parent)
{
}

void MyTCPServer::startServer(int port)
{
    if(!this->listen(QHostAddress::Any, port))
    {
        std::cout << "Could not start server" << std::endl;
    }
    else
    {
        std::cout << "Listening to port " << port << "..." << std::endl;
    }
}

void MyTCPServer::readyReadSlot(QString message)
{
    emit readyRead(message);
}

// This function is called by QTcpServer when a new connection is available.
void MyTCPServer::incomingConnection(qintptr socketDescriptor)
{
    // We have a new connection

    QThread *thread = new QThread;
    TCPWorker *worker = new TCPWorker(socketDescriptor);
    worker->moveToThread(thread);
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(started()), worker, SLOT(onThreadStart()));
    connect(worker,SIGNAL(readyRead(QString)), this, SLOT(readyReadSlot(QString)));
    connect(this,SIGNAL(write(QString)), worker, SLOT(write(QString)));

    thread->start();

    emit newConnection(socketDescriptor);
}
