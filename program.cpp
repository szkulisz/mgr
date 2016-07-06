#include "program.h"
#include <unistd.h>


Program::Program(QObject *parent) : QObject(parent)
{




//    mPendulumController.setPeriod(1000);
//    std::cout << "start" << std::endl;
//    mPendulumController.start();
//    mPendulumController.startController();
//    for (int i=0; i<1; ++i){
//        std::cout << "Cart: " << mPendulumController.getCartPosition() << std::endl;
//        std::cout << "Pend: " << mPendulumController.getPendulumAngle() << std::endl;
//        sleep(1);
//    }
//    mPendulumController.quit();
//    mPendulumController.wait();
//    std::cout << "koniec" << std::endl;


    // create a timer
    timer = new QTimer(this);

    // setup signal and slot
    connect(timer, SIGNAL(timeout()),
          this, SLOT(on_timeout()));

    connect(&mControllerTimer, SIGNAL(timeout()), this, SLOT(onControllerTimerTimeout()));

    connect(&mServer, SIGNAL(readyRead(QString)), this, SLOT(readyRead(QString)));
    connect(&mServer, SIGNAL(newConnection(qintptr)), this, SLOT(onNewConnection(qintptr)));
    mServer.startServer(5555);

}

void Program::on_timeout()
{
    static int i = 0;
    static int counter = 0;
//    QString message = QString("ID %1 CH %2 PE %3 CV %4").arg(counter++).arg(petla->odczytaj_wyjscie_obiektu()).arg(petla->wahadlo->getPendulumAngle()).arg(petla->odczytaj_sterowanie());
//    clientConnection->write(message.toLocal8Bit());
//    if ( i++ == 100) {
//        std::cout << "id: " << counter-1
//                << " chart: " << petla->odczytaj_wyjscie_obiektu()
//                << " pend: " << petla->wahadlo->getPendulumAngle()
//              << " ster: " << petla->odczytaj_sterowanie() << std::endl;
//        i = 0;
//    }
}


void Program::readyRead(QString message)
{
    std::cout << message.toStdString() << std::endl;
    QStringList tokens = message.split(" ",QString::SkipEmptyParts);

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
                message = QString("ADDR %1 CONTROL %2 ").arg(mControllerAdress).arg(ControlEnum::TakeFail);
                mServer.write(message);
            }
        } else if (tokens.at(3).toInt() == ControlEnum::GiveUp) {
            mUnderControl = 0;
            message = QString("ADDR %1 CONTROL %2 ").arg(BROADCAST_ADRESS).arg(ControlEnum::Free);
            mServer.write(message);
        }

    }
}

void Program::onNewConnection(qintptr clientAdress)
{
    QString message = QString("ADDR %1 CONNECTED CONTROL %2 ").arg(clientAdress).arg(mUnderControl);
    std::cout << message.toStdString() << std::endl;
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


