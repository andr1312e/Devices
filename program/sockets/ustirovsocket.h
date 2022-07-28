#ifndef SOCKETS_USTIROVSOCKET_H
#define SOCKETS_USTIROVSOCKET_H

#include <QObject>
#include <QTimer>
#include <QDebug>

#ifdef Q_OS_WINDOWS
#include <QSerialPort>
#include <QSerialPortInfo>
#else
#include <QTcpSocket>
#endif


#include "../mediators/logger.h"

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
    void ToGetGeoFromMessage(const QByteArray &message);
private Q_SLOTS:
    void OnReadyRead();
    void OnHostConnected();
    void OnDisconnectedFromHost();
    void OnErrorOccurred();
    void OnCheckConnectionTimerTimeOut();
public:
    void SendMessage(const QByteArray &message, bool isRestart) noexcept;
    void TryToSendLastMessageAgain() noexcept;
public:
    inline bool IsUstirovConnected() const noexcept
    {
        {
#ifdef Q_OS_WINDOWS
            return m_socket->isOpen();
#else
            return QAbstractSocket::ConnectedState == m_socket->state();
#endif
        }
    }
    QString GetLastUstirovErrorMessage() const noexcept;
private:
    inline void StopNoAnswerTimer() noexcept
    {
        if (m_noAnswerTimer->isActive())
        {
            m_noAnswerTimer->stop();
        }
    }
private:
#ifdef Q_OS_WINDOWS
    QSerialPort *m_socket;
#else
    QTcpSocket *m_socket;
    const QString m_moxaIpAdress;
    const quint16 m_moxaPort;
#endif
private:
    QTimer *m_checkConnectionTimer;
    QTimer *m_noAnswerTimer;
private:
    const Logger *const m_logger;
    QByteArray m_lastMessage;
    QByteArray m_readyReadBuffer;
    const std::array<quint8, 10> m_messagesSizes;
    const char m_messagesIdToGetState;
};

#endif // SOCKETS_USTIROVSOCKET_H
