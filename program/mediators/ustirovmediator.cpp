#include "ustirovmediator.h"

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
    const QString ustirovKit = ProFile::GetProFileField(QLatin1String("ustirovport"));
    if (ustirovKit.isEmpty())
    {
        m_ustirovPort = 4004;
        ProFile::SetProfileField(QLatin1String("ustirovport"), QString::number(m_ustirovPort));
    }
    else
    {
        bool isNum = false;
        m_ustirovPort = ustirovKit.toUInt(&isNum);
        if (!isNum)
        {
            m_ustirovPort = 4004;
            ProFile::SetProfileField(QLatin1String("ustirovport"), QString::number(m_ustirovPort));
        }
    }


    const QString fString = ProFile::GetProFileField(QLatin1String("f"));
    if (fString.isEmpty())
    {
        m_f = 30625000;
        ProFile::SetProfileField(QLatin1String("f"), QString::number(m_f));
    }
    else
    {
        bool isNum = false;
        m_f = fString.toDouble(&isNum);
        if (!isNum)
        {
            m_f = 30625000;
            ProFile::SetProfileField(QLatin1String("f"), QString::number(m_f));
        }
    }


    const QString frefString = ProFile::GetProFileField(QLatin1String("fref"));
    if (frefString.isEmpty())
    {
        m_fref = 40000000;
        ProFile::SetProfileField(QLatin1String("fref"), QString::number(m_fref));
    }
    else
    {
        bool isNum = false;
        m_fref = frefString.toDouble(&isNum);
        if (!isNum)
        {
            m_fref = 40000000;
            ProFile::SetProfileField(QLatin1String("fref"), QString::number(m_fref));
        }
    }
}

void UstirovMediator::CreateObjects()
{
    m_ustirovSocket = new UstirovSocket(m_logger, m_moxaIpAdress, m_ustirovPort, this);
    m_ustirovMessageRepository = new UstrirovMessageRepository();
    m_ustirovMessageSetter = new UstirovMessageSender(m_logger, m_f, m_fref);
    m_ustirovMessageGetter = new UstirovMessageGetter(m_f, m_fref, *m_ustirovMessageRepository, m_geoRepository, this);
}

void UstirovMediator::ConnectObjects()
{
    connect(m_ustirovSocket, &UstirovSocket::ToGetStateFromMessage, this, &UstirovMediator::OnGetStateFromMessage);
    connect(m_ustirovSocket, &UstirovSocket::ToGetGeoFromMessage, this, &UstirovMediator::OnGetGeoFromMessage);
    connect(m_ustirovSocket, &UstirovSocket::ToResetQueue, this, &UstirovMediator::OnResetQueue);
    connect(m_ustirovSocket, &UstirovSocket::ToRequestTimeOut, this, &UstirovMediator::OnRequestTimeOut);
    connect(m_ustirovSocket, &UstirovSocket::ToWantNextMessage, this, &UstirovMediator::OnSendNextMessage);
    connect(m_ustirovMessageGetter, &UstirovMessageGetter::ToAllNormalDataCollected, this, &UstirovMediator::OnAllNormalDataCollected);
    connect(m_ustirovMessageGetter, &UstirovMessageGetter::ToAllBparDataCollected, this, &UstirovMediator::OnAllBparDataCollected);
    connect(m_ustirovMessageGetter, &UstirovMessageGetter::ToAllGeoCollected, this, &UstirovMediator::OnAllGeoCollected);
    connect(m_pingTimer, &QTimer::timeout, this, &UstirovMediator::OnSendPing);
}

void UstirovMediator::StartPingTimer()
{
    m_pingTimer->setTimerType(Qt::VeryCoarseTimer);
    m_pingTimer->start(std::chrono::seconds(4));
}

void UstirovMediator::OnResetQueue() noexcept
{
    m_messagesToSendList.clear();
}

void UstirovMediator::OnAllNormalDataCollected()
{
    m_logger->Appends("UM: Отправляем сообщение обычное в рарм");
    const DevicesAdjustingKitMessage &message = m_ustirovMessageGetter->GetMessageNormal();
//    m_logger->Appends("РX " + QString::number(message.FvcoRx).toStdString());
//    m_logger->Appends("TX " + QString::number(message.FvcoTx).toStdString());
//    m_logger->Appends("Дист " + QString::number(message.Distance).toStdString());
    Q_EMIT ToSendRarmNormalState(message);
}

void UstirovMediator::OnAllBparDataCollected()
{
    m_logger->Appends("UM: Отправляем сообщение бпар в рарм");
    const DevicesBparAdjustingKitMessage &message = m_ustirovMessageGetter->GetMessageBpar();
    Q_EMIT ToSendRarmBparState(message);
}

void UstirovMediator::OnAllGeoCollected()
{
    m_logger->Appends("UM: Отправляем гео в рарм");
    const DevicesGeoStateMessage &message = m_geoRepository.GetMessage();
    Q_EMIT ToSendRarmGeoState(message);
}

void UstirovMediator::OnSendNextMessage()
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

QString UstirovMediator::GetLastUstirovSocketErrorMessage() const noexcept
{
    return m_ustirovSocket->GetLastUstirovErrorMessage();
}

QList<QByteArray> UstirovMediator::GetMessageList() const noexcept
{
    return m_messagesToSendList;
}

