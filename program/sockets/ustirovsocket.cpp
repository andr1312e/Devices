#include "ustirovsocket.h"

UstirovSocket::UstirovSocket(const Logger *logger, const QString &moxaIpAdress, const quint16 moxaPort, QObject *parent)
    : QObject(parent)
#ifdef Q_OS_LINUX
    , m_moxaIpAdress(moxaIpAdress)
    , m_moxaPort(moxaPort)
#endif
    , m_logger(logger)
    , m_messagesSizes(std::array<quint8, 10>
{
    0, 8, 8, 4, 4, 3, 3, 0, 0, 5
})
, m_messagesIdToGetState(0x07)
{
    CreateObjects();
    InitObjects();
    ConnectObjects();
    Q_ASSERT(m_messagesSizes.size() == 10);
}

UstirovSocket::~UstirovSocket()
{
#ifdef Q_OS_LINUX
    OnDisconnectedFromHost();
#endif
    delete m_socket;
    delete m_checkConnectionTimer;
    delete m_noAnswerTimer;
}

void UstirovSocket::CreateObjects()
{
#ifdef Q_OS_WINDOWS
    m_socket = new QSerialPort(this);
#else
    m_socket = new QTcpSocket(this);
#endif
    m_checkConnectionTimer = new QTimer(this);
    m_noAnswerTimer = new QTimer(this);
}

void UstirovSocket::InitObjects()
{
#ifdef Q_OS_WINDOWS
    m_socket->setParity(QSerialPort::EvenParity);
    m_socket->setFlowControl(QSerialPort::NoFlowControl);
    m_socket->setBaudRate(QSerialPort::Baud115200);
    m_socket->setStopBits(QSerialPort::OneStop);
#endif
    m_checkConnectionTimer->setInterval(std::chrono::seconds(2));
    m_checkConnectionTimer->setTimerType(Qt::VeryCoarseTimer);
    m_checkConnectionTimer->start();
    m_noAnswerTimer->setInterval(std::chrono::seconds(8));
    m_noAnswerTimer->setTimerType(Qt::CoarseTimer);
    m_noAnswerTimer->setSingleShot(true);
}

void UstirovSocket::ConnectObjects()
{
#ifdef Q_OS_WINDOWS
    connect(m_socket, &QSerialPort::readyRead, this, &UstirovSocket::OnReadyRead);
#else
    connect(m_socket, &QTcpSocket::connected, this, &UstirovSocket::OnHostConnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &UstirovSocket::OnReadyRead);
    connect(m_socket, &QTcpSocket::disconnected, this, &UstirovSocket::OnDisconnectedFromHost);
#endif
    connect(m_checkConnectionTimer, &QTimer::timeout, this, &UstirovSocket::OnCheckConnectionTimerTimeOut);
    connect(m_noAnswerTimer, &QTimer::timeout, this, &UstirovSocket::ToRequestTimeOut);
}

void UstirovSocket::OnReadyRead()
{
    StopNoAnswerTimer();
    const QByteArray socketMessage = m_socket->readAll();
    m_readyReadBuffer.append(socketMessage);
    m_logger->Appends("US: В буфере сообщение " + m_readyReadBuffer.toHex().toStdString());
    if (m_readyReadBuffer.count() > 1)
    {
        if (m_readyReadBuffer.count() == 2  && static_cast<char>(0x4f) == m_readyReadBuffer.front() && static_cast<char>(0x6b) == m_readyReadBuffer.at(1))
        {
            //4f6b - Ок. Высылаем сообщение следующее
            m_readyReadBuffer.clear();
            Q_EMIT ToWantNextMessage();
        }
        else
        {
            if (m_readyReadBuffer.front() == static_cast<char>(0x07))
            {
                //Какое то сообщение с состоянием
                const int messageId = m_readyReadBuffer.at(1);//узнаем что за подсообщение
                if (0x06 == messageId)//сообщение с режимом
                {
                    if (m_readyReadBuffer.count() > 2)
                    {
                        const int subMessageId = m_readyReadBuffer.at(2);
                        bool hasFullSize = false;
                        switch (subMessageId)
                        {
                        case 0:
                        case 1:
                            hasFullSize = m_readyReadBuffer.count() == m_messagesSizes.at(6);
                            break;
                        case 5:
                            hasFullSize = 8 ==  m_readyReadBuffer.count();
                            break;
                        default:
                            break;
                        }
                        if (hasFullSize)
                        {
                            Q_EMIT ToGetStateFromMessage(m_readyReadBuffer);
                            m_readyReadBuffer.clear();
                        }
                    }
                    return;
                }
                else
                {
                    if ((messageId > 0x00 && messageId < 0x06) || messageId == 0x09)//любое из сообщений кроме режима и гео
                    {
                        if (m_readyReadBuffer.count() == m_messagesSizes.at(messageId))
                        {
                            Q_EMIT ToGetStateFromMessage(m_readyReadBuffer);
                            m_readyReadBuffer.clear();
                            return;
                        }
                        else
                        {
                            //неверные размеры, мб меньше сообщение
                            if (m_readyReadBuffer.count() > m_messagesSizes.at(messageId))
                            {
                                //размер больше уже не распарсим, очищаем буфер и пробуем запросить снова
                                m_readyReadBuffer.clear();
                            }
                            return;
                        }
                    }
                }
            }
            m_readyReadBuffer.clear();
            Q_EMIT ToGetGeoFromMessage(socketMessage);
        }
    }
}

void UstirovSocket::OnHostConnected()
{
    m_logger->Appends("US: подключен ...");
}

void UstirovSocket::OnDisconnectedFromHost()
{
    Q_EMIT ToResetQueue();
#ifdef Q_OS_LINUX
    m_socket->disconnectFromHost();
#endif
    m_logger->Appends("US: отключен...");
}

void UstirovSocket::OnErrorOccurred()
{
    m_logger->Appends("US: Ошибка " + m_socket->errorString().toStdString());
}

void UstirovSocket::OnCheckConnectionTimerTimeOut()
{
    if (!IsUstirovConnected())
    {
#ifdef Q_OS_WINDOWS
        const QList<QSerialPortInfo> portsList = QSerialPortInfo::availablePorts();
        for (const QSerialPortInfo &portInfo : portsList)
        {
            qInfo() << "   " << portInfo.portName() << " " << portInfo.manufacturer();
        }
        m_socket->setPort(portsList.back());
        m_socket->open(QIODevice::ReadWrite);
#else
        m_socket->connectToHost(m_moxaIpAdress, m_moxaPort, QIODevice::ReadWrite);
        m_logger->Appends("US: ип: " + m_moxaIpAdress.toStdString() + " порт: " + std::to_string(m_moxaPort) + " пробуем подключится");
#endif
    }
}

void UstirovSocket::SendMessage(const QByteArray &message, bool isRestart) noexcept
{
    if (isRestart)
    {
        m_lastMessage.clear();
        m_logger->Appends("US: отправили сообщение " + message.toHex().toStdString());
        m_socket->write(message);
        m_socket->flush();
    }
    else
    {
        m_lastMessage = message;
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
}

void UstirovSocket::TryToSendLastMessageAgain() noexcept
{
    if (IsUstirovConnected() && !m_lastMessage.isEmpty())
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

QString UstirovSocket::GetLastUstirovErrorMessage() const noexcept
{
    return m_socket->errorString();
}




