#include "mediators/ustirovmediator.h"

UstirovMediator::UstirovMediator(const Logger *logger, const QString &moxaIpAdress, const QString &settingsFileName, QObject *parent)
    : QObject(parent)
    , m_moxaIpAdress(moxaIpAdress)
    , m_logger(logger)
{
    ReadDataFromSettingsFile(settingsFileName);
    CreateObjects();
    ConnectObjects();
}

UstirovMediator::~UstirovMediator()
{
    delete m_sendNextMessageTimer;
    delete m_ustirovSocket;
    delete m_ustirovMessageSetter;
    delete m_ustirovMessageGetter;
    delete m_ustirovMessageRepository;
    delete m_messagesToSendList;
}

void UstirovMediator::ReadDataFromSettingsFile(const QString &settingsFileName)
{
    QSettings mediatorSettings(settingsFileName, QSettings::IniFormat, this);
    if (mediatorSettings.contains(QStringLiteral("ustirovport")))
    {
        m_ustirovPort = mediatorSettings.value(QStringLiteral("ustirovport"), 4004).toUInt();
    }
    else
    {
        m_ustirovPort = 4004;
        mediatorSettings.setValue(QStringLiteral("ustirovport"), m_ustirovPort);

    }
    if (mediatorSettings.contains(QStringLiteral("f")))
    {
        f = mediatorSettings.value(QStringLiteral("f")).toDouble();
    }
    else
    {
        f = 32650000.0;
        mediatorSettings.setValue(QStringLiteral("f"), f);
    }
    if (mediatorSettings.contains(QStringLiteral("fref")))
    {
        fref = mediatorSettings.value(QStringLiteral("fref")).toDouble();
    }
    else
    {
        fref = 40000000.0;
        mediatorSettings.setValue(QStringLiteral("fref"), fref);
    }
    mediatorSettings.sync();
}

void UstirovMediator::CreateObjects()
{

    m_ustirovSocket = new UstirovSocket(m_logger, m_moxaIpAdress, m_ustirovPort, this);
    m_ustirovMessageRepository = new UstrirovMessageRepository();
    m_ustirovMessageSetter = new UstirovMessageSender(f, fref);
    m_ustirovMessageGetter = new UstirovMessageGetter(f, fref, m_ustirovMessageRepository, this);
    m_messagesToSendList = new QLinkedList<QByteArray>();
    m_sendNextMessageTimer = new QTimer(this);
    m_sendNextMessageTimer->setSingleShot(true);
    m_sendNextMessageTimer->setInterval(3200);
}

void UstirovMediator::ConnectObjects()
{
    connect(m_ustirovSocket, &UstirovSocket::ToGetStateFromMessage, this, &UstirovMediator::OnGetStateFromMessage);
    connect(m_ustirovSocket, &UstirovSocket::ToResetQueue, this, &UstirovMediator::OnResetQueue);
    connect(m_ustirovSocket, &UstirovSocket::ToRequestTimeOut, this, &UstirovMediator::OnRequestTimeOut);
    connect(m_ustirovSocket, &UstirovSocket::ToWantNextMessage, this, &UstirovMediator::OnWantedNextMessage);
    connect(m_ustirovMessageGetter, &UstirovMessageGetter::ToAllDataCollected, this, &UstirovMediator::OnAllDataCollected);
    connect(m_sendNextMessageTimer, &QTimer::timeout, this, &UstirovMediator::OnTimeToSendMessage);
}

void UstirovMediator::OnWantedNextMessage()
{
    if (!m_messagesToSendList->empty())
    {
        m_sendNextMessageTimer->start();
    }
}

void UstirovMediator::OnResetQueue()
{
    m_messagesToSendList->clear();
}

void UstirovMediator::OnAllDataCollected()
{
    m_logger->Appends("UM: Отправляем сообщение в рарм");
    const DevicesAdjustingKitMessage message = m_ustirovMessageGetter->GetMessage();
    Q_EMIT ToSendRarmUstirovState(message);
}

void UstirovMediator::OnTimeToSendMessage()
{
    m_logger->Appends("UM: Высылаем сообщение " + m_messagesToSendList->front().toHex().toStdString());
    m_ustirovSocket->SendMessage(m_messagesToSendList->front());
    m_messagesToSendList->removeFirst();
}

