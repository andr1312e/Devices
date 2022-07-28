#ifndef SERVER_INCOMINGDATAPARSER_H
#define SERVER_INCOMINGDATAPARSER_H

#include <QTcpSocket>
#include <QUrl>
#include <QTimer>
#include <QDataStream>
#include <QDebug>

#include "../mediators/logger.h"
#include "../datamessage.h"

class RarmSocket : public QObject
{
    Q_OBJECT
public:
    explicit RarmSocket(const Logger *logger, const QString &rarmAdress, quint16 port, QObject *parent);
    ~RarmSocket();
private:
    void CreateObjects();
    void InitObjects();
    void ConnectObjects();
Q_SIGNALS:
    void ToSetUstirovNormalState(const DevicesAdjustingKitMessage &state);
    void ToSetUstirovBparState(const DevicesBparAdjustingKitMessage &state);
    void ToRequestGeoState();
private Q_SLOTS:
    void OnConnectedToRarm();
    void OnReadyRead();
    void OnConnectionError();
    void OnDisconnected();
    void OnCheckConnection();

public Q_SLOTS:
    void OnSendRarmMoxaWorksState(DevicesMoxaStateMessage &moxaState);
    void OnSendRarmMeteoState(const DevicesMeteoKitGetMessage &meteoState);
    void OnSendRarmNormalState(const DevicesAdjustingKitMessage &upcbState);
    void OnSendRarmBparState(const DevicesBparAdjustingKitMessage &bparState);
    void OnSendRarmGeoState(const DevicesGeoStateMessage  &state);

private:
    void ChangeReadyReadConnection(bool state) noexcept;
    void StopRarmConnect();
    void ReconnectToRarm();
    void SendRarmMessage(const QByteArray &message);
public:
    inline bool IsRarmConnected() const noexcept
    {
        return QTcpSocket::ConnectedState == m_pTcpSocketToRarm->state();
    }
    std::string GetRarmError() const noexcept;
    inline quint16 GetRarmPort() const noexcept
    {
        return m_port;
    }
    inline std::string GetRarmAdress() const noexcept
    {
        return m_rarmAdress.toStdString();
    }


private:
    const QString m_rarmAdress;
    const quint16 m_port;
    const std::string m_logFileName;

private:
    const Logger *const m_logger;
    QTcpSocket *m_pTcpSocketToRarm;
    QByteArray gettingMessageArray;
    QTimer *m_timeToReConnectToRarm;
    QVarLengthArray<quint8, 1> m_messagesIdWantedFromRarm;

};

#endif // SERVER_INCOMINGDATAPARSER_H
