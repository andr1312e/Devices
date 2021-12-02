#include "program.h"
#include <QTextCodec>

#define Utf QTextCodec::codecForLocale()->toUnicode [br]

Program::Program(QObject *parent)
    : QObject(parent)
    , m_rarmAdress(QStringLiteral("127.0.0.1"))
    , m_rarmPort(4242)
{
    CreateObjects();
    ConnectObjects();
}

Program::~Program()
{
    delete m_rarmSocket;
    delete m_meteoMediator;
    delete m_ustirovMediator;
}

void Program::CreateObjects()
{
    m_rarmSocket=new RarmSocket(m_rarmAdress, m_rarmPort, this);
    m_meteoMediator=new MeteoMediator("meteoSettings.ini", this);
    m_ustirovMediator=new UstirovMediator("ustirovSettings", this);
    qDebug()<<" UKS - ustirov kit socket";
}

void Program::ConnectObjects()
{
    connect(m_meteoMediator, &MeteoMediator::ToSendRarmMeteoState, m_rarmSocket, &RarmSocket::OnSendRarmMeteoState);
    connect(m_ustirovMediator, &UstirovMediator::ToSendRarmUstirovState, m_rarmSocket, &RarmSocket::OnSendRarmUPCBState);
    connect(m_rarmSocket, &RarmSocket::ToSetUstirovState, m_ustirovMediator, &UstirovMediator::OnSetUstirovState);
    connect(m_rarmSocket, &RarmSocket::ToGetUstirovState, m_ustirovMediator, &UstirovMediator::OnGetUstirovState);
}
