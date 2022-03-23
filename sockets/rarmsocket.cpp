#include "sockets/rarmsocket.h"
#include <chrono>
#include <QDateTime>

RarmSocket::RarmSocket(const QString &rarmAdress, quint16 port, QObject *parent)
    : QObject(parent)
    , m_rarmAdress(rarmAdress)
    , m_port(port)
{
    CreateObjects();
    InitObjects();
    ConnectObjects();
}

RarmSocket::~RarmSocket()
{
    delete m_timeToReConnectToRarm;
    if (IsRarmConnected())
    {
        StopRarmConnect();
    }
    delete m_pTcpSocketToRarm;
}

void RarmSocket::CreateObjects()
{
    m_pTcpSocketToRarm = new QTcpSocket(this);
    m_timeToReConnectToRarm = new QTimer(this);
}

void RarmSocket::InitObjects()
{
    m_timeToReConnectToRarm->setInterval(std::chrono::seconds(2));
    m_timeToReConnectToRarm->setTimerType(Qt::VeryCoarseTimer);
    m_messagesIdWantedFromRarm =  {
        DEVICES_ADJUSTING_KIT_SET_STATE//id 151 Отправка состояний в юстировочный комлект
    };
    m_pTcpSocketToRarm->connectToHost(m_rarmAdress, m_port, QIODevice::ReadWrite);
}

void RarmSocket::ConnectObjects()
{
    connect(m_pTcpSocketToRarm, &QTcpSocket::connected, this, &RarmSocket::OnConnectedToRarm);
    connect(m_pTcpSocketToRarm, &QTcpSocket::disconnected, this, &RarmSocket::OnDisconnected);
    connect(m_timeToReConnectToRarm, &QTimer::timeout, this, &RarmSocket::OnCheckConnection);
}

void RarmSocket::OnConnectedToRarm()
{
    gettingMessageArray.clear();

    qDebug() << QStringLiteral("RS: ConnectedToRarm");
    QByteArray messageWidthIdWantsToGet;

    QDataStream dataStream(&messageWidthIdWantsToGet, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_5_3);
    dataStream.setByteOrder(QDataStream::LittleEndian);
    dataStream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    qint16 sizeOfMessage = qint16((m_messagesIdWantedFromRarm.count() + 2) * sizeof(quint8)); //размер списка сообщений+мой ид+ ид этого сообщения(255)
    dataStream << sizeOfMessage;//размер
    dataStream << quint8(255);//ид сообщения
    dataStream << quint8(DEVICE_MANAGER);// мой ид

    for (const quint8 item : qAsConst(m_messagesIdWantedFromRarm))
    {
        dataStream << item;
    }

    m_pTcpSocketToRarm->write(messageWidthIdWantsToGet);
    m_pTcpSocketToRarm->flush();
    ChangeReadyReadConnection(true);
}

void RarmSocket::OnReadyRead()
{
    gettingMessageArray.append(m_pTcpSocketToRarm->readAll());

    QDataStream in(&gettingMessageArray, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);
    in.setByteOrder(QDataStream::LittleEndian);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);

    static quint8 lastID;
    static quint16 lastSize;
    static quint32 lastPos;
    quint8 messageId;
    quint16 messageSize;
    bool headerHandling = false;
    in >> messageSize;
    in >> messageId;

    while ((messageSize - 1) <= (gettingMessageArray.size() - in.device()->pos()))
    {
        headerHandling = true;
        lastPos = in.device()->pos();

        switch (messageId)
        {
        case DEVICES_ADJUSTING_KIT_SET_STATE:
        { //160 Отправка данных в модуль обмена с периферийными устройствами(юстировочный комплект
            if (sizeof (DevicesAdjustingKitMessage) == (messageSize - 1))
            {
                DevicesAdjustingKitMessage setStateMessage;
                in.readRawData((char *) &setStateMessage, messageSize - 1);
                qDebug() << "RS: Get DEVICES_ADJUSTING_KIT_SET_STATE";
                Q_EMIT ToSetUstirovState(setStateMessage);
                gettingMessageArray.clear();
            }
            else
            {
                qDebug() << "RS: DEVICES_ADJUSTING_KIT_SET_STATE wrong size" << messageSize << " should be  " << sizeof (DevicesAdjustingKitMessage);
                in.skipRawData(messageSize - 1);
            }
            break;
        }
        default:
        {
            qDebug() << "RS: no id " << messageId;
            if ((messageSize - 1) > (gettingMessageArray.size() - in.device()->pos())) {
                gettingMessageArray.clear();
                qDebug() << QStringLiteral("RS: Clear buffer");
                return;
            }
            else
            {
                in.skipRawData(messageSize - 1);
                qDebug() << "RS:  skip " << messageSize << " bytes";
            }
            break;
        }
        }

        if ((gettingMessageArray.size() - in.device()->pos()) > 3)
        {
            lastPos = in.device()->pos();
            lastID = messageId;
            lastSize = messageSize;
            in >> messageSize;
            in >> messageId;
            headerHandling = false;
        }
    }

    if (!headerHandling)
    {
        in.device()->seek(in.device()->pos() - 3);
    }

    gettingMessageArray.remove(0, in.device()->pos());
}

