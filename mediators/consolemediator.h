#ifndef CONSOLEMEDIATOR_H
#define CONSOLEMEDIATOR_H
#include <QObject>


#include <cstdlib>
#include <iostream>

#include "sockets/rarmsocket.h"
#include "mediators/moxamediator.h"
#include "mediators/ustirovmediator.h"
#include "mediators/meteomediator.h"
#include "sockets/geosocket.h"

class ConsoleMediator: public QObject
{
public:
    ConsoleMediator(const std::string &logFilePath, RarmSocket *socket, MoxaMediator *moxa, UstirovMediator *ustirov, MeteoMediator *meteo, GeoSocket *geo, QObject *parent);
    ~ConsoleMediator();
public Q_SLOTS:
    void OnStartLog();
protected:
    virtual void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
private:
    void ClearConsole();
    void ToPrintLogFilePath();
    void PrintProcess();
    void PrintRarmState();
    void PrintMoxaState();
    void PrintUkitState();
    void PrintMeteoState();
    void PrintGeoState();
private:
    const std::string m_logFilePath;
private:
    const RarmSocket *const m_rarmSocket;
    const MoxaMediator *const m_moxa;
    const UstirovMediator * const m_ustirov;
    const MeteoMediator * const m_meteo;
    const GeoSocket * const m_geo;
private:
    int m_updateIterval=500;
    std::string m_progress;
    int m_iteration=0;
};

#endif // CONSOLEMEDIATOR_H
