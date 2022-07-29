#include "moxamediator.h"

MoxaMediator::MoxaMediator(QObject *parent)
    : QObject(parent)
    , m_messageRepository(new MoxaMessageRepository(this))
{
    ReadParamsFromProfile();
    InitObjects();
    ConnectObjects();
}

MoxaMediator::~MoxaMediator()
{
    m_process.kill();
    delete m_messageRepository;
}

void MoxaMediator::ReadParamsFromProfile()
{
    const QString moxaIpAdress = ProFile::GetProFileField(QLatin1Literal("moxaIpAdress"));
    if (moxaIpAdress.isEmpty())
    {
        m_moxaIpAdress = QLatin1Literal("10.130.246.121");
        ProFile::SetProfileField(QLatin1Literal("moxaIpAdress"), m_moxaIpAdress);
    }
    else
    {
        m_moxaIpAdress = moxaIpAdress;
    }
    const QString moxaCheckMiliseconds = ProFile::GetProFileField(QLatin1Literal("moxaCheckMiliseconds"));
    if (moxaCheckMiliseconds.isEmpty())
    {
        m_moxaCheckMiliseconds = 10000;
        ProFile::SetProfileField(QLatin1Literal("moxaCheckMiliseconds"), QString::number(m_moxaCheckMiliseconds));
    }
    else
    {
        bool isNum = false;
        m_moxaCheckMiliseconds = moxaCheckMiliseconds.toUInt(&isNum);
        if (!isNum)
        {
            m_moxaCheckMiliseconds = 10000;
            ProFile::SetProfileField(QLatin1Literal("moxaCheckMiliseconds"), QString::number(m_moxaCheckMiliseconds));
        }
    }
}

void MoxaMediator::InitObjects()
{
#ifdef Q_OS_WINDOWS
    m_processParametrs << QLatin1Literal("-n") << QLatin1Literal("1");
#else
    m_processParametrs << QLatin1Literal("-c 1");
#endif
    m_processParametrs.append(m_moxaIpAdress);
    startTimer(m_moxaCheckMiliseconds, Qt::VeryCoarseTimer);
}

void MoxaMediator::ConnectObjects()
{
    connect(m_messageRepository, &MoxaMessageRepository::ToSendRarmMoxaWorksState, this, &MoxaMediator::ToSendRarmMoxaWorksState);
}

void MoxaMediator::OnSendPcbWork()
{
    m_messageRepository->SetPcbState(1);
}

const QString &MoxaMediator::GetMoxaIpAdress() const noexcept
{
    return m_moxaIpAdress;
}

bool MoxaMediator::IsMoxaConnected() const noexcept
{
    return 1 == m_messageRepository->GetMoxaState();
}

void MoxaMediator::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
    const int result = m_process.execute(QLatin1Literal("ping"), m_processParametrs);
    if (0 == result)
    {
        m_messageRepository->SetMoxaState(1);
    }
    else
    {
        m_messageRepository->SetMoxaState(0);
    }
}
