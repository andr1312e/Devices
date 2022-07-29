#include "rarmsocket.h"
#include <chrono>
#include <QDateTime>

RarmSocket::RarmSocket(const Logger *logger, const QString &rarmAdress, quint16 port, QObject *parent)
    : QObject(parent)
    , m_rarmAdress(rarmAdress)
    , m_port(port)
    , m_logger(logger)
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
    m_timeToReConnectToRarm->start();
    m_messagesIdWantedFromRarm =
    {
        DEVICES_ADJUSTING_KIT_SET_STATE,//id 160 Отправка состояний в юстировочный комлект
        DEVICES_ADJUSTING_KIT_BRAR_SET_STATE,//id 165 Сообщение с установкой режима БПАР (юстировочный комплект)
        DEVICES_GEO_STATE_REQUEST_MESSAGE, //id 167 Сообщение с запросом координт от геомодуля ZED-F9P
    };
    m_pTcpSocketToRarm->connectToHost(m_rarmAdress, m_port, QIODevice::ReadWrite);
}

void RarmSocket::ConnectObjects()
{
    connect(m_pTcpSocketToRarm, &QTcpSocket::connected, this, &RarmSocket::OnConnectedToRarm);
    connect(m_pTcpSocketToRarm, &QTcpSocket::disconnected, this, &RarmSocket::OnDisconnected);
#ifndef QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        connect(m_pTcpSocketToRarm,  QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error), this, &RarmSocket::OnConnectionError);
#endif
    connect(m_timeToReConnectToRarm, &QTimer::timeout, this, &RarmSocket::OnCheckConnection);
}

