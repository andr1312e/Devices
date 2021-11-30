#ifndef MEDIATORS_METEOMEDIATOR_H
#define MEDIATORS_METEOMEDIATOR_H

#include <queue>
#include <QObject>
#include <QSettings>

#include "messageCreatrors/meteomessagegetter.h"

#include "sockets/meteosocket.h"

class MeteoMediator : public QObject
{
    Q_OBJECT
public:
    explicit MeteoMediator(const QString &settingsFileName, QObject *parent);
    ~MeteoMediator();
private:
    void ReadDataFromSettingsFile(const QString &settingsFileName);
    void CreateObjects();
    void InitObjects();
    void ConnectObjects();
Q_SIGNALS:
    void ToSendRarmMeteoState(const VOIStateMeteoMessage &meteoState);
private Q_SLOTS:
    void OnMakeNewRequest();
    void OnGetStateFromMessage(const QByteArray &message);
    void OnNoAnswerGet();
    void OnClearQueue();
    void OnAllDataCollected();
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
    MeteoServer *m_meteoServer;
    MeteoMessageGetter *m_meteoMessageGetter;
    std::queue<QByteArray> *m_messagesToSendQueue;
};

#endif // MEDIATORS_METEOMEDIATOR_H
