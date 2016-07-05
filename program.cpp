#include "program.h"
#include <unistd.h>


Program::Program(QObject *parent) : QObject(parent)
{




    mPendulumController.setPeriod(1000);
    std::cout << "start" << std::endl;
    mPendulumController.start();
    mPendulumController.startController();
    for (int i=0; i<1; ++i){
        std::cout << "Cart: " << mPendulumController.getCartPosition() << std::endl;
        std::cout << "Pend: " << mPendulumController.getPendulumAngle() << std::endl;
        sleep(1);
    }
//    mPendulumController.quit();
//    mPendulumController.wait();
    std::cout << "koniec" << std::endl;


    // create a timer
    timer = new QTimer(this);

    // setup signal and slot
    connect(timer, SIGNAL(timeout()),
          this, SLOT(on_timeout()));


    connect(&mServer, SIGNAL(readyRead(QString)), this, SLOT(readyRead(QString)));
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
//    QString message;// = clientConnection->readAll();
    std::cout << message.toStdString() << std::endl;
    QStringList tokens = message.split(" ",QString::SkipEmptyParts);

    if ( tokens.at(0).compare("START") == 0 ) {
        std::cout << tokens.at(0).toStdString() << std::endl;
        timer->start(10);
    }
    if ( tokens.at(0).compare("STOP") == 0 ) {
        timer->stop();
        std::cout << tokens.at(0).toStdString() << std::endl;
    }
    if ( tokens.at(0).compare("SETPOINT") == 0 ) {
        int SP = tokens.at(1).toInt();
    }
    if ( tokens.at(0).compare("PARAMS") == 0 ) {
        std::map<std::string, float> parametry;
        float K = tokens.at(2).toFloat();
        float Ti = tokens.at(4).toFloat();
        float Td = tokens.at(6).toFloat();
        parametry["k"] = K;
        parametry["Ti"] = Ti;
        parametry["Td"] = Td;
        parametry["Tp"] = 0.01f;
        parametry["b"] = 1;
        parametry["N"] = 1;
        parametry["ogr"] = 1;
        parametry["max"] = 0.5f;
        parametry["min"] = -0.5f;
    }
}


