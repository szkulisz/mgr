#include "program.h"


Program::Program(QObject *parent) : QObject(parent)
{

    std::map<std::string, float> parametry;
    parametry["k"] = 0.0003f;
    parametry["Ti"] = 0.3857f;
    parametry["Td"] = 0.0052f;
    parametry["Tp"] = 0.01f;
    parametry["b"] = 1;
    parametry["N"] = 1;
    parametry["ogr"] = 1;
    parametry["max"] = 0.5f;
    parametry["min"] = -0.5f;

    petla = new Petla( new RegulatorPID(parametry));
    petla->wahadlo->steruj(0.f);
    petla->ustaw_wartosc_zadana(0.f);
//    petla->ustaw_wartosc_zadana(new Sygnal_prost(1000, 500, 0.5, new Sygnal_0));

    // create a timer
    timer = new QTimer(this);

    // setup signal and slot
    connect(timer, SIGNAL(timeout()),
          this, SLOT(on_timeout()));

    server = new QTcpServer(this);

    // whenever a user connects, it will emit signal
        connect(server, SIGNAL(newConnection()),
                this, SLOT(newConnection()));
    if(!server->listen(QHostAddress::Any, 5555))
    {
//        qDebug() << "Server could not start";
    }
    else
    {
        //qDebug() << "Server started!";
    }

//    dataBase = QSqlDatabase::addDatabase("QMYSQL");
//    dataBase.setHostName("localhost");//192.168.1.212");
//    dataBase.setPort(3306);
//    dataBase.setUserName("mgr");
//    dataBase.setPassword("mgrr");
//    dataBase.setDatabaseName("mgr");
//    if (!dataBase.open()) {
//        std::cout << "Cannot connect with database..." <<std::endl;
//        std::cout << dataBase.lastError().text().toStdString() <<std::endl;
//    }
//    QSqlQuery query;
//    query.exec("truncate table ver2");

    // msec
//    timer->start(10);
}

void Program::on_timeout()
{
    static int i = 0;
    static int counter = 0;
    static QSqlQuery query;
    petla->krok();
//    query.prepare("INSERT INTO ver2 (ID, Chart, Pend, CV) VALUES (:id, :chart, :pend, :CV)");
//    query.bindValue(":id", counter++);
//    query.bindValue(":chart", petla->odczytaj_wyjscie_obiektu());
//    query.bindValue(":pend", petla->wahadlo->getS_pozycja_wahadla());
//    query.bindValue(":CV", petla->odczytaj_sterowanie());
//    query.exec();
    QString message = QString("ID %1 CH %2 PE %3 CV %4").arg(counter++).arg(petla->odczytaj_wyjscie_obiektu()).arg(0).arg(petla->odczytaj_sterowanie());
    clientConnection->write(message.toLocal8Bit());
    if ( i++ == 100) {
        std::cout << "id: " << counter-1
                << " chart: " << petla->odczytaj_wyjscie_obiektu()
                << " pend: " << petla->wahadlo->getS_pozycja_wahadla()
              << " ster: " << petla->odczytaj_sterowanie() << std::endl;
        i = 0;
    }
}

void Program::newConnection()
{
    std::cout << "NEW CONNECTION!" << std::endl;
    clientConnection = server->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));
    connect(clientConnection, SIGNAL(bytesWritten(qint64)),
            this, SLOT(bytesWritten(qint64)));
    connect(clientConnection, SIGNAL(readyRead()),
            this, SLOT(readyRead()));
}

void Program::bytesWritten(qint64 bytes)
{

}

void Program::readyRead()
{
    QString message = clientConnection->readAll();
    QStringList tokens = message.split(" ",QString::SkipEmptyParts);

    if ( tokens.at(0).compare("START") == 0 ) {
        std::cout << tokens.at(0).toStdString() << std::endl;
        timer->start(10);
    }
    if ( tokens.at(0).compare("STOP") == 0 ) {
        timer->stop();
        petla->wahadlo->steruj(0.f);
        std::cout << tokens.at(0).toStdString() << std::endl;
    }
    if ( tokens.at(0).compare("SETPOINT") == 0 ) {
        int SP = tokens.at(1).toInt();
        petla->ustaw_wartosc_zadana(SP);
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
        petla->ustaw_parametry_regulatora( parametry );
    }
}

void Program::logToDatabase(QVector<int> chart, QVector<int> pend, QVector<float> CV)
{

}

