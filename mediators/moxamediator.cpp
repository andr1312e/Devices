#include "moxamediator.h"

MoxaMediator::MoxaMediator(const QString &settingsFileName, QObject *parent)
    : QObject(parent)
    , m_messageRepository(new MoxaMessageRepository(this))
{
    ReadDataFromSettingsFile(settingsFileName);
    InitObjects();
    ConnectObjects();
}

MoxaMediator::~MoxaMediator()
{
    m_process.kill();
    delete m_messageRepository;
}

void MoxaMediator::ReadDataFromSettingsFile(const QString &settingsFileName)
{
    QSettings mediatorSettings(settingsFileName, QSettings::IniFormat, this);
    if (mediatorSettings.contains(QStringLiteral("moxaIpAdress")))
    {
        m_moxaIpAdress = mediatorSettings.value(QStringLiteral("moxaIpAdress"), "192.168.111.254").toString();
    }
    else
    {
        m_moxaIpAdress = QStringLiteral("192.168.111.254");
        mediatorSettings.setValue(QStringLiteral("moxaIpAdress"), m_moxaIpAdress);
    }

    if (mediatorSettings.contains(QStringLiteral("moxaCheckMiliseconds")))
    {
        m_moxaCheckMiliseconds = mediatorSettings.value(QStringLiteral("moxaCheckMiliseconds"), 10000).toUInt();
    }
    else
    {
        m_moxaCheckMiliseconds = 10000;
        mediatorSettings.setValue(QStringLiteral("moxaCheckMiliseconds"), m_moxaCheckMiliseconds);
    }
    mediatorSettings.sync();
}

void MoxaMediator::InitObjects()
{
#if defined(WIN32)
    m_processParametrs << QStringLiteral("-n") << QStringLiteral("1");
#else
    m_processParametrs << QStringLiteral("-c 1");
#endif
    m_processParametrs.append(m_moxaIpAdress);
    startTimer(m_moxaCheckMiliseconds, Qt::VeryCoarseTimer);
}

void MoxaMediator::ConnectObjects()
{
    connect(m_messageRepository, &MoxaMessageRepository::ToSendRarmMoxaWorksState, this, &MoxaMediator::ToSendRarmMoxaWorksState);
}

void MoxaMediator::OnRegisterPCBState()
{
    m_messageRepository->SetPcbState(1);
}

const QString &MoxaMediator::GetMoxaIpAdress() const
{
    return m_moxaIpAdress;
}

bool MoxaMediator::IsMoxaConnected() const
{
    return 1 == m_messageRepository->GetMoxaState();
}

void MoxaMediator::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
    const int result = m_process.execute(QStringLiteral("ping"), m_processParametrs);
    if (0 == result)
    {
        m_messageRepository->SetMoxaState(1);
    }
    else
    {
        m_messageRepository->SetMoxaState(0);
    }
}
