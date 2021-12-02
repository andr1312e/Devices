#include "ustirovsocket.h"

UstirovSocket::UstirovSocket(const QString &moxaIpAdress, const quint16 moxaPort, QObject *parent)
    : QObject(parent)
    , m_moxaIpAdress(moxaIpAdress)
    , m_moxaPort(moxaPort)
    , m_messagesIdToGetState(0x07)
{
    CreateObjects();
    InitObjects();
    ConnectObjects();
}

UstirovSocket::~UstirovSocket()
{
    OnDisconnectedFromHost();
    delete m_socket;
    delete m_messageSize;
}

void UstirovSocket::CreateObjects()
{
    m_socket=new QTcpSocket(this);
    m_messageSize=new QVarLengthArray<quint8, 7>({0, 8, 8, 0, 4, 3, 3});
    m_checkConnectionTimer=new QTimer(this);
    m_noAnswerTimer=new QTimer(this);
}

void UstirovSocket::InitObjects()
{
    m_checkConnectionTimer->setInterval(std::chrono::seconds(2));
    m_checkConnectionTimer->setTimerType(Qt::VeryCoarseTimer);
    m_checkConnectionTimer->start();
    m_noAnswerTimer->setInterval(500);
    m_noAnswerTimer->setTimerType(Qt::CoarseTimer);
    m_noAnswerTimer->setSingleShot(true);
}

void UstirovSocket::ConnectObjects()
{
    connect(m_socket, &QTcpSocket::connected, this, &UstirovSocket::OnHostConnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &UstirovSocket::OnReadyRead);
    connect(m_socket, &QTcpSocket::disconnected, this, &UstirovSocket::OnDisconnectedFromHost);
    connect(m_checkConnectionTimer, &QTimer::timeout, this, &UstirovSocket::OnCheckConnectionTimerTimeOut);
    connect(m_noAnswerTimer, &QTimer::timeout, this, &UstirovSocket::ToNoAnswerGet);
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &UstirovSocket::OnErrorOccurred);
#else
    connect(m_socket, &QTcpSocket::errorOccurred, this, &UstirovSocket::OnErrorOccurred);
#endif
}

void UstirovSocket::OnReadyRead()
{
    m_readyReadBuffer.append(m_socket->readAll());
    qDebug()<< "getOnReady read " << m_readyReadBuffer.toHex();
    if (m_messagesIdToGetState==m_readyReadBuffer.front())
    {
        if (m_readyReadBuffer.count()>2)
        {
            quint8 messageId=m_readyReadBuffer.at(1);
            if (messageId>0 && messageId<m_messageSize->count())
            {
                if (m_readyReadBuffer.count()==m_messageSize->at(messageId))
                {
//                    qDebug()<< "UKS - Get message with state: " << m_readyReadBuffer.toHex();
                    Q_EMIT ToStateGettingFromMessage(m_readyReadBuffer);
                    m_readyReadBuffer.clear();
                }
                else
                {
                    if(m_readyReadBuffer.count()>m_messageSize->at(messageId))
                    {
                        m_readyReadBuffer.clear();
                    }
                }
            }
            else
            {
                m_readyReadBuffer.clear();
            }
        }
    }
    else
    {
        if (m_readyReadBuffer.count()==2)
        {
            m_readyReadBuffer.clear();
            Q_EMIT ToSendingNext();
        }
        else
        {
            if(m_readyReadBuffer.count()>2)
            {
                m_readyReadBuffer.clear();
            }
        }
    }
}

void UstirovSocket::OnHostConnected()
{
    qDebug()<< "UKS - connected ...";
}

void UstirovSocket::OnDisconnectedFromHost()
{
    Q_EMIT ToResetQueue();
    m_socket->disconnectFromHost();
    qDebug()<< "UKS - Disconnected...";
}

