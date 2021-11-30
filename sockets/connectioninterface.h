#ifndef CONNECTIONINTERFACE_H
#define CONNECTIONINTERFACE_H

#include "enum/uenum.h"

class SocketInterface{
private:
    virtual void CreateObjects() = 0;
    virtual void InitObjects() = 0;
    virtual void ConnectObjects() = 0;
public:
    virtual void SendMessageToRarm(MessagesType &type, const QByteArray &message) = 0;
    virtual void NoResponce();
};

#endif // CONNECTIONINTERFACE_H
