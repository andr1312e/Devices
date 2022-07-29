#include "meteomediator.h"

MeteoMediator::MeteoMediator(const Logger *logger, QObject *parent)
    : QObject(parent)
    , m_logger(logger)
{
    ReadParamsFromProfile();
    CreateObjects();
    InitObjects();
    ConnectObjects();
}

MeteoMediator::~MeteoMediator()
{
    delete m_meteoServer;
    delete m_messagesToSendQueue;
    delete m_meteoMessageParser;
    delete m_makeNewRequestTimer;
}

void MeteoMediator::ReadParamsFromProfile()
{

    const QString meteoRequestInterval = ProFile::GetProFileField(QLatin1String("meteoRequestInterval"));
    if (meteoRequestInterval.isEmpty())
    {
        m_meteoRequestInterval = 5000;
        ProFile::SetProfileField(QLatin1String("meteoRequestInterval"), QString::number(m_meteoRequestInterval));
    }
    else
    {
        bool isNum = false;
        m_meteoRequestInterval = meteoRequestInterval.toUInt(&isNum);
        if (!isNum)
        {
            m_meteoRequestInterval = 5000;
            ProFile::SetProfileField(QLatin1String("meteoRequestInterval"), QString::number(m_meteoRequestInterval));
        }
    }

    const QString meteoTimeOutInterval = ProFile::GetProFileField(QLatin1String("meteoTimeOutInterval"));
    if (meteoTimeOutInterval.isEmpty())
    {
        m_meteoTimeOutInterval = 500;
        ProFile::SetProfileField(QLatin1String("meteoTimeOutInterval"), QString::number(m_meteoTimeOutInterval));
    }
    else
    {
        bool isNum = false;
        m_meteoTimeOutInterval = meteoTimeOutInterval.toUInt(&isNum);
        if (!isNum)
        {
            m_meteoTimeOutInterval = 500;
            ProFile::SetProfileField(QLatin1String("meteoTimeOutInterval"), QString::number(m_meteoTimeOutInterval));
        }
    }

    const QString moxaPort = ProFile::GetProFileField(QLatin1String("moxaPort"));
    if (moxaPort.isEmpty())
    {
        m_moxaPort = 4101;
        ProFile::SetProfileField(QLatin1String("moxaPort"), QString::number(m_moxaPort));
    }
    else
    {
        bool isNum = false;
        m_moxaPort = moxaPort.toUInt(&isNum);
        if (!isNum)
        {
            m_moxaPort = 4101;
            ProFile::SetProfileField(QLatin1String("moxaPort"), QString::number(m_moxaPort));
        }
    }


    const QString meteoPort = ProFile::GetProFileField(QLatin1String("meteoKitPort"));
    if (meteoPort.isEmpty())
    {
        m_meteoPort = 5011;
        ProFile::SetProfileField(QLatin1String("meteoKitPort"), QString::number(m_meteoPort));
    }
    else
    {
        bool isNum = false;
        m_meteoPort = meteoPort.toUInt(&isNum);
        if (!isNum)
        {
            m_meteoPort = 5011;
            ProFile::SetProfileField(QLatin1String("meteoKitPort"), QString::number(m_meteoPort));
        }
    }
}

void MeteoMediator::CreateObjects()
{
    m_meteoServer = new MeteoServer(m_logger, m_moxaPort, m_meteoPort, this);
    m_meteoMessageParser = new MeteoMessageGetter(this);
    m_makeNewRequestTimer = new QTimer(this);
    m_messagesToSendQueue = new QQueue<QByteArray>();
}

void MeteoMediator::InitObjects()
{
    m_makeNewRequestTimer->setInterval(m_meteoRequestInterval);
    m_makeNewRequestTimer->setTimerType(Qt::VeryCoarseTimer);
    m_makeNewRequestTimer->start();
}

