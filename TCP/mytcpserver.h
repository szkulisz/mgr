#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>

class MyTCPServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyTCPServer(QObject *parent = 0);
    void startServer(int port);
    void send(QString);

signals:
    void readyRead(QString);
    void write(QString);
    void newConnection(qintptr);

public slots:
    void readyReadSlot(QString message);

protected:
    void incomingConnection(qintptr socketDescriptor);

};

#endif // MYTCPSERVER_H
