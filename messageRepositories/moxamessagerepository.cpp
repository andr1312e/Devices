#include "moxamessagerepository.h"

MoxaMessageRepository::MoxaMessageRepository(QObject *parent)
    : QObject(parent)
    , m_senderTimer(new QTimer(this))
{
    ResetAllStates();
    m_senderTimer->setTimerType(Qt::VeryCoarseTimer);
    m_senderTimer->start(10000);
    connect(m_senderTimer, &QTimer::timeout, this, &MoxaMessageRepository::OnSendMoxaState);
}

MoxaMessageRepository::~MoxaMessageRepository()
{
    delete m_senderTimer;
}

void MoxaMessageRepository::OnSendMoxaState()
{
    Q_EMIT ToSendRarmMoxaWorksState(moxaState);
    QTimer::singleShot(1000, [ = ]()
    {
        ResetAllStates();
    } );
}

void MoxaMessageRepository::SetMoxaState(quint8 state)
{
    moxaState.state = state;
}

void MoxaMessageRepository::SetPcbState(quint8 state)
{
    moxaState.stateCK = state;
}

quint8 MoxaMessageRepository::GetMoxaState() const
{
    return moxaState.state;
}

void MoxaMessageRepository::ResetAllStates()
{
    moxaState.state = 0;
    moxaState.stateCK = 0;
}
