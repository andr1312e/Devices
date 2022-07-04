#include "mediators/ustirovmediator.h"

UstirovMediator::UstirovMediator(const Logger *logger, const QString &moxaIpAdress, const QString &settingsFileName, QObject *parent)
    : QObject(parent)
    , m_moxaIpAdress(moxaIpAdress)
    , m_isRestartMode(false)
    , m_logger(logger)
    , m_pingTimer(new QTimer(this))
{
    ReadDataFromSettingsFile(settingsFileName);
    CreateObjects();
    StartPingTimer();
    ConnectObjects();
}

UstirovMediator::~UstirovMediator()
{
    delete m_pingTimer;
    delete m_ustirovSocket;
    delete m_ustirovMessageSetter;
    delete m_ustirovMessageGetter;
    delete m_ustirovMessageRepository;
}

void UstirovMediator::ReadDataFromSettingsFile(const QString &settingsFileName)
{
    const QString ustirovKit=ProFile::GetProFileField(QLatin1Literal("ustirovport"));
    if(ustirovKit.isEmpty())
    {
        m_ustirovPort=4004;
        ProFile::SetProfileField(QLatin1Literal("ustirovport"), QString::number(m_ustirovPort));
    }
    else
    {
        bool isNum=false;
        m_ustirovPort = ustirovKit.toUInt(&isNum);
        if(!isNum)
        {
            m_ustirovPort=4004;
            ProFile::SetProfileField(QLatin1Literal("ustirovport"), QString::number(m_ustirovPort));
        }
    }


    const QString fString=ProFile::GetProFileField(QLatin1Literal("f"));
    if(fString.isEmpty())
    {
        f=30250000;
        ProFile::SetProfileField(QLatin1Literal("f"), QString::number(f));
    }
    else
    {
        bool isNum=false;
        f = fString.toUInt(&isNum);
        if(!isNum)
        {
            f=30250000;
            ProFile::SetProfileField(QLatin1Literal("f"), QString::number(f));
        }
    }


    const QString frefString=ProFile::GetProFileField(QLatin1Literal("fref"));
    if(frefString.isEmpty())
    {
        fref=40000000;
        ProFile::SetProfileField(QLatin1Literal("fref"), QString::number(fref));
    }
    else
    {
        bool isNum=false;
        fref = frefString.toUInt(&isNum);
        if(!isNum)
        {
            fref=40000000;
            ProFile::SetProfileField(QLatin1Literal("fref"), QString::number(fref));
        }
    }
}

void UstirovMediator::CreateObjects()
{
    m_ustirovSocket = new UstirovSocket(m_logger, m_moxaIpAdress, m_ustirovPort, this);
    m_ustirovMessageRepository = new UstrirovMessageRepository();
    m_ustirovMessageSetter = new UstirovMessageSender(m_logger, f, fref);
    m_ustirovMessageGetter = new UstirovMessageGetter(f, fref, m_ustirovMessageRepository, this);
}

void UstirovMediator::ConnectObjects()
{
    connect(m_ustirovSocket, &UstirovSocket::ToGetStateFromMessage, this, &UstirovMediator::OnGetStateFromMessage);
    connect(m_ustirovSocket, &UstirovSocket::ToResetQueue, this, &UstirovMediator::OnResetQueue);
    connect(m_ustirovSocket, &UstirovSocket::ToRequestTimeOut, this, &UstirovMediator::OnRequestTimeOut);
    connect(m_ustirovSocket, &UstirovSocket::ToWantNextMessage, this, &UstirovMediator::OnSendMessage);
    connect(m_ustirovMessageGetter, &UstirovMessageGetter::ToAllDataCollected, this, &UstirovMediator::OnAllDataCollected);
    connect(m_pingTimer, &QTimer::timeout, this, &UstirovMediator::OnSendPing);
}

void UstirovMediator::StartPingTimer()
{
    m_pingTimer->setTimerType(Qt::VeryCoarseTimer);
    m_pingTimer->start(3500);
}

void UstirovMediator::OnResetQueue()
{
    m_messagesToSendList.clear();
}

