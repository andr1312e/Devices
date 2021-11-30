#ifndef SERVER_INCOMINGDATAPARSER_H
#define SERVER_INCOMINGDATAPARSER_H

#include <QTcpSocket>
#include <QUrl>
#include <QTimer>
#include <QDataStream>
#include <QDebug>
#include <iterator>

#include "datamessage.h"

class RarmSocket : public QObject
{
   Q_OBJECT

public:
   RarmSocket(const QString &rarmAdress, quint16 port, QObject *parent);
   ~RarmSocket();
private:
   void CreateObjects();
   void InitObjects();
   void ConnectObjects();
Q_SIGNALS:
   void ToSetUstirovState(const DevicesAdjustingKitMessage &state);
   void ToGetUstirovState();
private Q_SLOTS:
   void OnConnectedToRarm();
   void OnReadyRead();
   void OnConnectionError(QAbstractSocket::SocketError socketError);
   void OnDisconnected();
   void OnCheckConnection();

public Q_SLOTS:
   void OnSendRarmMeteoState(const VOIStateMeteoMessage &meteoState);
   void OnSendRarmUPCBState(const DevicesAdjustingKitMessage &upcbState);

private:
   const QString m_rarmAdress;
   const quint16 m_port;

   QTcpSocket *m_pTcpSocketToRarm;

   QByteArray gettingMessageArray;

   QTimer *m_timeToReConnectToRarm;

   QVarLengthArray<quint8, 1> m_messagesIdWantedFromRarm;


private:
   void ChangeReadyReadConnection(bool state);
   void StopRarmConnect();
   void ReconnectToRarm();
   bool IsRarmConnected();

   void SendRarmMessage(const QByteArray &message);
};

#endif // SERVER_INCOMINGDATAPARSER_H
