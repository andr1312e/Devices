#ifndef USTIROVSOCKET_H
#define USTIROVSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

#include "mediators/logger.h"

class UstirovSocket : public QObject
{
    Q_OBJECT
public:
    explicit UstirovSocket(const Logger *logger, const QString &moxaIpAdress, const quint16 moxaPort, QObject *parent);
    ~UstirovSocket();
private:
    void CreateObjects();
    void InitObjects();
    void ConnectObjects();
Q_SIGNALS:
    void ToRequestTimeOut();
    void ToWantNextMessage();
    void ToResetQueue();
    void ToGetStateFromMessage(const QByteArray &message);
private Q_SLOTS:
    void OnReadyRead();
    void OnHostConnected();
    void OnDisconnectedFromHost();
    void OnErrorOccurred();
    void OnCheckConnectionTimerTimeOut();
public:
    void SendMessage(const QByteArray &message, bool isRestart);
    void TryToSendLastMessageAgain();
public:
    bool IsUstirovConnected() const;
    QString GetLastUstirovErrorMessage() const;
private:
    void StopNoAnswerTimer();
private:
    QTcpSocket *m_socket;
    const QString m_moxaIpAdress;
    const quint16 m_moxaPort;
private:
    QTimer *m_checkConnectionTimer;
    QTimer *m_noAnswerTimer;
private:
    const Logger * const m_logger;
    QByteArray m_lastMessage;
    QByteArray m_readyReadBuffer;
    const QVarLengthArray<quint8, 7> *m_messageSize;
    const char m_messagesIdToGetState;
};

#endif // USTIROVSOCKET_H
