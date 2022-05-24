#ifndef MOXAMESSAGEREPOSITORY_H
#define MOXAMESSAGEREPOSITORY_H
#include <QObject>
#include <QTimer>
#include "datamessage.h"

class MoxaMessageRepository : public QObject
{
    Q_OBJECT
public:
    MoxaMessageRepository(QObject *parent);
    ~MoxaMessageRepository();
Q_SIGNALS:
    void ToSendRarmMoxaWorksState(DevicesMoxaStateMessage &moxaState);
private Q_SLOTS:
    void OnSendMoxaState();
public:
    void SetMoxaState(quint8 state);
    void SetPcbState(quint8 state);
    quint8 GetMoxaState() const;
private:
    void ResetAllStates();
private:
    DevicesMoxaStateMessage moxaState;
    QTimer *const m_senderTimer;
};

#endif // MOXAMESSAGEREPOSITORY_H
