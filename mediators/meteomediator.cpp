#include "mediators/meteomediator.h"

MeteoMediator::MeteoMediator(const QString &settingsFileName, QObject *parent)
    : QObject(parent)
{
    ReadDataFromSettingsFile(settingsFileName);
    CreateObjects();
    InitObjects();
    ConnectObjects();
}

MeteoMediator::~MeteoMediator()
{
    delete m_meteoServer;
    delete m_messagesToSendQueue;
    delete m_meteoMessageGetter;
    delete m_makeNewRequestTimer;
}

void MeteoMediator::ReadDataFromSettingsFile(const QString &settingsFileName)
{
    QSettings settings(settingsFileName, QSettings::IniFormat, this);
    if (settings.contains(QStringLiteral("meteoRequestInterval")))
    {
        m_meteoRequestInterval=settings.value(QStringLiteral("meteoRequestInterval"), 5000).toUInt();
    }
    else
    {
        m_meteoRequestInterval=5000;
        settings.setValue(QStringLiteral("meteoRequestInterval"), m_meteoRequestInterval);
    }

    if (settings.contains(QStringLiteral("meteoTimeOutInterval")))
    {
        m_meteoTimeOutInterval=settings.value(QStringLiteral("meteoTimeOutInterval"), 500).toUInt();
    }
    else
    {
        m_meteoTimeOutInterval=500;
        settings.setValue(QStringLiteral("meteoTimeOutInterval"), m_meteoTimeOutInterval);
    }

    if (settings.contains(QStringLiteral("moxaPort")))
    {
        m_moxaPort=settings.value(QStringLiteral("moxaPort"), 4101).toUInt();
    }
    else
    {
        m_moxaPort=4101;
        settings.setValue(QStringLiteral("moxaPort"), m_moxaPort);

    }

    if (settings.contains(QStringLiteral("meteoKitPort")))
    {
        m_meteoPort=settings.value(QStringLiteral("meteoKitPort"), 5011).toUInt();
    }
    else
    {
        m_meteoPort=5011;
        settings.setValue(QStringLiteral("meteoKitPort"), m_meteoPort);

    }
    settings.sync();
}

void MeteoMediator::CreateObjects()
{
    m_meteoServer=new MeteoServer(m_moxaPort, m_meteoPort, this);
    m_meteoMessageGetter=new MeteoMessageGetter(this);
    m_messagesToSendQueue=new std::queue<QByteArray>();
    m_makeNewRequestTimer=new QTimer(this);
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
    connect(m_meteoServer, &MeteoServer::ToResetQueue, this, &MeteoMediator::OnClearQueue);
    connect(m_meteoServer, &MeteoServer::ToRequestTimeOut, this, &MeteoMediator::OnRequestTimeOut);
    connect(m_makeNewRequestTimer, &QTimer::timeout, this, &MeteoMediator::OnMakeNewRequest);
    connect(m_meteoMessageGetter, &MeteoMessageGetter::ToAllDataCollected, this, &MeteoMediator::OnAllDataCollected);
}

void MeteoMediator::OnMakeNewRequest()
{
    if (m_meteoServer->IsMeteoConnected())
    {
//         qDebug()<< "MM : make request to get meteo";
        OnClearQueue();
        m_messagesToSendQueue->push(CreateMessage(60, 1));
        m_messagesToSendQueue->push(CreateMessage(60, 2));
        m_messagesToSendQueue->push(CreateMessage(60, 3));
        SendingNextMessageInQueue();
    }
    else
    {
//        qDebug()<< "MM : send rarm meteo not connected";
        Q_EMIT ToSendRarmMeteoState(m_meteoMessageGetter->NoConnectionMessage());
    }
}

void MeteoMediator::OnGetStateFromMessage(const QByteArray &message)
{
    m_meteoMessageGetter->ParseMessage(message);
    SendingNextMessageInQueue();
}

void MeteoMediator::OnRequestTimeOut()
{
//    qDebug()<< "MM : meteo timeout";
    OnClearQueue();
    Q_EMIT ToSendRarmMeteoState(m_meteoMessageGetter->MessageTimeOut());
}


void MeteoMediator::OnClearQueue()
{
    while(!m_messagesToSendQueue->empty())
    {
        m_messagesToSendQueue->pop();
    }
}

void MeteoMediator::SendingNextMessageInQueue()
{
    if(!m_messagesToSendQueue->empty())
    {
        m_meteoServer->SendMessage(m_messagesToSendQueue->front());
        m_messagesToSendQueue->pop();
    }
}

void MeteoMediator::OnAllDataCollected()
{
//    qDebug()<< "MM : send meteo to rarm";
    Q_EMIT ToSendRarmMeteoState(m_meteoMessageGetter->GetMessage());
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
    for(int i = 2; i < message.count(); i++)
    {
        check_sum = check_sum^message[i];
    }
    message.append(check_sum);
    return message;
}

