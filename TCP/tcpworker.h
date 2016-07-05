#ifndef TCPWORKER_H
#define TCPWORKER_H

#include <QObject>
#include <QTcpSocket>

class TCPWorker : public QObject
{
    Q_OBJECT
public:
    explicit TCPWorker(qintptr ID, QObject *parent = 0);

signals:
    void error(QTcpSocket::SocketError socketError);
    void readyRead(QString);
    void finished();

public slots:
    void write(QString message);
    void readyReadSlot();
    void disconnected();
    void onThreadStart();

private:
    QTcpSocket *mSocket;
    qintptr mSocketDescriptor;
};

#endif // TCPWORKER_H
