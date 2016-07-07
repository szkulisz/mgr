#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>
#include <QTimer>
#include <QAtomicInt>
#include <iostream>
#include "HARDWARE/AD5313_DAC/ad5313.h"
#include "HARDWARE/MCP23S17/mcp23s17.h"
#include "HARDWARE/encoder.h"
#include "HARDWARE/GPIO/GPIO.h"
#include <map>
#include <QCoreApplication>
#include <QVector>
#include "controller.h"
#include "TCP/mytcpserver.h"

enum ControlEnum {
    Taken,
    Free,
    Take,
    TakeSuccess,
    TakeFail,
    GiveUp,
    Prolong
};

class Program : public QObject
{
    Q_OBJECT
public:
    explicit Program(QObject *parent = 0);


signals:

public slots:
    void on_timeout();
    void readyRead(QString message);
    void onNewConnection(qintptr clientAdress);
    void onControllerTimerTimeout();

private:
    Controller mPendulumController;
    QAtomicInt mUnderControl = 0;
    MyTCPServer mServer;
    int mControllerAdress;
    const int BROADCAST_ADRESS = -10;
    QTimer mControllerTimer;
    int mControllerTime;
    QTimer mGuiRefreshTimer;

    void prolongControllerTime();
};

#endif // PROGRAM_H
