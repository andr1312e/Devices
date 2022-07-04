#ifndef MEDIATORS_USTIROVMEDIATOR_H
#define MEDIATORS_USTIROVMEDIATOR_H

#include <QObject>
#include <QSettings>

#include "messageCreatrors/statemessagesender.h"
#include "messageCreatrors/statemessagegetter.h"

#include "sockets/ustirovsocket.h"

#include "messageRepositories/ustrirovmessagerepository.h"

#include "enviroment.h"

class UstirovMediator : public QObject
{
    Q_OBJECT
public:
    explicit UstirovMediator(const Logger *logger, const QString &moxaIpAdress, const QString &settingsFileName, QObject *parent);
    ~UstirovMediator();
private:
    void ReadDataFromSettingsFile(const QString &settingsFileName);
    void CreateObjects();
    void ConnectObjects();
    void StartPingTimer();
Q_SIGNALS:
    void ToSendRarmUstirovState(const DevicesAdjustingKitMessage &upcbState);
    void ToSendPcbWork();
public Q_SLOTS:
    void OnSetDataToUstirov(const DevicesAdjustingKitMessage &state);
    void OnGetDataFromUstirov();
private Q_SLOTS:
    void OnGetStateFromMessage(const QByteArray &message);
    void OnRequestTimeOut();
    void OnResetQueue();
    void OnAllDataCollected();
    void OnSendMessage();
    void OnSendPing();
public:
    quint16 GetUstirovPort() const;
    bool IsUstirovConnected() const;
    QString GetLastUstirovErrorMessage() const;
    int GetMessagesCount() const;
    QList<QByteArray> GetMessageList() const;
private:
    void RestartCommandsCreate();
    void SetStateCommandsCreate(const DevicesAdjustingKitMessage &state);
    void GetStateCommandsCreate();
    void SendToRarmMessageWithNoConnectionInfo();
private:
    QString m_moxaIpAdress;
    quint16 m_ustirovPort;
    double f;
    double fref;
    bool m_isRestartMode;
private:
    const Logger *m_logger;
    QTimer *const m_pingTimer;
    UstirovSocket *m_ustirovSocket;
    UstirovMessageSender *m_ustirovMessageSetter;
    UstirovMessageGetter *m_ustirovMessageGetter;
    UstrirovMessageRepository *m_ustirovMessageRepository;
    QList<QByteArray> m_messagesToSendList;
};

#endif // MEDIATORS_USTIROVMEDIATOR_H
