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
    explicit UstirovMediator(const Logger *logger, const QString &moxaIpAdress, QObject *parent);
    ~UstirovMediator();
private:
    void ReadParamsFromProfile();
    void CreateObjects();
    void ConnectObjects();
    void StartPingTimer();
Q_SIGNALS:
    void ToSendRarmNormalState(const DevicesAdjustingKitMessage &upcbState);
    void ToSendRarmBparState(const DevicesBparAdjustingKitMessage &bparState);
    void ToSendPcbWork();
public Q_SLOTS:
    void OnSetNormalSateToUstirov(const DevicesAdjustingKitMessage &state);
    void OnSetBparStateToUstirov(const DevicesBparAdjustingKitMessage &state);
private Q_SLOTS:
    void OnGetStateFromMessage(const QByteArray &message);
    void OnRequestTimeOut();
    void OnResetQueue();
    void OnAllNormalDataCollected();
    void OnAllBparDataCollected();
    void OnSendMessage();
    void OnSendPing();
public:
    quint16 GetUstirovPort() const;
    bool IsUstirovConnected() const;
    QString GetLastUstirovSocketErrorMessage() const;
    int GetMessagesCount() const;
    QList<QByteArray> GetMessageList() const;
private:
    void RestartCommandsCreate();
    void SetStateCommandsCreate(const DevicesAdjustingKitMessage &state);
    void GetNormalStateCommandsCreate();
    void SendToRarmMessageWithNoConnectionInfo();
private:
    QString m_moxaIpAdress;
    quint16 m_ustirovPort;
    double m_f;
    double m_fref;
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
