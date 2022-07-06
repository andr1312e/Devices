#ifndef SERVER_INCOMINGDATAPARSER_H
#define SERVER_INCOMINGDATAPARSER_H

#include <QTcpSocket>
#include <QUrl>
#include <QTimer>
#include <QDataStream>
#include <QDebug>

#include "mediators/logger.h"
#include "datamessage.h"

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
private Q_SLOTS:
   void OnConnectedToRarm();
   void OnReadyRead();
   void OnConnectionError(QAbstractSocket::SocketError socketError);
   void OnDisconnected();
   void OnCheckConnection();

public Q_SLOTS:
   void OnSendRarmMoxaWorksState(DevicesMoxaStateMessage &moxaState);
   void OnSendRarmMeteoState(const DevicesMeteoKitGetMessage &meteoState);
   void OnSendRarmUPCBNormalState(const DevicesAdjustingKitMessage &upcbState);
   void OnSendRarmUPCBBparState(const DevicesBparAdjustingKitMessage &bparState);
   void OnSendRarmGeoMessage(const DevicesGeoStateMessage  &state);

private:
   void ChangeReadyReadConnection(bool state);
   void StopRarmConnect();
   void ReconnectToRarm();
   void SendRarmMessage(const QByteArray &message);
public:
   bool IsRarmConnected() const;
   std::string GetRarmError() const;
   quint16 GetRarmPort() const;
   std::string GetRarmAdress() const;


private:
   const QString m_rarmAdress;
   const quint16 m_port;
   const std::string m_logFileName;

private:
   const Logger * const m_logger;
   QTcpSocket *m_pTcpSocketToRarm;
   QByteArray gettingMessageArray;
   QTimer *m_timeToReConnectToRarm;
   QVarLengthArray<quint8, 1> m_messagesIdWantedFromRarm;

};

#endif // SERVER_INCOMINGDATAPARSER_H
