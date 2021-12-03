#ifndef MEDIATORS_USTIROVMEDIATOR_H
#define MEDIATORS_USTIROVMEDIATOR_H

#include <queue>
#include <QObject>

#include <QSettings>

#include "messageCreatrors/statemessagesender.h"
#include "messageCreatrors/statemessagegetter.h"

#include "sockets/ustirovsocket.h"

#include "messageRepositories/ustrirovmessagerepository.h"

class UstirovMediator : public QObject
{
    Q_OBJECT
public:
    explicit UstirovMediator(const QString &moxaIpAdress, const QString &settingsFileName, QObject *parent);
    ~UstirovMediator();
private:
    void ReadDataFromSettingsFile(const QString &settingsFileName);
    void CreateObjects();
    void ConnectObjects();
Q_SIGNALS:
    void ToSendRarmUstirovState(DevicesAdjustingKitMessage &upcbState);
public Q_SLOTS:
    void OnSetDataToUstirov(const DevicesAdjustingKitMessage &state);
    void OnGetDataFromUstirov();
private Q_SLOTS:
    void OnGetStateFromMessage(const QByteArray &message);
    void OnRequestTimeOut();
    void OnSendingNext();
    void OnResetQueue();
    void OnAllDataCollected();
private:
    void SetStateCommandsCreate(const DevicesAdjustingKitMessage &state);
    void GetStateCommandsCreate();
    void SendToRarmMessageWithNoConnectionInfo();
private:
    QString m_moxaIpAdress;
    quint16 m_ustirovPort;
    double f;
    double fref;
    UstirovSocket *m_ustirovSocket;
    UstirovMessageSender *m_ustirovMessageSetter;
    UstirovMessageGetter *m_ustirovMessageGetter;
    QSharedPointer<UstrirovMessageRepository> m_ustirovMessageRepository;
    QSharedPointer<std::queue<QByteArray>> m_messagesToSendQueue;
};

#endif // MEDIATORS_USTIROVMEDIATOR_H
