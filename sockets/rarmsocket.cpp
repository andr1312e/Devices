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

    qDebug() << QStringLiteral("SocketToRarm::OnConnectedToRarm Подкючились к рарму");
    QByteArray messageWidthIdWantsToGet;

    QDataStream dataStream(&messageWidthIdWantsToGet, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_5_3);
    dataStream.setByteOrder(QDataStream::LittleEndian);
    dataStream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    qint16 sizeOfMessage = qint16((m_messagesIdWantedFromRarm.count() + 2) * sizeof(quint8)); //размер списка сообщений+мой ид+ ид этого сообщения(255)
    dataStream << sizeOfMessage;//размер
    dataStream << quint8(255);//ид сообщения
    dataStream << quint8(DEVICE_MANAGER);// мой ид

    for (quint8 item : m_messagesIdWantedFromRarm)
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
                qDebug() << "получили DEVICES_ADJUSTING_KIT_SET_STATE";
                Q_EMIT ToSetUstirovState(setStateMessage);
            }
            else
            {
                qDebug() << "DEVICES_ADJUSTING_KIT_SET_STATE размеры не соотвествующие:получили " << messageSize << " и должно быть " << sizeof (DevicesAdjustingKitMessage);
                in.skipRawData(messageSize - 1);
            }
            break;
        }
        default:
        {
            qDebug() << "Получено сообщение " << messageId << "  не обрабатываем! Убери из списка или напиши обработку";
            if ((messageSize - 1) > (gettingMessageArray.size() - in.device()->pos())) {
                gettingMessageArray.clear();
                qDebug() << QStringLiteral("Отчищаем буфер сообщений");
                return;
            }
            else
            {
                in.skipRawData(messageSize - 1);
                qDebug() << "Пропустили " << messageSize << " байт";
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
        qDebug() << "Пробуем получить соединение снова..." << socketError;
        ReconnectToRarm();
    } else
    {
        qDebug() << "Ошибка: " << socketError;
    }
}

void RarmSocket::OnDisconnected()
{
    ChangeReadyReadConnection(false);
    qDebug() << QStringLiteral("Отсоединились от РАРМ...");
}

void RarmSocket::OnCheckConnection()
{
    qDebug() << "Время проверить поключение";

    if (!IsRarmConnected())
    {
        ReconnectToRarm();
        qDebug() << "Переподключаемся";
    }
}

void RarmSocket::OnSendMoxaWorksState(DevicesMoxaStateMessage &moxaState)
{
        qDebug()<< "RarmSocket::OnSendMoxaWorksState " << moxaState.state;
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
    qDebug()<< "RarmSocket::OnSendRarmMeteoState ";
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
    qDebug()<< "RarmSocket::OnSendRarmUPCBState ";
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
//    out << currentTimeVal.usecs;
//    out << currentTimeVal.secs;
//    out << upcbState.Fvco;
//    out << upcbState.DoplerFrequency;
//    out << upcbState.Distance;
//    out << upcbState.GAIN_TX;
//    out << upcbState.GAIN_RX;//сначала TX потом RX
//    out << upcbState.Attenuator;
//    out << upcbState.WorkMode;
//    out << upcbState.PhaseIncrement;
    SendRarmMessage(ustirovMessage);
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