void MeteoMediator::ConnectObjects()
{
    connect(m_meteoServer, &MeteoServer::ToGetStateFromMessage, this, &MeteoMediator::OnGetStateFromMessage);
    connect(m_meteoServer, &MeteoServer::ToRequestTimeOut, this, &MeteoMediator::OnRequestTimeOut);
    connect(m_makeNewRequestTimer, &QTimer::timeout, this, &MeteoMediator::OnMakeNewRequest);
    connect(m_meteoMessageParser, &MeteoMessageGetter::ToAllDataCollected, this, &MeteoMediator::OnAllDataCollected);
}

void MeteoMediator::OnMakeNewRequest()
{
    if (m_meteoServer->IsMeteoConnected())
    {
        m_logger->Appends("MM: Запрашиваем метео");
        m_messagesToSendQueue->clear();
        m_messagesToSendQueue->enqueue(CreateMessage(60, 1));
        m_messagesToSendQueue->enqueue(CreateMessage(60, 2));
        m_messagesToSendQueue->enqueue(CreateMessage(60, 3));
        SendingNextMessageInQueue();
    }
    else
    {
        m_logger->Appends("MM: Запрос, хотя метео не подключен");
        Q_EMIT ToSendRarmMeteoState(m_meteoMessageParser->NoConnectionMessage());
    }
}

void MeteoMediator::OnGetStateFromMessage(const QByteArray &message)
{
    m_meteoMessageParser->ParseMessage(message);
    SendingNextMessageInQueue();
}

void MeteoMediator::OnRequestTimeOut()
{
    m_logger->Appends("MM: Метео таймаут");
    m_messagesToSendQueue->clear();
    Q_EMIT ToSendRarmMeteoState(m_meteoMessageParser->MessageTimeOut());
}

void MeteoMediator::SendingNextMessageInQueue()
{
    if (!m_messagesToSendQueue->empty())
    {
        m_meteoServer->SendMessage(m_messagesToSendQueue->dequeue());
    }
}

void MeteoMediator::OnAllDataCollected()
{
    m_logger->Appends("MM: Метео высылаем в рарм");
    Q_EMIT ToSendRarmMeteoState(m_meteoMessageParser->GetMessage());
}

bool MeteoMediator::IsMeteoConnected() const noexcept
{
    return m_meteoServer->IsMeteoConnected();
}

quint16 MeteoMediator::GetMeteoPort() const noexcept
{
    return m_meteoServer->GetMeteoPort();
}

std::string MeteoMediator::GetMeteoData() const noexcept
{
    const DevicesMeteoKitGetMessage &message = m_meteoMessageParser->GetMessage();
    if (0 == message.state)
    {
        return "\n       Нет связи";
    }
    else
    {
        if (5 == message.state)
        {
            return "\n       Давление: " + std::to_string(message.pressure) +
                   "\n       Температура: " + std::to_string(message.temperature) +
                   "\n       Влажность: " + std::to_string(message.wet) +
                   "\n       Для последнего сообщения вышело время ожидания(тайм аут)";
        }
        else
        {
            return "\n       Давление: " + std::to_string(message.pressure) +
                   "\n       Температура: " + std::to_string(message.temperature) +
                   "\n       Влажность: " + std::to_string(message.wet);
        }
    }
}

std::string MeteoMediator::GetMeteoError() const noexcept
{
    return m_meteoServer->GetMeteoError();
}

std::string MeteoMediator::GetLastMessageTime() const noexcept
{
    return m_meteoMessageParser->GetLastMessageTime();
}

const QByteArray MeteoMediator::CreateMessage(quint16 time_average, quint8 num_command) const
{
    const quint8 null_int = 0;
    QByteArray message;

    message.append(0xAA);
    message.append(0x55);
    message.append(time_average % 256);
    message.append(time_average / 256);
    message.append(num_command);
    message.append(null_int);
    message.append(null_int);
    message.append(null_int);
    message.append(null_int);
    message.append(null_int);

    quint8 check_sum = 0;
    for (int i = 2; i < message.count(); i++)
    {
        check_sum = check_sum ^ message[i];
    }
    message.append(check_sum);
    return message;
}