void UstirovMediator::OnAllDataCollected()
{
    m_logger->Appends("UM: Отправляем сообщение в рарм");
    const DevicesAdjustingKitMessage &message = m_ustirovMessageGetter->GetMessage();
    Q_EMIT ToSendRarmUstirovState(message);
}

void UstirovMediator::OnSendMessage()
{
    if (m_pingTimer->isActive())
    {
        Q_EMIT ToSendPcbWork();
    }
    else
    {

        if (m_messagesToSendList.isEmpty())
        {
            m_pingTimer->start();
        }
        else
        {
            m_logger->Appends("UM: Высылаем сообщение " + m_messagesToSendList.front().toHex().toStdString());
            const QByteArray &frontMessage = m_messagesToSendList.front();
            m_ustirovSocket->SendMessage(frontMessage, m_isRestartMode);
            m_messagesToSendList.removeFirst();
        }
    }
}

void UstirovMediator::OnSendPing()
{
    m_ustirovSocket->SendMessage(m_ustirovMessageSetter->CreateZeroCommand(), false);
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
    return m_messagesToSendList.count();
}

QList<QByteArray> UstirovMediator::GetMessageList() const
{
    return m_messagesToSendList;
}

void UstirovMediator::RestartCommandsCreate()
{
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateRestartCommand());
}

void UstirovMediator::OnGetStateFromMessage(const QByteArray &message)
{
    if (m_ustirovMessageGetter->FillDataIntoStructFromMessage(message))
    {

        OnSendMessage();
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
    m_pingTimer->stop();
    if (m_ustirovSocket->IsUstirovConnected())
    {
        if (3 == state.state)
        {
            m_logger->Appends("UM: Перезагружаем устройство");
            m_messagesToSendList.clear();
            m_isRestartMode = true;
            RestartCommandsCreate();
            OnSendMessage();
        }
        else if (4 == state.state)
        {
            m_logger->Appends("UM: Считываем данные");
            m_isRestartMode = false;
            m_messagesToSendList.clear();
            GetStateCommandsCreate();
        }
        else
        {
            m_logger->Appends("UM: Пишем данные в юстировочный");
            m_isRestartMode = false;
            m_ustirovMessageRepository->SetDistanceToLocator(state.DistanceToLocator);
            m_messagesToSendList.clear();
            SetStateCommandsCreate(state);
            GetStateCommandsCreate();
            OnSendMessage();
        }
    }
    else
    {
        m_isRestartMode = false;
        SendToRarmMessageWithNoConnectionInfo();
    }

}

void UstirovMediator::OnGetDataFromUstirov()
{
    m_logger->Appends("UM: Берем данные из юстировочного");
    if (m_ustirovSocket->IsUstirovConnected())
    {
        m_messagesToSendList.clear();
        GetStateCommandsCreate();
        OnSendMessage();
    }
    else
    {
        SendToRarmMessageWithNoConnectionInfo();
    }
}

void UstirovMediator::SetStateCommandsCreate(const DevicesAdjustingKitMessage &state)
{
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateSixCommand(0.0));
    if (m_ustirovMessageRepository->GetFvco() != state.Fvco)
    {
        m_messagesToSendList.append(m_ustirovMessageSetter->CreateFirstCommand(state.Fvco));
    }
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateSecondCommand(state.Fvco, state.DoplerFrequency));
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateThirdCommand(state.Distance, state.DistanceToLocator));
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateFourthCommand(state.GAIN_TX, state.GAIN_RX));
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateFiveCommand(state.Attenuator));
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateSixCommand(state.WorkMode));
}

void UstirovMediator::GetStateCommandsCreate()
{
    for (int id = 1; id < 7; ++id)
    {
        m_messagesToSendList.append(m_ustirovMessageSetter->CreateSevenCommand(id));
    }
}

void UstirovMediator::SendToRarmMessageWithNoConnectionInfo()
{
    m_ustirovMessageGetter->SetNoConnectionState();
    Q_EMIT ToSendRarmUstirovState(m_ustirovMessageGetter->GetMessage());
}
