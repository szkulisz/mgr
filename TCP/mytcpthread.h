#ifndef MYTCPTHREAD_H
#define MYTCPTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QDebug>

class MyTCPThread : public QThread
{
    Q_OBJECT
public:
    explicit MyTCPThread(qintptr ID, QObject *parent = 0);

    void run();

signals:
    void error(QTcpSocket::SocketError socketError);
    void readyRead(QString);

public slots:
    void write(QString message);
    void readyReadSlot();
    void disconnected();

private:
    QTcpSocket *mSocket;
    qintptr mSocketDescriptor;
};

#endif // MYTCPTHREAD_H
