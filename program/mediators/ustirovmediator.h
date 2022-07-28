#ifndef MEDIATORS_USTIROVMEDIATOR_H
#define MEDIATORS_USTIROVMEDIATOR_H

#include <QObject>
#include <QSettings>

#include <chrono>

#include "../messageCreatrors/ustirovmessagesender.h"
#include "../messageCreatrors/ustirovmessagegegetter.h"

#include "../sockets/ustirovsocket.h"

#include "../messageRepositories/ustrirovmessagerepository.h"

#include "../enviroment.h"

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
    void ToSendRarmGeoState(const DevicesGeoStateMessage &geoMessage);
    void ToSendPcbWork();
public Q_SLOTS:
    void OnSetUstirovNormalState(const DevicesAdjustingKitMessage &state);
    void OnSetUstirovBparState(const DevicesBparAdjustingKitMessage &state);
    void OnRequestGeoState();
private Q_SLOTS:
    void OnGetStateFromMessage(const QByteArray &message);
    void OnGetGeoFromMessage(const QByteArray &message);
    void OnRequestTimeOut();
    void OnResetQueue() noexcept;
    void OnAllNormalDataCollected();
    void OnAllBparDataCollected();
    void OnAllGeoCollected();
    void OnSendNextMessage();
    void OnSendPing();
public:
    inline quint16 GetUstirovPort() const noexcept
    {
        return m_ustirovPort;
    }
    inline bool IsUstirovConnected() const noexcept
    {
        return m_ustirovSocket->IsUstirovConnected();
    }
    QString GetLastUstirovSocketErrorMessage() const noexcept;
    inline int GetMessagesCount() const noexcept
    {
        return m_messagesToSendList.count();
    }
    QList<QByteArray> GetMessageList() const noexcept;
private:
    void RestartCommandsCreate() noexcept;
    void SetNormalStateCommandsCreate(const DevicesAdjustingKitMessage &state);
    void GetNormalStateCommandsCreate() noexcept;
    void SendToRarmMessageWithNoConnectionInfo() noexcept;
private:
    const QString m_moxaIpAdress;
    quint16 m_ustirovPort;
    double m_f;
    double m_fref;
    bool m_isRestartMode;
private:
    const Logger *const m_logger;
    QTimer *const m_pingTimer;
    UstirovSocket *m_ustirovSocket;
    UstirovMessageSender *m_ustirovMessageSetter;
    UstirovMessageGetter *m_ustirovMessageGetter;
    UstrirovMessageRepository *m_ustirovMessageRepository;
    GeoMessageRepository m_geoRepository;
    QList<QByteArray> m_messagesToSendList;
};

#endif // MEDIATORS_USTIROVMEDIATOR_H
