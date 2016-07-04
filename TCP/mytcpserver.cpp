#include "mytcpserver.h"
#include "mytcpthread.h"
#include <iostream>

MyTCPServer::MyTCPServer(QObject *parent) :
    QTcpServer(parent)
{
}

void MyTCPServer::startServer()
{
    if(!this->listen(QHostAddress::Any, 5555))
    {
        std::cout << "Could not start server" << std::endl;
    }
    else
    {
        std::cout << "Listening to port " << 5555 << "..." << std::endl;
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

    // Every new connection will be run in a newly created thread
    MyTCPThread *thread = new MyTCPThread(socketDescriptor, this);

    // connect signal/slot
    // once a thread is not needed, it will be beleted later
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(readyRead(QString)), this, SLOT(readyReadSlot(QString)));
    connect(this, SIGNAL(write(QString)), thread, SLOT(write(QString)));

    thread->start();

    static int i;
    emit write(QString::number(i++));
}
