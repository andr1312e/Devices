#ifndef MEDIATORS_METEOMEDIATOR_H
#define MEDIATORS_METEOMEDIATOR_H

#include <queue>
#include <QObject>
#include <QSettings>
#include <QQueue>
#include <QtGlobal>

#include "enviroment.h"
#include "messageCreatrors/meteomessagegetter.h"
#include "mediators/logger.h"
#include "sockets/meteosocket.h"

class MeteoMediator : public QObject
{
    Q_OBJECT
public:
    explicit MeteoMediator(const Logger *logger, QObject *parent);
    ~MeteoMediator();
private:
    void ReadParamsFromProfile();
    void CreateObjects();
    void InitObjects();
    void ConnectObjects();
Q_SIGNALS:
    void ToSendRarmMeteoState(const DevicesMeteoKitGetMessage &meteoState);
private Q_SLOTS:
    void OnMakeNewRequest();
    void OnGetStateFromMessage(const QByteArray &message);
    void OnRequestTimeOut();
    void OnAllDataCollected();
public:
    bool IsMeteoConnected() const noexcept;
    quint16 GetMeteoPort() const noexcept;
    std::string GetMeteoData() const noexcept;
    std::string GetMeteoError() const noexcept;
    std::string GetLastMessageTime() const;
private:
    void SendingNextMessageInQueue();
    const QByteArray CreateMessage(quint16 time_average, quint8 num_command) const;
private:
    quint16 m_meteoRequestInterval;
    quint16 m_meteoTimeOutInterval;
    quint16 m_moxaPort;
    quint16 m_meteoPort;
private:
    QTimer *m_makeNewRequestTimer;
private:
    const Logger *const m_logger;
    MeteoServer *m_meteoServer;
    MeteoMessageGetter *m_meteoMessageParser;
    QQueue<QByteArray> *m_messagesToSendQueue;
};

#endif // MEDIATORS_METEOMEDIATOR_H
