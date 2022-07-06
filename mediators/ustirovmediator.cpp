#include "mediators/ustirovmediator.h"

UstirovMediator::UstirovMediator(const Logger *logger, const QString &moxaIpAdress, QObject *parent)
    : QObject(parent)
    , m_moxaIpAdress(moxaIpAdress)
    , m_isRestartMode(false)
    , m_logger(logger)
    , m_pingTimer(new QTimer(this))
{
    ReadParamsFromProfile();
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

void UstirovMediator::ReadParamsFromProfile()
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
        m_f=30250000;
        ProFile::SetProfileField(QLatin1Literal("f"), QString::number(m_f));
    }
    else
    {
        bool isNum=false;
        m_f = fString.toUInt(&isNum);
        if(!isNum)
        {
            m_f=30250000;
            ProFile::SetProfileField(QLatin1Literal("f"), QString::number(m_f));
        }
    }


    const QString frefString=ProFile::GetProFileField(QLatin1Literal("fref"));
    if(frefString.isEmpty())
    {
        m_fref=40000000;
        ProFile::SetProfileField(QLatin1Literal("fref"), QString::number(m_fref));
    }
    else
    {
        bool isNum=false;
        m_fref = frefString.toUInt(&isNum);
        if(!isNum)
        {
            m_fref=40000000;
            ProFile::SetProfileField(QLatin1Literal("fref"), QString::number(m_fref));
        }
    }
}

void UstirovMediator::CreateObjects()
{
    m_ustirovSocket = new UstirovSocket(m_logger, m_moxaIpAdress, m_ustirovPort, this);
    m_ustirovMessageRepository = new UstrirovMessageRepository();
    m_ustirovMessageSetter = new UstirovMessageSender(m_logger, m_f, m_fref);
    m_ustirovMessageGetter = new UstirovMessageGetter(m_f, m_fref, m_ustirovMessageRepository, this);
}

void UstirovMediator::ConnectObjects()
{
    connect(m_ustirovSocket, &UstirovSocket::ToGetStateFromMessage, this, &UstirovMediator::OnGetStateFromMessage);
    connect(m_ustirovSocket, &UstirovSocket::ToResetQueue, this, &UstirovMediator::OnResetQueue);
    connect(m_ustirovSocket, &UstirovSocket::ToRequestTimeOut, this, &UstirovMediator::OnRequestTimeOut);
    connect(m_ustirovSocket, &UstirovSocket::ToWantNextMessage, this, &UstirovMediator::OnSendMessage);
    connect(m_ustirovMessageGetter, &UstirovMessageGetter::ToAllNormalDataCollected, this, &UstirovMediator::OnAllNormalDataCollected);
    connect(m_ustirovMessageGetter, &UstirovMessageGetter::ToAllBparDataCollected, this, &UstirovMediator::OnAllBparDataCollected);
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

void UstirovMediator::OnAllNormalDataCollected()
{
    m_logger->Appends("UM: Отправляем сообщение нормальное в рарм");
    const DevicesAdjustingKitMessage &message = m_ustirovMessageGetter->GetMessageNormal();
    Q_EMIT ToSendRarmNormalState(message);
}

void UstirovMediator::OnAllBparDataCollected()
{
    m_logger->Appends("UM: Отправляем сообщение бпар в рарм");
    const DevicesBparAdjustingKitMessage &message = m_ustirovMessageGetter->GetMessageBpar();
    Q_EMIT ToSendRarmBparState(message);
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

QString UstirovMediator::GetLastUstirovSocketErrorMessage() const
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
    Q_EMIT ToSendRarmNormalState(m_ustirovMessageGetter->GetMessageNormal());
}

void UstirovMediator::OnSetNormalSateToUstirov(const DevicesAdjustingKitMessage &state)
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
            m_logger->Appends("UM: Считываем нормальные данные");
            m_isRestartMode = false;
            m_messagesToSendList.clear();
            GetNormalStateCommandsCreate();
        }
        else
        {
            m_logger->Appends("UM: Пишем нормальные данные в юстировочный");
            m_isRestartMode = false;
            m_ustirovMessageRepository->SetDistanceToLocator(state.DistanceToLocator);
            m_messagesToSendList.clear();
            SetStateCommandsCreate(state);
            GetNormalStateCommandsCreate();
            OnSendMessage();
        }
    }
    else
    {
        m_isRestartMode = false;
        SendToRarmMessageWithNoConnectionInfo();
    }

}

void UstirovMediator::OnSetBparStateToUstirov(const DevicesBparAdjustingKitMessage &state)
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
            m_logger->Appends("UM: Считываем БПАР данные");
            m_isRestartMode = false;
            m_messagesToSendList.clear();
            m_messagesToSendList.append(m_ustirovMessageSetter->CreateSevenCommand(5));
        }
        else
        {
            m_logger->Appends("UM: Пишем данные БПАР в юстировочный");
            m_isRestartMode = false;
            m_ustirovMessageRepository->SetDistanceToLocator(state.DistanceToLocator);
            m_messagesToSendList.clear();
            m_messagesToSendList.append(m_ustirovMessageSetter->CreateBparCommand(state));
            m_messagesToSendList.append(m_ustirovMessageSetter->CreateSevenCommand(5));
            OnSendMessage();
        }
    }
    else
    {
        m_isRestartMode = false;
        SendToRarmMessageWithNoConnectionInfo();
    }
}

void UstirovMediator::SetStateCommandsCreate(const DevicesAdjustingKitMessage &state)
{
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateSixCommand(0.0));
    if (m_ustirovMessageRepository->GetFvco() != state.FvcoRx)
    {
        m_messagesToSendList.append(m_ustirovMessageSetter->CreateFirstCommand(state.FvcoRx));
    }
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateSecondCommand(state.FvcoRx));
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateThirdCommand(state.Distance, state.DistanceToLocator));
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateFourthCommand(state.GAIN_TX, state.GAIN_RX));
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateFiveCommand(state.Attenuator));
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateSixCommand(state.WorkMode));
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateNineCommand(state.DoplerFrequency));
}

void UstirovMediator::GetNormalStateCommandsCreate()
{
    for (int id = 1; id < 7; ++id)
    {
        m_messagesToSendList.append(m_ustirovMessageSetter->CreateSevenCommand(id));
    }
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateSevenCommand(9));
}

void UstirovMediator::SendToRarmMessageWithNoConnectionInfo()
{
    m_ustirovMessageGetter->SetNoConnectionStateNormal();
    Q_EMIT ToSendRarmNormalState(m_ustirovMessageGetter->GetMessageNormal());
}