void UstirovSocket::OnErrorOccurred(QAbstractSocket::SocketError socketError)
{
    Q_EMIT ToResetQueue();
    switch (socketError)
    {
    case QAbstractSocket::ConnectionRefusedError:
        qDebug()<<QStringLiteral("UKS - ConnectionRefusedError");
        break;
    case QAbstractSocket::RemoteHostClosedError:
        qDebug()<<QStringLiteral("UKS - RemoteHostClosedError");
        break;
    case QAbstractSocket::SocketAccessError:
        qDebug()<<QStringLiteral("UKS - SocketAccessError");
        break;
    case QAbstractSocket::SocketResourceError:
        qDebug()<<QStringLiteral("UKS - SocketResourceError");
        break;
    case QAbstractSocket::SocketTimeoutError:
        qDebug()<<QStringLiteral("UKS - SocketTimeoutError");
        break;
    case QAbstractSocket::DatagramTooLargeError:
        qDebug()<<QStringLiteral("UKS - DatagramTooLargeError");
        break;
    case QAbstractSocket::NetworkError:
        qDebug()<<QStringLiteral("UKS - NetworkError");
        break;
    case QAbstractSocket::AddressInUseError:
        qDebug()<<QStringLiteral("UKS - AddressInUseError");
        break;
    case QAbstractSocket::SocketAddressNotAvailableError:
        qDebug()<<QStringLiteral("UKS - SocketAddressNotAvailableError");
        break;
    case QAbstractSocket::UnsupportedSocketOperationError:
        qDebug()<<QStringLiteral("UKS - UnsupportedSocketOperationError");
        break;
    case QAbstractSocket::UnfinishedSocketOperationError:
        qDebug()<<QStringLiteral("UKS - UnfinishedSocketOperationError");
        break;
    case QAbstractSocket::ProxyAuthenticationRequiredError:
        qDebug()<<QStringLiteral("UKS - ProxyAuthenticationRequiredError");
        break;
    case QAbstractSocket::SslHandshakeFailedError:
        qDebug()<<QStringLiteral("UKS - SslHandshakeFailedError");
        break;
    case QAbstractSocket::ProxyConnectionRefusedError:
        qDebug()<<QStringLiteral("UKS - ProxyConnectionRefusedError");
        break;
    case QAbstractSocket::ProxyConnectionClosedError:
        qDebug()<<QStringLiteral("UKS - ProxyConnectionClosedError");
        break;
    case QAbstractSocket::ProxyConnectionTimeoutError:
        qDebug()<<QStringLiteral("UKS - ProxyConnectionTimeoutError");
        break;
    case QAbstractSocket::ProxyNotFoundError:
        qDebug()<<QStringLiteral("UKS - ProxyNotFoundError");
        break;
    case QAbstractSocket::ProxyProtocolError:
        qDebug()<<QStringLiteral("UKS - ProxyProtocolError");
        break;
    case QAbstractSocket::OperationError:
        qDebug()<<QStringLiteral("UKS - OperationError");
        break;
    case QAbstractSocket::SslInternalError:
        qDebug()<<QStringLiteral("UKS - SslInternalError");
        break;
    case QAbstractSocket::SslInvalidUserDataError:
        qDebug()<<QStringLiteral("UKS - SslInvalidUserDataError");
        break;
    case QAbstractSocket::TemporaryError:
        qDebug()<<QStringLiteral("UKS - TemporaryError");
        break;
    case QAbstractSocket::UnknownSocketError:
        qDebug()<<QStringLiteral("UKS - UnknownSocketError");
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug()<<QStringLiteral("UKS - HostNotFoundError");
        break;
    }
}

void UstirovSocket::OnCheckConnectionTimerTimeOut()
{
//    qDebug()<< "UKS - Time to check connection ip: " << m_moxaIpAdress << " port: " << m_moxaPort;
    if (!IsUstirovConnected())
    {
        m_socket->connectToHost(m_moxaIpAdress, m_moxaPort, QIODevice::ReadWrite);
        qDebug()<< "UKS - ip: " << m_moxaIpAdress << " port: " << m_moxaPort << " reconnect";
    }
}

void UstirovSocket::SendMessage(const QByteArray &message)
{
    qDebug()<< "UstirovSocket::SendMessage";
    m_lastMessage=message;
    if (IsUstirovConnected())
    {
        qDebug()<< "UKS - send message successfully " << message.toHex();
        m_socket->write(message);
        m_socket->flush();

    }
    else
    {
        Q_EMIT ToResetQueue();
        qDebug()<< "UKS - message doesn't send, socket don't connected " << message.toHex();
    }
}

void UstirovSocket::TryToSendLastMessageAgain()
{
    if(IsUstirovConnected() && !m_lastMessage.isEmpty())
    {
        m_socket->write(m_lastMessage);
        m_socket->flush();
    }
    else
    {
        Q_EMIT ToResetQueue();
        qDebug()<< "UKS - message doesn't send, , socket don't connected " << m_lastMessage.toHex();
    }
}

const bool UstirovSocket::IsUstirovConnected() const
{
    return m_socket->state()== QAbstractSocket::ConnectedState;
}


