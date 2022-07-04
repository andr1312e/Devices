#include "moxamediator.h"

MoxaMediator::MoxaMediator(QObject *parent)
    : QObject(parent)
    , m_messageRepository(new MoxaMessageRepository(this))
{
    ReadDataFromEnviroment();
    InitObjects();
    ConnectObjects();
}

MoxaMediator::~MoxaMediator()
{
    m_process.kill();
    delete m_messageRepository;
}

void MoxaMediator::ReadDataFromEnviroment()
{
    const QString moxaIpAdress=ProFile::GetProFileField(QLatin1Literal("moxaIpAdress"));
    if(moxaIpAdress.isEmpty())
    {
        m_moxaIpAdress=QLatin1Literal("192.168.111.254");
        ProFile::SetProfileField(QLatin1Literal("moxaIpAdress"), m_moxaIpAdress);
    }
    else
    {
        m_moxaIpAdress=moxaIpAdress;
    }
    const QString moxaCheckMiliseconds=ProFile::GetProFileField(QLatin1Literal("moxaCheckMiliseconds"));
    if(moxaCheckMiliseconds.isEmpty())
    {
        m_moxaCheckMiliseconds=10000;
        ProFile::SetProfileField(QLatin1Literal("moxaCheckMiliseconds"), QString::number(m_moxaCheckMiliseconds));
    }
    else
    {
        bool isNum=false;
        m_moxaCheckMiliseconds=moxaCheckMiliseconds.toUInt(&isNum);
        if(!isNum)
        {
            m_moxaCheckMiliseconds=10000;
            ProFile::SetProfileField(QLatin1Literal("moxaCheckMiliseconds"), QString::number(m_moxaCheckMiliseconds));
        }
    }
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

bool MoxaMediator::IsIpCorrect(const QString &ip) const noexcept
{
    return true;
//    const int firstPointIndex=ip.indexOf('.');
//    const int secondPointIndex=ip.indexOf('.', firstPointIndex+1);
//    const int thirdPointIndex=ip.indexOf('.', secondPointIndex+1);
//    const int fourthPointIndex=ip.indexOf('.', thirdPointIndex+1);
//    if( firstPointIndex>0 && secondPointIndex>0 && thirdPointIndex>0 && fourthPointIndex>0)
//    {
//        if(firstPointIndex!=secondPointIndex && secondPointIndex!= thirdPointIndex && firstPointIndex!= fourthPointIndex)
//        {
//            const QString firstSubString=ip.left(firstPointIndex);
//            const QString secondSubString=ip.mid(firstPointIndex, secondPointIndex-firstPointIndex);
//            const QString thirdSubString=ip.mid(secondPointIndex, thirdPointIndex-secondPointIndex);
//            const QString fourthSubString=ip.mid(fourthPointIndex);
//            bool isNum1, isNum2, isNum3, isNum4;
//            const bool isFirstCorrect=firstSubString.toUInt(isNum1) < 256;
//            const bool isSecondCorrect=secondSubString.toUInt(isNum2) < 256;
//            const bool isThirdCorrect=thirdSubString.toUInt(isNum3) < 256;
//            const bool isFourthCorrect=fourthSubString.toUInt(isNum4) < 256;
//            return isNum1 && isNum2 && isNum3 && isNum4 && isFirstCorrect && isSecondCorrect && isThirdCorrect && isFourthCorrect;
//        }
//        else
//        {
//            return false;
//        }
//    }
//    else
//    {
//        return false;
//    }
}
