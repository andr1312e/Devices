#ifndef USTIROVSOCKET_H
#define USTIROVSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class UstirovSocket : public QObject
{
    Q_OBJECT
public:
    explicit UstirovSocket(const QString &moxaIpAdress, const quint16 moxaPort, QObject *parent);
    ~UstirovSocket();
private:
    void CreateObjects();
    void InitObjects();
    void ConnectObjects();
Q_SIGNALS:
    void ToNoAnswerGet();
    void ToSendingNext();
    void ToResetQueue();
    void ToStateGettingFromMessage(const QByteArray &message);
private Q_SLOTS:
    void OnReadyRead();
    void OnHostConnected();
    void OnDisconnectedFromHost();
    void OnErrorOccurred(QAbstractSocket::SocketError socketError);
    void OnCheckConnectionTimerTimeOut();
public:
    void SendMessage(const QByteArray &message);
    void TryToSendLastMessageAgain();
    const bool IsUstirovConnected() const;
private:
    QTcpSocket *m_socket;
    const QString m_moxaIpAdress;
    const quint16 m_moxaPort;
private:
    QTimer *m_checkConnectionTimer;
    QTimer *m_noAnswerTimer;
private:
    QByteArray m_lastMessage;
    QByteArray m_readyReadBuffer;
    const QVarLengthArray<quint8, 7> *m_messageSize;
    const char m_messagesIdToGetState;
};

#endif // USTIROVSOCKET_H
