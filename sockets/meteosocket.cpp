#include "meteosocket.h"


MeteoServer::MeteoServer(const Logger *logger, const quint16 moxaPort, const quint16 meteoKitPort, QObject *parent)
    : QObject(parent)
    , m_socketMeteo(Q_NULLPTR)
    , m_moxaPort(moxaPort)
    , m_meteoKitPort(meteoKitPort)
    , m_logger(logger)
{
    CreateObjects();
    InitObjects();
    ConnectObjects();
    m_serverMeteo->listen(QHostAddress::Any, m_moxaPort);
}

MeteoServer::~MeteoServer()
{
    OnDisconnectedFromHost();
    delete m_noAnswerTimer;
    delete m_serverMeteo;
}

void MeteoServer::CreateObjects()
{
    m_serverMeteo = new QTcpServer(this);
    m_noAnswerTimer = new QTimer(this);
}

void MeteoServer::InitObjects()
{
    m_noAnswerTimer->setInterval(std::chrono::seconds(2));
    m_noAnswerTimer->setTimerType(Qt::VeryCoarseTimer);
    m_noAnswerTimer->setSingleShot(true);
}

void MeteoServer::ConnectObjects()
{
    connect(m_serverMeteo, &QTcpServer::newConnection, this, &MeteoServer::OnNewSocketConnected);
    connect(m_noAnswerTimer, &QTimer::timeout, this, &MeteoServer::ToRequestTimeOut);
    connect(m_serverMeteo, &QTcpServer::acceptError, this, &MeteoServer::OnErrorOccurred);
}

void MeteoServer::OnNewSocketConnected()
{

    QTcpSocket * const socket = m_serverMeteo->nextPendingConnection();

    m_logger->Appends("MS - новое подключение");
    if(socket->peerPort() == m_meteoKitPort)
    {
        m_logger->Appends("MS - подключено ");
        m_socketMeteo = socket;
        ConnectMeteoSocketObject();
    }
    else
    {
        m_logger->Appends("MS - нужен порт: "+std::to_string(m_meteoKitPort)+ " однако наш: " + std::to_string(socket->peerPort()));
    }
}

void MeteoServer::OnReadyRead()
{
    StopNoAnswerTimer();
    m_logger->Appends("MS - получили сообщение");
    m_readyReadBuffer.append(m_socketMeteo->readAll());
    if(m_readyReadBuffer.length() >= m_returnedMessageSize) // длина ответа 11 байт
    {
        QByteArray message = m_readyReadBuffer.mid(0, m_returnedMessageSize);
        m_readyReadBuffer.remove(0, m_returnedMessageSize);

        Q_EMIT ToGetStateFromMessage(message);
    }
}

void MeteoServer::OnDisconnectedFromHost()
{
    m_socketMeteo->disconnectFromHost();
    m_logger->Appends("MS - отключен");
}

void MeteoServer::OnErrorOccurred()
{
    m_logger->Appends("MS - ошибка "+ m_socketMeteo->errorString().toStdString());
}

void MeteoServer::SendMessage(const QByteArray &array)
{
    if (IsMeteoConnected())
    {
        m_socketMeteo->write(array);
        m_socketMeteo->flush();
        m_noAnswerTimer->start();
    }
    else
    {
        m_logger->Appends("MS - нет подключения при попытке отправить сообщение");
    }
}

bool MeteoServer::IsMeteoConnected() const
{
    if (Q_NULLPTR==m_socketMeteo)
    {
        return false;
    }
    else
    {
        return m_socketMeteo->state()==QAbstractSocket::ConnectedState;
    }

}

quint16 MeteoServer::GetMeteoPort() const
{
    return m_meteoKitPort;
}

std::string MeteoServer::GetMeteoError() const
{
    if(Q_NULLPTR==m_socketMeteo)
    {
        return "Не подключено";
    }
    else
    {
        return m_socketMeteo->errorString().toStdString();
    }
}

void MeteoServer::ConnectMeteoSocketObject()
{
    connect(m_noAnswerTimer, &QTimer::timeout, this, &MeteoServer::ToRequestTimeOut);
    connect(m_socketMeteo, &QTcpSocket::connected, this, &MeteoServer::OnNewSocketConnected);
    connect(m_socketMeteo, &QTcpSocket::readyRead, this, &MeteoServer::OnReadyRead);
    connect(m_socketMeteo, &QTcpSocket::disconnected, this, &MeteoServer::OnDisconnectedFromHost);
}

void MeteoServer::StopNoAnswerTimer()
{
    if (m_noAnswerTimer->isActive())
    {
        m_noAnswerTimer->stop();
    }
}

