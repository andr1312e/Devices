#include "ustirovsocket.h"

UstirovSocket::UstirovSocket(const Logger *logger, const QString &moxaIpAdress, const quint16 moxaPort, QObject *parent)
    : QObject(parent)
    , m_moxaIpAdress(moxaIpAdress)
    , m_moxaPort(moxaPort)
    , m_logger(logger)
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
    delete m_checkConnectionTimer;
    delete m_noAnswerTimer;
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
    connect(m_noAnswerTimer, &QTimer::timeout, this, &UstirovSocket::ToRequestTimeOut);
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &UstirovSocket::OnErrorOccurred);
#else
    connect(m_socket, &QTcpSocket::errorOccurred, this, &UstirovSocket::OnErrorOccurred);
#endif
}

void UstirovSocket::OnReadyRead()
{
    StopNoAnswerTimer();
    m_readyReadBuffer.append(m_socket->readAll());
    m_logger->Appends("US Получили сообщение "+m_readyReadBuffer.toHex().toStdString());
    if (m_messagesIdToGetState==m_readyReadBuffer.front())
    {
        if (m_readyReadBuffer.count()>3)
        {
            const quint8 messageId=m_readyReadBuffer.at(1);
            if (messageId>0 && messageId<m_messageSize->count())
            {
                if (m_readyReadBuffer.count()==m_messageSize->at(messageId))
                {
                    Q_EMIT ToGetStateFromMessage(m_readyReadBuffer);
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
        if (3==m_readyReadBuffer.count())
        {
            m_readyReadBuffer.clear();
            Q_EMIT ToWantNextMessage();
        }
        else
        {
            if(m_readyReadBuffer.count()>3)
            {
                m_readyReadBuffer.clear();
            }
        }
    }
}

void UstirovSocket::OnHostConnected()
{
    m_logger->Appends("US - подключен ...");
}

void UstirovSocket::OnDisconnectedFromHost()
{
    Q_EMIT ToResetQueue();
    m_socket->disconnectFromHost();
    m_logger->Appends("US - отключен...");
}

void UstirovSocket::OnErrorOccurred()
{
    m_logger->Appends("Ошибка "+ m_socket->errorString().toStdString());
}

void UstirovSocket::OnCheckConnectionTimerTimeOut()
{
    if (!IsUstirovConnected())
    {
        m_socket->connectToHost(m_moxaIpAdress, m_moxaPort, QIODevice::ReadWrite);
        m_logger->Appends("US: ип: " +m_moxaIpAdress.toStdString() + " порт: " + std::to_string(m_moxaPort) + " пробуем подключится");
    }
}

void UstirovSocket::SendMessage(const QByteArray &message)
{
    m_lastMessage=message;
    if (IsUstirovConnected())
    {
        m_logger->Appends("US: отправили сообщение " + message.toHex().toStdString());
        m_socket->write(message);
        m_socket->flush();
        m_noAnswerTimer->start();

    }
    else
    {
        Q_EMIT ToResetQueue();
        m_logger->Appends("US: пытались отправить сообщение но сокет не подключен");
    }
}

void UstirovSocket::TryToSendLastMessageAgain()
{
    if(IsUstirovConnected() && !m_lastMessage.isEmpty())
    {
        m_socket->write(m_lastMessage);
        m_socket->flush();
        m_noAnswerTimer->start();
    }
    else
    {
        Q_EMIT ToResetQueue();
        m_logger->Appends("US: пытались отправить сообщение снова но сокет не подключен");
    }
}

bool UstirovSocket::IsUstirovConnected() const
{
    return QAbstractSocket::ConnectedState==m_socket->state();
}

QString UstirovSocket::GetLastUstirovErrorMessage() const
{
    return m_socket->errorString();
}

void UstirovSocket::StopNoAnswerTimer()
{
    if (m_noAnswerTimer->isActive())
    {
        m_noAnswerTimer->stop();
    }
}