void RarmSocket::OnConnectionError(QAbstractSocket::SocketError socketError)
{
    if (socketError ==  QAbstractSocket::ConnectionRefusedError ||
            socketError ==  QAbstractSocket::HostNotFoundError ||
            socketError ==  QAbstractSocket::SocketTimeoutError ||
            socketError ==  QAbstractSocket::NetworkError)
    {
        qDebug() << "RS - Try to reconnect..." << socketError;
        ReconnectToRarm();
    } else
    {
        qDebug() << "RS - Error " << socketError;
    }
}

void RarmSocket::OnDisconnected()
{
    ChangeReadyReadConnection(false);
    qDebug() << QStringLiteral("RS - Disconnect from rarm");
}

void RarmSocket::OnCheckConnection()
{


    if (!IsRarmConnected())
    {
         qDebug() << "RS - Try to reconnect...";
        ReconnectToRarm();
    }
}

void RarmSocket::OnSendRarmMoxaWorksState(DevicesMoxaStateMessage &moxaState)
{
        qDebug()<< "RS: SendMoxaState " << moxaState.state;
    QByteArray moxaMessage;
    QDateTime currentDateTime(QDateTime::currentDateTime());
    moxaState.sTimeMeasurement.usecs=currentDateTime.toMSecsSinceEpoch();
    moxaState.sTimeMeasurement.secs=currentDateTime.toSecsSinceEpoch();

    QDataStream out(&moxaMessage, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out.setByteOrder(QDataStream::LittleEndian);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);

    out << qint16(sizeof (DevicesMoxaStateMessage) + 1);
    out << quint8(DEVICES_MOXA_STATE_MESSAGE);//id сообщения которое я отправляю
    out.writeRawData(reinterpret_cast<const char *>(&moxaState), sizeof(DevicesMoxaStateMessage));
    SendRarmMessage(moxaMessage);

}

void RarmSocket::OnSendRarmMeteoState(DevicesMeteoKitGetMessage &meteoState)
{
    qDebug()<< "RS: SendMeteoState ";
    QByteArray meteoMessage;

    QDateTime currentDateTime(QDateTime::currentDateTime());
    meteoState.sTimeMeasurement.usecs=currentDateTime.toMSecsSinceEpoch();
    meteoState.sTimeMeasurement.secs=currentDateTime.toSecsSinceEpoch();

    QDataStream out(&meteoMessage, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out.setByteOrder(QDataStream::LittleEndian);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);

    out << qint16(sizeof (DevicesMeteoKitGetMessage) + 1);
    out << quint8(DEVICES_METEO_KIT_GET_MESSAGE);//id сообщения которое я отправляю
    out.writeRawData(reinterpret_cast<const char *>(&meteoState), sizeof(DevicesMeteoKitGetMessage));
    SendRarmMessage(meteoMessage);
}

void RarmSocket::OnSendRarmUPCBState(DevicesAdjustingKitMessage &upcbState)
{
    qDebug()<< "RS: SendUkitState ";
    QByteArray ustirovMessage;

    QDateTime currentDateTime(QDateTime::currentDateTime());
    upcbState.sTimeMeasurement.usecs=currentDateTime.toMSecsSinceEpoch();
    upcbState.sTimeMeasurement.secs=currentDateTime.toSecsSinceEpoch();

    QDataStream out(&ustirovMessage, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out.setByteOrder(QDataStream::LittleEndian);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);

    out << qint16(sizeof (DevicesAdjustingKitMessage) + 1);
    out << quint8(DEVICES_ADJUSTING_KIT_GET_STATE);//id сообщения которое я отправляю
    out.writeRawData(reinterpret_cast<const char *>(&upcbState), sizeof(DevicesAdjustingKitMessage));
    SendRarmMessage(ustirovMessage);
}

void RarmSocket::OnSendRarmGeoMessage(DevicesGeoStateMessage &state)
{
    qDebug()<< "RS: Send Geo Message ";
    QByteArray geoMessage;

    QDateTime currentDateTime(QDateTime::currentDateTime());
    state.sTimeMeasurement.usecs=currentDateTime.toMSecsSinceEpoch();
    state.sTimeMeasurement.secs=currentDateTime.toSecsSinceEpoch();

    QDataStream out(&geoMessage, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out.setByteOrder(QDataStream::LittleEndian);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);

    out << qint16(sizeof (DevicesGeoStateMessage) + 1);
    out << quint8(DEVICES_GEO_STATE_MESSAGE);//id сообщения которое я отправляю
    out.writeRawData(reinterpret_cast<const char *>(&state), sizeof(DevicesGeoStateMessage));
    SendRarmMessage(geoMessage);
}

void RarmSocket::SendRarmMessage(const QByteArray &message)
{
    m_pTcpSocketToRarm->write(message);
    m_pTcpSocketToRarm->flush();
}

void RarmSocket::ChangeReadyReadConnection(bool state)
{
    if (state)
    {
        connect(m_pTcpSocketToRarm, &QTcpSocket::readyRead, this, &RarmSocket::OnReadyRead);
    }
    else
    {
        disconnect(m_pTcpSocketToRarm, &QTcpSocket::readyRead, this, &RarmSocket::OnReadyRead);
    }
}

void RarmSocket::StopRarmConnect()
{
    m_pTcpSocketToRarm->abort();
}

void RarmSocket::ReconnectToRarm()
{
    m_pTcpSocketToRarm->connectToHost(m_rarmAdress, m_port, QIODevice::ReadWrite);
}

bool RarmSocket::IsRarmConnected()
{
    return (m_pTcpSocketToRarm->state() == QTcpSocket::ConnectedState);
}