void UstirovMediator::RestartCommandsCreate() noexcept
{
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateRestartCommand());
}

void UstirovMediator::OnGetStateFromMessage(const QByteArray &message)
{
    if (m_ustirovMessageGetter->FillDataIntoStructFromMessage(message))
    {
        OnSendNextMessage();
    }
    else
    {
        m_ustirovSocket->TryToSendLastMessageAgain();
    }
}

void UstirovMediator::OnGetGeoFromMessage(const QByteArray &message)
{
    m_ustirovMessageGetter->SaveCoordinatesToRepository(message);
}

void UstirovMediator::OnRequestTimeOut()
{
    m_logger->Appends("UM: Время ожидания истекло");
    m_ustirovMessageGetter->SetTimeOutState();
    Q_EMIT ToSendRarmNormalState(m_ustirovMessageGetter->GetMessageNormal());
}

void UstirovMediator::OnSetUstirovNormalState(const DevicesAdjustingKitMessage &state)
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
            OnSendNextMessage();
        }
        else
        {
            if (4 == state.state)
            {
                m_logger->Appends("UM: Считываем нормальные данные");
                m_isRestartMode = false;
                m_messagesToSendList.clear();
                GetNormalStateCommandsCreate();
                OnSendNextMessage();
            }
            else
            {
                m_logger->Appends("UM: Пишем нормальные данные в юстировочный");
                m_isRestartMode = false;
                m_ustirovMessageRepository->SetDistanceToLocator(state.DistanceToLocator);
                m_messagesToSendList.clear();
                SetNormalStateCommandsCreate(state);
                GetNormalStateCommandsCreate();
                OnSendNextMessage();
            }
        }
    }
    else
    {
        m_isRestartMode = false;
        SendToRarmMessageWithNoConnectionInfo();
    }

}

void UstirovMediator::OnSetUstirovBparState(const DevicesBparAdjustingKitMessage &state)
{
    m_pingTimer->stop();
    if (m_ustirovSocket->IsUstirovConnected())
    {
        switch (state.state)
        {
        case 3:
        {
            m_logger->Appends("UM: Перезагружаем устройство");
            m_messagesToSendList.clear();
            m_isRestartMode = true;
            RestartCommandsCreate();
            OnSendNextMessage();
            break;
        }
        case 4:
        {
            m_logger->Appends("UM: Считываем БПАР данные");
            m_isRestartMode = false;
            m_messagesToSendList.clear();
            m_ustirovMessageRepository->SetThreshold(state.hasThreshold, state.threshold);
            m_messagesToSendList.append(m_ustirovMessageSetter->CreateSevenCommand(5));
            OnSendNextMessage();
            break;
        }
        default:
        {
            m_logger->Appends("UM: Пишем данные БПАР в юстировочный");
            m_isRestartMode = false;
            m_ustirovMessageRepository->SetDistanceToLocator(state.DistanceToLocator);
            m_messagesToSendList.clear();
            m_ustirovMessageRepository->SetThreshold(state.hasThreshold, state.threshold);
            m_messagesToSendList.append(m_ustirovMessageSetter->CreateBparCommand(state));
            m_messagesToSendList.append(m_ustirovMessageSetter->CreateSevenCommand(6));
            OnSendNextMessage();
            break;
        }
        }
    }
    else
    {
        m_isRestartMode = false;
        SendToRarmMessageWithNoConnectionInfo();
    }
}

void UstirovMediator::OnRequestGeoState()
{
    m_pingTimer->stop();
    if (m_ustirovSocket->IsUstirovConnected())
    {
        m_logger->Appends("UM: Получаем геоданные с ЮК");
        m_messagesToSendList.clear();
        m_isRestartMode = false;
        m_messagesToSendList.append(m_ustirovMessageSetter->CreateSevenCommand(7));
        OnSendNextMessage();
    }
    else
    {
        m_logger->Appends("UM: Хотим получить с ЮК координаты");
    }
}

void UstirovMediator::SetNormalStateCommandsCreate(const DevicesAdjustingKitMessage &state)
{
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateSixCommand(0.0));
    if (m_ustirovMessageRepository->GetFvco() != state.FvcoRx)
    {
        m_messagesToSendList.append(m_ustirovMessageSetter->CreateFirstCommand(state.FvcoRx));
    }
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateSecondCommand(state.FvcoTx));
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateThirdCommand(state.Distance, state.DistanceToLocator));
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateFourthCommand(state.GAIN_TX, state.GAIN_RX));
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateFiveCommand(state.Attenuator));
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateNineCommand(state.DoplerFrequency));
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateSixCommand(state.WorkMode));
}

void UstirovMediator::GetNormalStateCommandsCreate() noexcept
{
    for (int id = 1; id < 6; ++id)
    {
        m_messagesToSendList.append(m_ustirovMessageSetter->CreateSevenCommand(id));
    }
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateSevenCommand(9));
    m_messagesToSendList.append(m_ustirovMessageSetter->CreateSevenCommand(6));
}

void UstirovMediator::SendToRarmMessageWithNoConnectionInfo() noexcept
{
    m_ustirovMessageGetter->SetNoConnectionStateNormal();
    Q_EMIT ToSendRarmNormalState(m_ustirovMessageGetter->GetMessageNormal());
}
