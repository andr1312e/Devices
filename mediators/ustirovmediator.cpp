#include "mediators/ustirovmediator.h"

UstirovMediator::UstirovMediator(const QString &settingsFileName, QObject *parent)
    : QObject(parent)
{
    ReadDataFromSettingsFile(settingsFileName);
    CreateObjects();
    ConnectObjects();
}

UstirovMediator::~UstirovMediator()
{
    delete m_messagesToSendQueue;
    delete m_ustirovSocket;
    delete m_setMessageCreator;
    delete m_stateMessageGetter;
}

void UstirovMediator::ReadDataFromSettingsFile(const QString &settingsFileName)
{
    QSettings settings(settingsFileName, QSettings::IniFormat, this);
    if (settings.contains(QStringLiteral("ustirovadress")))
    {
        m_ustirovAdress=settings.value(QStringLiteral("ustirovadress"), "192.168.127.254").toString();
    }
    else
    {
        m_ustirovAdress=QStringLiteral("192.168.127.254");
        settings.setValue(QStringLiteral("ustirovadress"), m_ustirovAdress);
    }
    if (settings.contains(QStringLiteral("ustirovport")))
    {
        m_ustirovPort=settings.value(QStringLiteral("ustirovport"), 4004).toUInt();
    }
    else
    {
        m_ustirovPort=4004;
        settings.setValue(QStringLiteral("ustirovport"), m_ustirovPort);

    }
    settings.sync();
}

void UstirovMediator::CreateObjects()
{
    m_ustirovSocket=new UstirovSocket(m_ustirovAdress, m_ustirovPort, this);
    m_setMessageCreator=new StateMessageSender();
    m_stateMessageGetter=new StateMessageGetter(this);
    m_messagesToSendQueue=new std::queue<QByteArray>();
}

void UstirovMediator::ConnectObjects()
{
    connect(m_ustirovSocket, &UstirovSocket::ToStateGettingFromMessage, this, &UstirovMediator::OnGetMessageWithState);
    connect(m_ustirovSocket, &UstirovSocket::ToSendingNext, this, &UstirovMediator::OnSendingNext);
    connect(m_ustirovSocket, &UstirovSocket::ToResetQueue, this, &UstirovMediator::OnResetQueue);
    connect(m_stateMessageGetter, &StateMessageGetter::ToAllDataCollected, this, &UstirovMediator::OnAllDataCollected);
}

void UstirovMediator::OnSendingNext()
{
    if(m_messagesToSendQueue->empty())
    {
        qDebug()<< "UM - All messages send";
    }
    else
    {
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
    DevicesAdjustingKitMessage message=m_stateMessageGetter->GetMessage();
    Q_EMIT ToSendRarmUstirovState(message);
}

void UstirovMediator::OnGetMessageWithState(const QByteArray &message)
{
    if (m_stateMessageGetter->FillDataIntoStructFromMessage(message))
    {
        OnSendingNext();
    }
    else
    {
        m_ustirovSocket->TryToSendLastMessageAgain();
    }
}

void UstirovMediator::OnNoAnswerGet()
{
    m_stateMessageGetter->SetBadState();
    Q_EMIT ToSendRarmUstirovState(m_stateMessageGetter->GetMessage());
}

void UstirovMediator::OnSetUstirovState(const DevicesAdjustingKitMessage &state)
{
    if(m_ustirovSocket->IsUstirovConnected())
    {
        SetStateCommandsCreate(state);
        OnSendingNext();
    }
    Q_EMIT ToSendRarmUstirovState(m_stateMessageGetter->GetMessage());
}

void UstirovMediator::OnGetUstirovState()
{
    if(m_ustirovSocket->IsUstirovConnected())
    {
        GetStateCommandsCreate();
        OnSendingNext();
    }
    else
    {
        m_stateMessageGetter->SetBadState();
        Q_EMIT ToSendRarmUstirovState(m_stateMessageGetter->GetMessage());
    }
}

void UstirovMediator::SetStateCommandsCreate(const DevicesAdjustingKitMessage &state)
{
    OnResetQueue();
    m_messagesToSendQueue->push(m_setMessageCreator->createFirstCommand(state.Fvco));
    m_messagesToSendQueue->push(m_setMessageCreator->createSecondCommand(state.Fvco, state.DoplerFrequency));
    m_messagesToSendQueue->push(m_setMessageCreator->createThirdCommand(state.Distance));
    m_messagesToSendQueue->push(m_setMessageCreator->createFourthCommand(state.GAIN_TX, state.GAIN_RX));
    m_messagesToSendQueue->push(m_setMessageCreator->createFiveCommand(state.Attenuator));
    m_messagesToSendQueue->push(m_setMessageCreator->createSixCommand(state.WorkMode, state.PhaseIncrement));
}

void UstirovMediator::GetStateCommandsCreate()
{
    OnResetQueue();
    for (int i=1; i<7; i++)
    {
        m_messagesToSendQueue->push(m_setMessageCreator->createSevenCommand(i));
    }
}
