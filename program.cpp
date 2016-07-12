#include "program.h"
#include <cmath>
#include <unistd.h>


Program::Program(QObject *parent) : QObject(parent)
{




//    mPendulumController.setPeriod(1000);
//    std::cout << "start" << std::endl;
    mPendulumController.start();
//    mPendulumController.startControlling();
//    for (int i=0; i<3; ++i){
//        std::cout << "Cart: " << mPendulumController.getCartPosition() << std::endl;
//        std::cout << "Pend: " << mPendulumController.getPendulumAngle() << std::endl;
//        sleep(1);
//    }
//    mPendulumController.quit();
//    mPendulumController.wait();
//    std::cout << "koniec" << std::endl;


    // create a timer

    // setup signal and slot
    connect(&mGuiRefreshTimer, SIGNAL(timeout()),this, SLOT(on_timeout()));

    connect(&mControllerTimer, SIGNAL(timeout()), this, SLOT(onControllerTimerTimeout()));

    connect(&mServer, SIGNAL(readyRead(QString)), this, SLOT(readyRead(QString)));
    connect(&mServer, SIGNAL(newConnection(qintptr)), this, SLOT(onNewConnection(qintptr)));
    mServer.startServer(5555);

}

Program::~Program()
{
   if (mPendulumController.isRunning())  {
       mPendulumController.quit();
       mPendulumController.wait();
   }

}

void Program::on_timeout()
{
    QString message = QString("ADDR %1 STATUS %2 %3 %4 %5 %6 ").arg(BROADCAST_ADRESS).
            arg(mPendulumController.getElapsedTime()).arg(mPendulumController.getCartPosition()).
            arg(mPendulumController.getCartSetpoint()).arg(mPendulumController.getPendulumAngle()).
            arg(mPendulumController.getControlValue());
    mServer.write(message);
    std::cout << QString::number(mPendulumController.getElapsedTime()).toStdString() << std::endl;
}


