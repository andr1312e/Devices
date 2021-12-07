#include "mediators/ustirovmediator.h"

UstirovMediator::UstirovMediator(const QString &moxaIpAdress, const QString &settingsFileName, QObject *parent)
    : QObject(parent)
    , m_moxaIpAdress(moxaIpAdress)
{
    ReadDataFromSettingsFile(settingsFileName);
    CreateObjects();
    ConnectObjects();
}

UstirovMediator::~UstirovMediator()
{
    delete m_ustirovSocket;
    delete m_ustirovMessageSetter;
    delete m_ustirovMessageGetter;
}

void UstirovMediator::ReadDataFromSettingsFile(const QString &settingsFileName)
{
    QSettings mediatorSettings(settingsFileName, QSettings::IniFormat, this);
    if (mediatorSettings.contains(QStringLiteral("ustirovport")))
    {
        m_ustirovPort=mediatorSettings.value(QStringLiteral("ustirovport"), 4004).toUInt();
    }
    else
    {
        m_ustirovPort=4004;
        mediatorSettings.setValue(QStringLiteral("ustirovport"), m_ustirovPort);

    }
    if (mediatorSettings.contains(QStringLiteral("f")))
    {
        f=mediatorSettings.value(QStringLiteral("f")).toDouble();
    }
    else
    {
        f=32650000.0;
        mediatorSettings.setValue(QStringLiteral("f"), f);
    }
    if(mediatorSettings.contains(QStringLiteral("fref")))
    {
        fref=mediatorSettings.value(QStringLiteral("fref")).toDouble();
    }
    else
    {
        fref=40000000.0;
        mediatorSettings.setValue(QStringLiteral("fref"), fref);
    }
    mediatorSettings.sync();
}

void UstirovMediator::CreateObjects()
{
    m_ustirovSocket=new UstirovSocket(m_moxaIpAdress, m_ustirovPort, this);
    m_ustirovMessageRepository=QSharedPointer<UstrirovMessageRepository>(new UstrirovMessageRepository());
    m_ustirovMessageSetter=new UstirovMessageSender(f, fref, m_ustirovMessageRepository);
    m_ustirovMessageGetter=new UstirovMessageGetter(f, fref, m_ustirovMessageRepository, this);
    m_messagesToSendQueue=QSharedPointer<std::queue<QByteArray>>(new std::queue<QByteArray>());
}

void UstirovMediator::ConnectObjects()
{
    connect(m_ustirovSocket, &UstirovSocket::ToGetStateFromMessage, this, &UstirovMediator::OnGetStateFromMessage);
    connect(m_ustirovSocket, &UstirovSocket::ToResetQueue, this, &UstirovMediator::OnResetQueue);
    connect(m_ustirovSocket, &UstirovSocket::ToRequestTimeOut, this, &UstirovMediator::OnRequestTimeOut);
    connect(m_ustirovSocket, &UstirovSocket::ToSendingNext, this, &UstirovMediator::OnSendingNext);
    connect(m_ustirovMessageGetter, &UstirovMessageGetter::ToAllDataCollected, this, &UstirovMediator::OnAllDataCollected);
}

void UstirovMediator::OnSendingNext()
{
    if(m_messagesToSendQueue->empty())
    {
        qDebug()<< "UM: Message queue is clear";
    }
    else
    {
        qDebug()<< "UM: Front message from queue " << m_messagesToSendQueue->front().toHex();
        m_ustirovSocket->SendMessage(m_messagesToSendQueue->front());
        m_messagesToSendQueue->pop();
    }
}

void UstirovMediator::OnResetQueue()
{
    while(!m_messagesToSendQueue->empty())
    {
        m_messagesToSendQueue->pop();
    }
}

void UstirovMediator::OnAllDataCollected()
{
    qDebug()<< "UM: Ready to rarm get message send";
    DevicesAdjustingKitMessage message=m_ustirovMessageGetter->GetMessage();
    Q_EMIT ToSendRarmUstirovState(message);
}

void UstirovMediator::OnGetStateFromMessage(const QByteArray &message)
{
    if (m_ustirovMessageGetter->FillDataIntoStructFromMessage(message))
    {
        OnSendingNext();
    }
    else
    {
        m_ustirovSocket->TryToSendLastMessageAgain();
    }
}

void UstirovMediator::OnRequestTimeOut()
{
    qDebug()<< "UM: Timeout on request";
    m_ustirovMessageGetter->SetTimeOutState();
    Q_EMIT ToSendRarmUstirovState(m_ustirovMessageGetter->GetMessage());
}

void UstirovMediator::OnSetDataToUstirov(const DevicesAdjustingKitMessage &state)
{
    qDebug()<< "UM: Get from rarm request to set";
    m_ustirovMessageRepository->SetDistanceToLocator(state.DistanceToLocator);
    if(m_ustirovSocket->IsUstirovPcbConnected())
    {
        OnResetQueue();
        SetStateCommandsCreate(state);
        GetStateCommandsCreate();
        OnSendingNext();
    }
    else
    {
        SendToRarmMessageWithNoConnectionInfo();
    }
}

void UstirovMediator::OnGetDataFromUstirov()
{
    qDebug()<< "UM: Get from rarm request to set";
    if(m_ustirovSocket->IsUstirovPcbConnected())
    {
        OnResetQueue();
        GetStateCommandsCreate();
        OnSendingNext();
    }
    else
    {
        SendToRarmMessageWithNoConnectionInfo();
    }
}

void UstirovMediator::SetStateCommandsCreate(const DevicesAdjustingKitMessage &state)
{
    m_messagesToSendQueue->push(m_ustirovMessageSetter->CreateFirstCommand(state.Fvco));
    m_messagesToSendQueue->push(m_ustirovMessageSetter->CreateSecondCommand(state.Fvco, state.DoplerFrequency));
    m_messagesToSendQueue->push(m_ustirovMessageSetter->CreateThirdCommand(state.Distance));
    m_messagesToSendQueue->push(m_ustirovMessageSetter->CreateFourthCommand(state.GAIN_TX, state.GAIN_RX));
    m_messagesToSendQueue->push(m_ustirovMessageSetter->CreateFiveCommand(state.Attenuator));
    m_messagesToSendQueue->push(m_ustirovMessageSetter->CreateSixCommand(state.WorkMode, state.PhaseIncrement));
}

void UstirovMediator::GetStateCommandsCreate()
{
    for (int id=1; id<3; ++id)
    {
        m_messagesToSendQueue->push(m_ustirovMessageSetter->CreateSevenCommand(id));
    }
    for (int id=4; id<7; ++id)
    {
        m_messagesToSendQueue->push(m_ustirovMessageSetter->CreateSevenCommand(id));
    }
}

void UstirovMediator::SendToRarmMessageWithNoConnectionInfo()
{
    m_ustirovMessageGetter->SetNoConnectionState();
    Q_EMIT ToSendRarmUstirovState(m_ustirovMessageGetter->GetMessage());
}
