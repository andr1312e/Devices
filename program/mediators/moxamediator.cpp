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
    const QString moxaIpAdress = ProFile::GetProFileField(QLatin1String("moxaIpAdress"));
    if (moxaIpAdress.isEmpty())
    {
        m_moxaIpAdress = QLatin1String("10.130.246.121");
        ProFile::SetProfileField(QLatin1String("moxaIpAdress"), m_moxaIpAdress);
    }
    else
    {
        m_moxaIpAdress = moxaIpAdress;
    }
    const QString moxaCheckMiliseconds = ProFile::GetProFileField(QLatin1String("moxaCheckMiliseconds"));
    if (moxaCheckMiliseconds.isEmpty())
    {
        m_moxaCheckMiliseconds = 10000;
        ProFile::SetProfileField(QLatin1String("moxaCheckMiliseconds"), QString::number(m_moxaCheckMiliseconds));
    }
    else
    {
        bool isNum = false;
        m_moxaCheckMiliseconds = moxaCheckMiliseconds.toUInt(&isNum);
        if (!isNum)
        {
            m_moxaCheckMiliseconds = 10000;
            ProFile::SetProfileField(QLatin1String("moxaCheckMiliseconds"), QString::number(m_moxaCheckMiliseconds));
        }
    }
}

void MoxaMediator::InitObjects()
{
#ifdef Q_OS_WINDOWS
    m_processParametrs << QLatin1String("-n") << QLatin1String("1");
#else
    m_processParametrs << QLatin1String("-c 1");
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
    const int result = m_process.execute(QLatin1String("ping"), m_processParametrs);
    if (0 == result)
    {
        m_messageRepository->SetMoxaState(1);
    }
    else
    {
        m_messageRepository->SetMoxaState(0);
    }
}