void Program::readyRead(QString message)
{
    std::cout << message.toStdString() << std::endl;
    QStringList tokens = message.split(" ",QString::SkipEmptyParts);
    QString response;
    if ( tokens.at(2).compare("CONTROL") == 0 ) {
        if (tokens.at(3).toInt() == ControlEnum::Take) {
            if (mUnderControl.testAndSetRelaxed(0, 1)) {
                mControllerAdress = tokens.at(1).toInt();
                message = QString("ADDR %1 CONTROL %2 ").arg(mControllerAdress).arg(ControlEnum::TakeSuccess);
                mServer.write(message);
                message = QString("ADDR %1 CONTROL %2 ").arg(BROADCAST_ADRESS).arg(ControlEnum::Taken);
                mServer.write(message);
                mControllerTime = 60;
                mControllerTimer.start(1000);
                onControllerTimerTimeout();
            } else {
                response = QString("ADDR %1 CONTROL %2 ").arg(mControllerAdress).arg(ControlEnum::TakeFail);
                mServer.write(response);
            }
        } else if (tokens.at(3).toInt() == ControlEnum::GiveUp) {
            mUnderControl = 0;
            mControllerTimer.stop();
            response = QString("ADDR %1 CONTROL %2 ").arg(BROADCAST_ADRESS).arg(ControlEnum::Free);
            mServer.write(response);
        } else if (tokens.at(3).toInt() == ControlEnum::Prolong) {
            prolongControllerTime();
        }

    } else if ( tokens.at(2).compare("PARAMS") == 0 ) {
        std::map<std::string, float> params;
        params["Kp"] = tokens.at(3).toFloat();
        params["Ki"] = tokens.at(4).toFloat();
        params["Kd"] = tokens.at(5).toFloat();
        params["N"] = tokens.at(6).toInt();
        int tempHz = tokens.at(11).toInt();
        params["Ts"] = std::round( (1.f/tempHz) * 1000000 );
        mPendulumController.setCartPIDParams(params);
        params["Kp"] = tokens.at(7).toFloat();
        params["Ki"] = tokens.at(8).toFloat();
        params["Kd"] = tokens.at(9).toFloat();
        params["N"] = tokens.at(10).toInt();
        params["Ts"] = std::round( (1.f/tempHz) * 1000000 );
        mPendulumController.setPendulumPIDParams(params);
        std::map<std::string, float> cartParams = mPendulumController.getCartPIDParams();
        std::map<std::string, float> pendulumParams = mPendulumController.getPendulumPIDParams();
        mPendulumController.setSamplingFrequency(tokens.at(11).toInt());

        response = QString("ADDR %1 PARAMS %2 %3 %4 %5 %6 %7 %8 %9 %10 ").arg(BROADCAST_ADRESS).
                arg(cartParams["Kp"]).arg(cartParams["Ki"]).arg(cartParams["Kd"]).arg(cartParams["N"]).
                arg(pendulumParams["Kp"]).arg(pendulumParams["Ki"]).arg(pendulumParams["Kd"]).arg(pendulumParams["N"]).
                arg(mPendulumController.getSamplingFrequency());
        mServer.write(response);

    } else if ( tokens.at(2).compare("SETPOINT") == 0 ) {
        mPendulumController.setCartSetpoint(tokens.at(3).toInt()/100.f);
        response = QString("ADDR %1 SETPOINT %2 ").arg(BROADCAST_ADRESS).arg(mPendulumController.getCartSetpoint());
        mServer.write(response);

    } else if ( tokens.at(2).compare("START") == 0 ) {
        mPendulumController.startControlling();
        mGuiRefreshTimer.start(50);
        mIsStarted = true;
        response = QString("ADDR %1 STARTED ").arg(BROADCAST_ADRESS);
        mServer.write(response);
        std::cout << "STARTED" << std::endl;

    } else if ( tokens.at(2).compare("STOP") == 0 ) {
        mPendulumController.stopControlling();
        mGuiRefreshTimer.stop();
        mIsStarted = false;
        response = QString("ADDR %1 STOPPED ").arg(BROADCAST_ADRESS);
        mServer.write(response);
        std::cout << "STOPPED" << std::endl;

    }


    if (message.count("ADDR") > 1) {
        readyRead(message.mid(message.indexOf("ADDR", 5)));
    }
}

void Program::onNewConnection(qintptr clientAdress)
{
    QString message = QString("ADDR %1 CONNECTED CONTROL %2 STARTED %3 ").arg(clientAdress).arg(mUnderControl).arg(mIsStarted);
    mServer.write(message);
    std::map<std::string, float> cartParams = mPendulumController.getCartPIDParams();
    std::map<std::string, float> pendulumParams = mPendulumController.getPendulumPIDParams();
    message = QString("ADDR %1 PARAMS %2 %3 %4 %5 %6 %7 %8 %9 %10 ").arg(clientAdress).
            arg(cartParams["Kp"]).arg(cartParams["Ki"]).arg(cartParams["Kd"]).arg(cartParams["N"]).
            arg(pendulumParams["Kp"]).arg(pendulumParams["Ki"]).arg(pendulumParams["Kd"]).arg(pendulumParams["N"]).
            arg(mPendulumController.getSamplingFrequency());
    mServer.write(message);
}

void Program::onControllerTimerTimeout()
{
    if (mControllerTime) {
        mControllerTime--;
    } else {
        mControllerTimer.stop();
        mUnderControl = 0;
        QString message = QString("ADDR %1 CONTROL %2 ").arg(BROADCAST_ADRESS).arg(ControlEnum::Free);
        mServer.write(message);
    }
}

void Program::prolongControllerTime()
{
    mControllerTime = 60;
    onControllerTimerTimeout();
    mControllerTimer.stop();
    mControllerTimer.start(1000);
}


