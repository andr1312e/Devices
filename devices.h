#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>
#include <QThread>
#include <QCoreApplication>
#include <QDir>

#include "sockets/rarmsocket.h"

#include "mediators/moxamediator.h"
#include "mediators/meteomediator.h"
#include "mediators/ustirovmediator.h"
#include "sockets/geosocket.h"
#include "mediators/consolemediator.h"
#include "mediators/logger.h"

class Devices : public QObject
{
    Q_OBJECT
public:
    explicit Devices();
    ~Devices();
private:
    void CreateObjects();
    void PrepareObjects();
    void ConnectObjects();
    void StartObjects();
private:
    const QString m_rarmAdress;
    const quint16 m_rarmPort;
private:
    const Logger *m_logger;
    RarmSocket *m_rarmSocket;
    MoxaMediator *m_moxaMediator;
    MeteoMediator *m_meteoMediator;
    UstirovMediator *m_ustirovMediator;
    GeoSocket *m_geoSocket;

    ConsoleMediator *m_consoleMediator;
    QThread *m_consoleThread;
};

#endif // PROGRAM_H
