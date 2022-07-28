#ifndef MEDIATORS_CONSOLEMEDIATOR_H
#define MEDIATORS_CONSOLEMEDIATOR_H
#include <QObject>

#include <cstdlib>
#include <iostream>

#include "../sockets/rarmsocket.h"
#include "moxamediator.h"
#include "ustirovmediator.h"
#include "meteomediator.h"

class ConsoleMediator: public QObject
{
public:
    explicit ConsoleMediator(const std::string &logFilePath, RarmSocket *socket, MoxaMediator *moxa, UstirovMediator *ustirov, MeteoMediator *meteo, QObject *parent);
    ~ConsoleMediator();
public Q_SLOTS:
    void OnStartLog();
protected:
    virtual void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
private:
    void ClearConsole() const noexcept;
    void ToPrintLogFilePath() const noexcept;
    void PrintProcess() noexcept;
    void PrintRarmState() const noexcept;
    void PrintMoxaState() const noexcept;
    void PrintUkitState() const noexcept;
    void PrintMeteoState() const noexcept;
    void PrintGeoState() const noexcept;
private:
    const std::string m_logFileMessagePath;
private:
    const RarmSocket *const m_rarmSocket;
    const MoxaMediator *const m_moxa;
    const UstirovMediator *const m_ustirov;
    const MeteoMediator *const m_meteo;
private:
    int m_updateIterval = 500;
    std::string m_progressIterationString;
    int m_iteration = 0;
};

#endif // MEDIATORS_CONSOLEMEDIATOR_H
