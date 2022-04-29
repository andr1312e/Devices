#ifndef MEDIATORS_USTIROVMEDIATOR_H
#define MEDIATORS_USTIROVMEDIATOR_H

#include <QObject>
#include <QLinkedList>
#include <QSettings>

#include "messageCreatrors/statemessagesender.h"
#include "messageCreatrors/statemessagegetter.h"

#include "sockets/ustirovsocket.h"

#include "messageRepositories/ustrirovmessagerepository.h"

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
Q_SIGNALS:
    void ToSendRarmUstirovState(const DevicesAdjustingKitMessage &upcbState);
public Q_SLOTS:
    void OnSetDataToUstirov(const DevicesAdjustingKitMessage &state);
    void OnGetDataFromUstirov();
private Q_SLOTS:
    void OnGetStateFromMessage(const QByteArray &message);
    void OnRequestTimeOut();
    void OnWantedNextMessage();
    void OnResetQueue();
    void OnAllDataCollected();
    void OnTimeToSendMessage();
public:
    quint16 GetUstirovPort() const;
    bool IsUstirovConnected() const;
    QString GetLastUstirovErrorMessage() const;
    int GetMessagesCount() const;
    QLinkedList<QByteArray> *GetMessageList() const;
private:
    void SetStateCommandsCreate(const DevicesAdjustingKitMessage &state);
    void GetStateCommandsCreate();
    void SendToRarmMessageWithNoConnectionInfo();
private:
    QString m_moxaIpAdress;
    quint16 m_ustirovPort;
    double f;
    double fref;
private:
    QTimer *m_sendNextMessageTimer;
private:
    const Logger *m_logger;
    UstirovSocket *m_ustirovSocket;
    UstirovMessageSender *m_ustirovMessageSetter;
    UstirovMessageGetter *m_ustirovMessageGetter;
    UstrirovMessageRepository *m_ustirovMessageRepository;
    QLinkedList<QByteArray> *m_messagesToSendList;
};

#endif // MEDIATORS_USTIROVMEDIATOR_H
