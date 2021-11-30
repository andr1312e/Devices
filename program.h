#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>

#include "datamessage.h"
#include "sockets/rarmsocket.h"

#include "mediators/meteomediator.h"
#include "mediators/ustirovmediator.h"

class Program : public QObject
{
    Q_OBJECT
public:
    explicit Program(QObject *parent);
    ~Program();
private:
    void CreateObjects();
    void ConnectObjects();
private:
    const QString m_rarmAdress;
    const quint16 m_rarmPort;
    const QString m_settingsFileName;
    RarmSocket *m_rarmSocket;

    MeteoMediator *m_meteoMediator;
    UstirovMediator *m_ustirovMediator;


};

#endif // PROGRAM_H
