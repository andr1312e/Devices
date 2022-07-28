#ifndef MESSAGEREPOSITORIES_MOXAMESSAGEREPOSITORY_H
#define MESSAGEREPOSITORIES_MOXAMESSAGEREPOSITORY_H

#include <chrono>

#include <QObject>
#include <QTimer>
#include "../datamessage.h"

class MoxaMessageRepository : public QObject
{
    Q_OBJECT
public:
    explicit MoxaMessageRepository(QObject *parent);
    ~MoxaMessageRepository();
Q_SIGNALS:
    void ToSendRarmMoxaWorksState(DevicesMoxaStateMessage &moxaState);
private Q_SLOTS:
    void OnSendMoxaState();
public:
    void SetMoxaState(quint8 state) noexcept;
    void SetPcbState(quint8 state) noexcept;
    quint8 GetMoxaState() const noexcept;
private:
    void ResetRepository() noexcept;
private:
    DevicesMoxaStateMessage moxaState;
    QTimer *const m_senderTimer;
};

#endif // MESSAGEREPOSITORIES_MOXAMESSAGEREPOSITORY_H
