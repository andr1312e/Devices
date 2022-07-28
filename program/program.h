#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>
//#include <QThread>
#include <QSharedMemory>
#include <QCoreApplication>

#include "sockets/rarmsocket.h"

#include "mediators/moxamediator.h"
#include "mediators/meteomediator.h"
#include "mediators/ustirovmediator.h"
#include "sockets/geosocket.h"

class Program : public QCoreApplication
{
    Q_OBJECT
public:
    explicit Program(int &argc, char **argv);
    ~Program();
private:
    bool HasNoRunningInstances();
    void CreateObjects();
    void ConnectObjects();
private:
    QSharedMemory m_sharedMemory;
    const QString m_rarmAdress;
    const quint16 m_rarmPort;
    RarmSocket *m_rarmSocket;

    MoxaMediator *m_moxaMediator;
    MeteoMediator *m_meteoMediator;
    UstirovMediator *m_ustirovMediator;
    GeoSocket *m_geoSocket;
};

#endif // PROGRAM_H