void RarmSocket::OnConnectedToRarm()
{
    gettingMessageArray.clear();

    m_logger->Appends("RS: Подключен к рарму");
    QByteArray messageWidthIdWantsToGet;

    QDataStream dataStream(&messageWidthIdWantsToGet, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_5_11);
    dataStream.setByteOrder(QDataStream::LittleEndian);
    dataStream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    const qint16 sizeOfMessage = qint16((m_messagesIdWantedFromRarm.count() + 2) * sizeof(quint8)); //размер списка сообщений+мой ид+ ид этого сообщения(255)
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
    in.setVersion(QDataStream::Qt_5_11);
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

    while ((messageSize - 1) <= (gettingMessageArray.count() - in.device()->pos()))
    {
        headerHandling = true;
        lastPos = in.device()->pos();

        switch (messageId)
        {
        case DEVICES_ADJUSTING_KIT_SET_STATE:
        {
            //160 Отправка данных в модуль обмена с периферийными устройствами(юстировочный комплект
            if (sizeof (DevicesAdjustingKitMessage) == (messageSize - 1))
            {
                DevicesAdjustingKitMessage setNormalStateMessage;
                in.readRawData((char *) &setNormalStateMessage, messageSize - 1);
                m_logger->Appends("RS: Получили DEVICES_ADJUSTING_KIT_SET_STATE");
                Q_EMIT ToSetUstirovNormalState(setNormalStateMessage);
                gettingMessageArray.clear();
            }
            else
            {
                m_logger->Appends("RS: DEVICES_ADJUSTING_KIT_SET_STATE неверный размер");
                in.skipRawData(messageSize - 1);
            }
            break;
        }
        case DEVICES_ADJUSTING_KIT_BRAR_SET_STATE:
        {
            //165 Сообщение с установкой режима БПАР (юстировочный комплект)
            if (sizeof (DevicesBparAdjustingKitMessage) == (messageSize - 1))
            {
                DevicesBparAdjustingKitMessage setBparMessage;
                in.readRawData((char *) &setBparMessage, messageSize - 1);
                m_logger->Appends("RS: Получили DEVICES_ADJUSTING_KIT_BRAR_SET_STATE");
                Q_EMIT ToSetUstirovBparState(setBparMessage);
                gettingMessageArray.clear();
            }
            else
            {
                m_logger->Appends("RS: DEVICES_ADJUSTING_KIT_BRAR_SET_STATE неверный размер");
                in.skipRawData(messageSize - 1);
            }
            break;
        }
        case DEVICES_GEO_STATE_REQUEST_MESSAGE:
        {
            //id 167 Сообщение с запросом координт от геомодуля ZED-F9P
            if (sizeof (DevicesGeoStateMessage) == (messageSize - 1))
            {
                m_logger->Appends("RS: Получили DEVICES_GEO_STATE_REQUEST_MESSAGE");
                gettingMessageArray.clear();
                Q_EMIT ToRequestGeoState();
            }
            else
            {
                m_logger->Appends("RS: DEVICES_GEO_STATE_REQUEST_MESSAGE неверный размер");
                in.skipRawData(messageSize - 1);
            }
            break;
        }
        default:
        {
            m_logger->Appends("RS: Ид не обрабатываем " + std::to_string(messageId));
            if ((messageSize - 1) > (gettingMessageArray.size() - in.device()->pos()))
            {
                gettingMessageArray.clear();
                m_logger->Appends("RS: Чистим буфер");
                return;
            }
            else
            {
                in.skipRawData(messageSize - 1);
                m_logger->Appends("RS: Пропускаем " + std::to_string(messageSize) + " байт");
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

void RarmSocket::OnConnectionError()
{
    m_logger->Appends("RS: Ошибка сокета " + m_pTcpSocketToRarm->errorString().toStdString());
}

void RarmSocket::OnDisconnected()
{
    ChangeReadyReadConnection(false);
    m_logger->Appends("RS: Отключились от рарм");
}

void RarmSocket::OnCheckConnection()
{
    if (!IsRarmConnected())
    {
        m_logger->Appends("RS: Переподключаемся");
        ReconnectToRarm();
    }
}

void RarmSocket::OnSendRarmMoxaWorksState(DevicesMoxaStateMessage &moxaState)
{
    m_logger->Appends("RS: Отправляем состояние мохи. Cвязь:" + std::to_string(moxaState.state));
    QByteArray moxaMessage;
    QDateTime currentDateTime(QDateTime::currentDateTime());
    moxaState.sTimeMeasurement.usecs = currentDateTime.toMSecsSinceEpoch();
    moxaState.sTimeMeasurement.secs = currentDateTime.toSecsSinceEpoch();

    QDataStream out(&moxaMessage, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_11);
    out.setByteOrder(QDataStream::LittleEndian);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);

    out << qint16(sizeof (DevicesMoxaStateMessage) + 1);
    out << quint8(DEVICES_MOXA_STATE_MESSAGE);//id сообщения которое я отправляю
    out.writeRawData(reinterpret_cast<const char *>(&moxaState), sizeof(DevicesMoxaStateMessage));
    SendRarmMessage(moxaMessage);

}

void RarmSocket::OnSendRarmMeteoState(const DevicesMeteoKitGetMessage &meteoState)
{
    m_logger->Appends("RS: Отправляем метео. Состояние: " + std::to_string(meteoState.state));
    QByteArray meteoMessage;


    QDataStream out(&meteoMessage, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_11);
    out.setByteOrder(QDataStream::LittleEndian);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);

    out << qint16(sizeof (DevicesMeteoKitGetMessage) + 1);
    out << quint8(DEVICES_METEO_KIT_GET_MESSAGE);//id сообщения которое я отправляю
    out.writeRawData(reinterpret_cast<const char *>(&meteoState), sizeof(DevicesMeteoKitGetMessage));
    SendRarmMessage(meteoMessage);
}

void RarmSocket::OnSendRarmNormalState(const DevicesAdjustingKitMessage &upcbState)
{
    m_logger->Appends("RS: Отправляем юстировочное обычное. Состояние: " + std::to_string(upcbState.state));
    QByteArray ustirovMessage;

    QDataStream out(&ustirovMessage, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_11);
    out.setByteOrder(QDataStream::LittleEndian);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);

    out << qint16(sizeof (DevicesAdjustingKitMessage) + 1);
    out << quint8(DEVICES_ADJUSTING_KIT_GET_STATE);//id сообщения которое я отправляю
    out.writeRawData(reinterpret_cast<const char *>(&upcbState), sizeof(DevicesAdjustingKitMessage));
    SendRarmMessage(ustirovMessage);
}

void RarmSocket::OnSendRarmBparState(const DevicesBparAdjustingKitMessage &bparState)
{
    m_logger->Appends("RS: Отправляем юстировочное БПАР. Состояние: " + std::to_string(bparState.state));
    QByteArray ustirovMessage;

    QDataStream out(&ustirovMessage, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_11);
    out.setByteOrder(QDataStream::LittleEndian);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);

    out << qint16(sizeof (DevicesBparAdjustingKitMessage) + 1);
    out << quint8(DEVICES_ADJUSTING_KIT_BRAR_GET_STATE);//id сообщения которое я отправляю
    out.writeRawData(reinterpret_cast<const char *>(&bparState), sizeof(DevicesBparAdjustingKitMessage));
    SendRarmMessage(ustirovMessage);
}

void RarmSocket::OnSendRarmGeoState(const DevicesGeoStateMessage &state)
{
    m_logger->Appends("RS: Отправляем гео");
    QByteArray geoMessage;

    QDataStream out(&geoMessage, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_11);
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

void RarmSocket::ChangeReadyReadConnection(bool state) noexcept
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

std::string RarmSocket::GetRarmError() const noexcept
{
    return m_pTcpSocketToRarm->errorString().toStdString();
}