quint16 UstirovMediator::GetUstirovPort() const
{
    return m_ustirovPort;
}

bool UstirovMediator::IsUstirovConnected() const
{
    return m_ustirovSocket->IsUstirovConnected();
}

QString UstirovMediator::GetLastUstirovErrorMessage() const
{
    return m_ustirovSocket->GetLastUstirovErrorMessage();
}

int UstirovMediator::GetMessagesCount() const
{
    return m_messagesToSendList->count();
}

QLinkedList<QByteArray> *UstirovMediator::GetMessageList() const
{
    return m_messagesToSendList;
}

void UstirovMediator::RestartCommandsCreate()
{
    m_messagesToSendList->append(m_ustirovMessageSetter->CreateRestartCommand);
}

void UstirovMediator::OnGetStateFromMessage(const QByteArray &message)
{
    if (m_ustirovMessageGetter->FillDataIntoStructFromMessage(message))
    {
        OnWantedNextMessage();
    }
    else
    {
        m_ustirovSocket->TryToSendLastMessageAgain();
    }
}

void UstirovMediator::OnRequestTimeOut()
{
    m_logger->Appends("UM: Время ожидания истекло");
    m_ustirovMessageGetter->SetTimeOutState();
    Q_EMIT ToSendRarmUstirovState(m_ustirovMessageGetter->GetMessage());
}

void UstirovMediator::OnSetDataToUstirov(const DevicesAdjustingKitMessage &state)
{
    if (3 == state)
    {
        m_logger->Appends("UM: Перезагружаем устройство");
        if (m_ustirovSocket->IsUstirovConnected())
        {
            m_messagesToSendList->clear();
            RestartCommandsCreate();
            OnWantedNextMessage();
        }
    }
    else
    {
        m_logger->Appends("UM: Пишем данные в юстировочный");
        m_ustirovMessageRepository->SetDistanceToLocator(state.DistanceToLocator);
        if (m_ustirovSocket->IsUstirovConnected())
        {
            m_messagesToSendList->clear();
            SetStateCommandsCreate(state);
            //        GetStateCommandsCreate();
            OnWantedNextMessage();
        }
        else
        {
            SendToRarmMessageWithNoConnectionInfo();
        }
    }

}

void UstirovMediator::OnGetDataFromUstirov()
{
    m_logger->Appends("UM: Берем данные из юстировочного");
    if (m_ustirovSocket->IsUstirovConnected())
    {
        m_messagesToSendList->clear();
        GetStateCommandsCreate();
        OnWantedNextMessage();
    }
    else
    {
        SendToRarmMessageWithNoConnectionInfo();
    }
}

void UstirovMediator::SetStateCommandsCreate(const DevicesAdjustingKitMessage &state)
{
    m_messagesToSendList->append(m_ustirovMessageSetter->CreateSixCommand(false));
    m_messagesToSendList->append(m_ustirovMessageSetter->CreateFirstCommand(state.Fvco));
    m_messagesToSendList->append(m_ustirovMessageSetter->CreateSecondCommand(state.Fvco, state.DoplerFrequency));
    m_messagesToSendList->append(m_ustirovMessageSetter->CreateThirdCommand(state.Distance, state.DistanceToLocator));
    m_messagesToSendList->append(m_ustirovMessageSetter->CreateFourthCommand(state.GAIN_TX, state.GAIN_RX));
    m_messagesToSendList->append(m_ustirovMessageSetter->CreateFiveCommand(state.Attenuator));
    m_messagesToSendList->append(m_ustirovMessageSetter->CreateSixCommand(state.WorkMode));
}

void UstirovMediator::GetStateCommandsCreate()
{
    for (int id = 1; id < 3; ++id)
    {
        m_messagesToSendList->append(m_ustirovMessageSetter->CreateSevenCommand(id));
    }
    //3 сообщения не было с предпоследней прошивкой, узнай если с этой у Юры
    for (int id = 4; id < 7; ++id)
    {
        m_messagesToSendList->append(m_ustirovMessageSetter->CreateSevenCommand(id));
    }
}

void UstirovMediator::SendToRarmMessageWithNoConnectionInfo()
{
    m_ustirovMessageGetter->SetNoConnectionState();
    Q_EMIT ToSendRarmUstirovState(m_ustirovMessageGetter->GetMessage());
}
