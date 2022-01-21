#include "meteosocket.h"


MeteoServer::MeteoServer(const quint16 moxaPort, const quint16 meteoKitPort, QObject *parent)
    : QObject(parent)
    , m_socketMeteo(Q_NULLPTR)
    , m_moxaPort(moxaPort)
    , m_meteoKitPort(meteoKitPort)
{
    CreateObjects();
    InitObjects();
    ConnectObjects();
}

MeteoServer::~MeteoServer()
{
    OnDisconnectedFromHost();
    delete m_noAnswerTimer;
    delete m_serverMeteo;
}

void MeteoServer::CreateObjects()
{
    m_serverMeteo = new QTcpServer(this);
    m_noAnswerTimer = new QTimer(this);
}

void MeteoServer::InitObjects()
{
    m_serverMeteo->listen(QHostAddress::Any, m_moxaPort);
    m_noAnswerTimer->setInterval(std::chrono::seconds(2));
    m_noAnswerTimer->setTimerType(Qt::VeryCoarseTimer);
    m_noAnswerTimer->setSingleShot(true);
}

void MeteoServer::ConnectObjects()
{
    connect(m_serverMeteo, &QTcpServer::newConnection, this, &MeteoServer::OnNewSocketConnected);
    connect(m_noAnswerTimer, &QTimer::timeout, this, &MeteoServer::ToRequestTimeOut);
    connect(m_serverMeteo, &QTcpServer::acceptError, this, &MeteoServer::OnErrorOccurred);
}

void MeteoServer::OnNewSocketConnected()
{

    QTcpSocket *socket = m_serverMeteo->nextPendingConnection();
    qDebug()<< "MKS - new connection";
    if(socket->peerPort() == m_meteoKitPort)
    {
        qDebug()<< "MKS - connected ";
        m_socketMeteo = socket;
        ConnectMeteoSocketObject();
    }
    else
    {
        qDebug() << "MKS - wanted port: "<< m_meteoKitPort << " our: " << socket->peerPort();
    }
}

void MeteoServer::OnReadyRead()
{
    StopNoAnswerTimer();
    qDebug()<< "MKS: GetMessage  ";
    m_readyReadBuffer.append(m_socketMeteo->readAll());
    if(m_readyReadBuffer.length() >= m_returnedMessageSize) // длина ответа 11 байт
    {
        QByteArray message = m_readyReadBuffer.mid(0, m_returnedMessageSize);
        m_readyReadBuffer.remove(0, m_returnedMessageSize);

        Q_EMIT ToGetStateFromMessage(message);
    }
}

void MeteoServer::OnDisconnectedFromHost()
{
    Q_EMIT ToResetQueue();
    m_socketMeteo->disconnectFromHost();
    qDebug()<< "MKS - disconnect...";
}

