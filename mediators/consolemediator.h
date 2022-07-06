#ifndef MEDIATORS_CONSOLEMEDIATOR_H
#define MEDIATORS_CONSOLEMEDIATOR_H
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
    explicit ConsoleMediator(const std::string &logFilePath, RarmSocket *socket, MoxaMediator *moxa, UstirovMediator *ustirov, MeteoMediator *meteo, GeoSocket *geo, QObject *parent);
    ~ConsoleMediator();
public Q_SLOTS:
    void OnStartLog();
protected:
    virtual void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
private:
    void ClearConsole() noexcept;
    void ToPrintLogFilePath() noexcept;
    void PrintProcess() noexcept;
    void PrintRarmState() noexcept;
    void PrintMoxaState() noexcept;
    void PrintUkitState() noexcept;
    void PrintMeteoState() noexcept;
    void PrintGeoState() noexcept;
private:
    const std::string m_logFileMessagePath;
private:
    const RarmSocket *const m_rarmSocket;
    const MoxaMediator *const m_moxa;
    const UstirovMediator *const m_ustirov;
    const MeteoMediator *const m_meteo;
    const GeoSocket *const m_geo;
private:
    int m_updateIterval = 500;
    std::string m_progressIterationString;
    int m_iteration = 0;
};

#endif // MEDIATORS_CONSOLEMEDIATOR_H
