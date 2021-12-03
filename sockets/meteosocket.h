#ifndef METEOSOCKET_H
#define METEOSOCKET_H
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QDebug>
#include <chrono>
#include <QtMath>

#include "datamessage.h"

#include "messageRepositories/meteomessagerepository.h"

class MeteoServer : public QObject
{
    Q_OBJECT
public:
    explicit MeteoServer(const quint16 moxaPort, const quint16 meteoKitPort, QObject *parent);
    ~MeteoServer();
private:
    void CreateObjects();
    void InitObjects();
    void ConnectObjects();
Q_SIGNALS:
    void ToRequestTimeOut();
    void ToResetQueue();
    void ToGetStateFromMessage(const QByteArray &message);
private Q_SLOTS:
    void OnReadyRead();
    void OnNewSocketConnected();
    void OnDisconnectedFromHost();
    void OnErrorOccurred(QAbstractSocket::SocketError socketError);
    void OnStateChanged(QAbstractSocket::SocketState state);
public:
    void SendMessage(const QByteArray &message);
    const bool IsMeteoConnected() const;
private:
    void ConnectMeteoSocketObject();
    void StopNoAnswerTimer();
private:
    QTcpServer *m_serverMeteo;
    QTcpSocket *m_socketMeteo;
    const quint16 m_moxaPort=4101;
    const quint16 m_meteoKitPort=5011;
private:
    const quint8 m_returnedMessageSize=11;

    QTimer *m_noAnswerTimer;
    QByteArray m_readyReadBuffer;

};


#endif // METEOSOCKET_H