void MeteoServer::OnErrorOccurred(QAbstractSocket::SocketError socketError)
{
    qDebug()<< "MKS::OnErrorOccurred!!!!!";
    m_socketMeteo->disconnectFromHost();
    switch (socketError) {
    case QAbstractSocket::ConnectionRefusedError:
        qDebug()<< QStringLiteral("MKS - Истекло время ожидания").toUtf8();
        break;
    case QAbstractSocket::RemoteHostClosedError:
        qDebug()<< QStringLiteral("MKS - Удаленный хост закрыл соединение").toUtf8();
        break;
    case QAbstractSocket::SocketAccessError:
        qDebug()<< QStringLiteral("MKS - Адрес хоста не найден").toUtf8();
        break;
    case QAbstractSocket::SocketResourceError:
        qDebug()<< QStringLiteral("MKS - Приложению не хватало необходимых прав").toUtf8();
        break;
    case QAbstractSocket::SocketTimeoutError:
        qDebug()<< QStringLiteral("MKS - Слишком много сокетов").toUtf8();
        break;
    case QAbstractSocket::DatagramTooLargeError:
        qDebug()<< QStringLiteral("MKS - Размер дейтаграммы превышал предел операционной системы").toUtf8();
        break;
    case QAbstractSocket::NetworkError:
        qDebug()<< QStringLiteral("MKS - Произошла ошибка сети (например, сетевой кабель был случайно отключен)").toUtf8();
        break;
    case QAbstractSocket::AddressInUseError:
        qDebug()<< QStringLiteral("MKS - Слишком много сокетов").toUtf8();
        break;
    case QAbstractSocket::SocketAddressNotAvailableError:
        qDebug()<< QStringLiteral("MKS - Адрес, уже используется в каком то соединении").toUtf8();
        break;
    case QAbstractSocket::UnsupportedSocketOperationError:
        qDebug()<< QStringLiteral("MKS - Адрес не принадлежит хосту").toUtf8();
        break;
    case QAbstractSocket::UnfinishedSocketOperationError:
        qDebug()<< QStringLiteral("MKS - Запрошенная операция сокета не поддерживается локальной операционной системой").toUtf8();
        break;
    case QAbstractSocket::ProxyAuthenticationRequiredError:
        qDebug()<< QStringLiteral("MKS - Подтверждение связи SSL / TLS не удалось, поэтому соединение было закрыто ").toUtf8();
        break;
    case QAbstractSocket::SslHandshakeFailedError:
        qDebug()<< QStringLiteral("MKS - Последняя попытка операции еще не завершена").toUtf8();
        break;
    case QAbstractSocket::ProxyConnectionRefusedError:
        qDebug()<< QStringLiteral("MKS - Не удалось связаться с прокси-сервером, потому что соединение с этим сервером было отказано").toUtf8();
        break;
    case QAbstractSocket::ProxyConnectionClosedError:
        qDebug()<< QStringLiteral("MKS - Соединение с прокси-сервером было неожиданно закрыто").toUtf8();
        break;
    case QAbstractSocket::ProxyConnectionTimeoutError:
        qDebug()<< QStringLiteral("MKS - Время ожидания подключения к прокси-серверу истекло или прокси-сервер перестал отвечать на этапе проверки подлинности.").toUtf8();
        break;
    case QAbstractSocket::ProxyNotFoundError:
        qDebug()<< QStringLiteral("MKS - Адрес прокси, заданный с помощью setProxy () (или прокси приложения), не найден.").toUtf8();
        break;
    case QAbstractSocket::ProxyProtocolError:
        qDebug()<< QStringLiteral("MKS - Согласование соединения с прокси-сервером не удалось, потому что ответ прокси-сервера не был понят.").toUtf8();
        break;
    case QAbstractSocket::OperationError:
        qDebug()<< QStringLiteral("MKS - Была предпринята попытка выполнения операции, когда сокет находился в недопустимом состоянии.").toUtf8();
        break;
    case QAbstractSocket::SslInternalError:
        qDebug()<< QStringLiteral("MKS -  Используемая библиотека SSL сообщила о внутренней ошибке.").toUtf8();
        break;
    case QAbstractSocket::SslInvalidUserDataError:
        qDebug()<< QStringLiteral("MKS -  Были предоставлены неверные данные (сертификат, ключ, шифр и т. Д.), И их использование привело к ошибке в библиотеке SSL.").toUtf8();
        break;
    case QAbstractSocket::TemporaryError:
        qDebug()<< QStringLiteral("MKS -  Произошла временная ошибка (например, операция будет заблокирована, а сокет не блокируется).").toUtf8();
        break;
    case QAbstractSocket::UnknownSocketError:
        qDebug()<< QStringLiteral("MKS -  Произошла неопознанная ошибка.").toUtf8();
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug()<< QStringLiteral("MKS -  Хост не найден").toUtf8();
        break;
    }
}

void MeteoServer::OnStateChanged(QAbstractSocket::SocketState state)
{
    qDebug()<< "MKS: state "<< state;
    switch (state) {
    case QAbstractSocket::UnconnectedState:
        break;
    case QAbstractSocket::HostLookupState:
        break;
    case QAbstractSocket::ConnectingState:
        break;
    case QAbstractSocket::ConnectedState:
        break;
    case QAbstractSocket::BoundState:
        break;
    case QAbstractSocket::ListeningState:
        break;
    case QAbstractSocket::ClosingState:
        break;
    default:
        break;
    }
}

void MeteoServer::SendMessage(const QByteArray &array)
{
    if (IsMeteoConnected())
    {
        m_socketMeteo->write(array);
        m_socketMeteo->flush();
        m_noAnswerTimer->start();
    }
    else
    {
        Q_EMIT ToResetQueue();
        qDebug() << "MKS: SendMessage нет подключения при попытке отправить сообщение";
    }
}

const bool MeteoServer::IsMeteoConnected() const
{
    if (m_socketMeteo==Q_NULLPTR)
    {
        return false;
    }
    else
    {
        return m_socketMeteo->state()==QAbstractSocket::ConnectedState;
    }

}

void MeteoServer::ConnectMeteoSocketObject()
{
    connect(m_noAnswerTimer, &QTimer::timeout, this, &MeteoServer::ToRequestTimeOut);
    connect(m_socketMeteo, &QTcpSocket::connected, this, &MeteoServer::OnNewSocketConnected);
    connect(m_socketMeteo, &QTcpSocket::readyRead, this, &MeteoServer::OnReadyRead);
    connect(m_socketMeteo, &QTcpSocket::disconnected, this, &MeteoServer::OnDisconnectedFromHost);
    connect(m_socketMeteo, &QTcpSocket::stateChanged, this, &MeteoServer::OnStateChanged);
    connect(m_socketMeteo, &QTcpSocket::aboutToClose, [&](){qDebug()<< "&QTcpSocket::aboutToClose";});
}

void MeteoServer::StopNoAnswerTimer()
{
    if (m_noAnswerTimer->isActive())
    {
        m_noAnswerTimer->stop();
    }
}

