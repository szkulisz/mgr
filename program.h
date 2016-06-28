#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>
#include <QTimer>
#include <iostream>
#include "HARDWARE/AD5313_DAC/ad5313.h"
#include "HARDWARE/MCP23S17/mcp23s17.h"
#include "HARDWARE/encoder.h"
#include "HARDWARE/GPIO/GPIO.h"
#include "petla.h"
#include "regulatory/regulatorpid.h"
#include <map>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTcpSocket>
#include <QTcpServer>
#include <QCoreApplication>
#include <QDebug>
#include <QVector>
#include "controller.h"

class Program : public QObject
{
    Q_OBJECT
public:
    explicit Program(QObject *parent = 0);
    QTimer *timer;
    QSqlDatabase dataBase;
    QTcpServer *server;
    QTcpSocket *clientConnection;

    Petla *petla;
signals:

public slots:
    void on_timeout();
    void newConnection();
//    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    Controller mPendulumController;
};

#endif // PROGRAM_H
