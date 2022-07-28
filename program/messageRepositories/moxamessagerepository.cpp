#include "moxamessagerepository.h"

MoxaMessageRepository::MoxaMessageRepository(QObject *parent)
    : QObject(parent)
    , m_senderTimer(new QTimer(this))
{
    ResetRepository();
    m_senderTimer->setTimerType(Qt::VeryCoarseTimer);
    connect(m_senderTimer, &QTimer::timeout, this, &MoxaMessageRepository::OnSendMoxaState);
    m_senderTimer->start(std::chrono::seconds(10));

}

MoxaMessageRepository::~MoxaMessageRepository()
{
    delete m_senderTimer;
}

void MoxaMessageRepository::OnSendMoxaState()
{
    Q_EMIT ToSendRarmMoxaWorksState(moxaState);
    QTimer::singleShot(std::chrono::seconds(1), [ = ]()
    {
        ResetRepository();
    } );
}

void MoxaMessageRepository::SetMoxaState(quint8 state) noexcept
{
    moxaState.state = state;
}

void MoxaMessageRepository::SetPcbState(quint8 state) noexcept
{
    moxaState.stateCK = state;
}

quint8 MoxaMessageRepository::GetMoxaState() const noexcept
{
    return moxaState.state;
}

void MoxaMessageRepository::ResetRepository() noexcept
{
    moxaState.state = 0;
    moxaState.stateCK = 0;
}
