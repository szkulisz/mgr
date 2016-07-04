#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>

class MyTCPServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyTCPServer(QObject *parent = 0);
    void startServer();
    void send(QString);

signals:
    void readyRead(QString);
    void write(QString);

public slots:
    void readyReadSlot(QString message);

protected:
    void incomingConnection(qintptr socketDescriptor);

};

#endif // MYTCPSERVER_H
