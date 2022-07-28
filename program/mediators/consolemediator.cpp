#include "consolemediator.h"

#include <iterator>

#if _WIN32
#include <Windows.h>
#endif

ConsoleMediator::ConsoleMediator(const std::string &logFilePath, RarmSocket *rarmSocket, MoxaMediator *moxa, UstirovMediator *ustirov, MeteoMediator *meteo, QObject *parent)
    : QObject(parent)
    , m_logFileMessagePath("Путь файлам логов: " + logFilePath)
    , m_rarmSocket(rarmSocket)
    , m_moxa(moxa)
    , m_ustirov(ustirov)
    , m_meteo(meteo)
    , m_progressIterationString("Обновление раз в " + std::to_string(m_updateIterval) + "млс... ")
{
    setlocale(LC_ALL, "Russian"); //Функция setlocale() с аргументами
#if _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif
}

ConsoleMediator::~ConsoleMediator()
{

}

void ConsoleMediator::OnStartLog()
{
    startTimer(m_updateIterval, Qt::VeryCoarseTimer);
}

void ConsoleMediator::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
    ClearConsole();
    ToPrintLogFilePath();
    PrintProcess();
    PrintRarmState();
    PrintMoxaState();
    PrintUkitState();
    PrintMeteoState();
    PrintGeoState();
}

void ConsoleMediator::ClearConsole() const noexcept
{
#ifdef Q_OS_WINDOWS
    std::system("cls");
#else
    std::system ("clear");
#endif
}

void ConsoleMediator::ToPrintLogFilePath() const noexcept
{
    std::cout << m_logFileMessagePath << std::endl;
}

void ConsoleMediator::PrintProcess() noexcept
{
    m_iteration++;
    if (20 == m_iteration)
    {
        m_iteration = 0;
#ifdef QT_DEBUG
        m_progressIterationString = "Режим отладки! Обновление раз в " + std::to_string(m_updateIterval) + " млс... ";
#else
        m_progressIterationString = "Обновление раз в " + std::to_string(m_updateIterval) + " млс... ";
#endif
    }
    else
    {
        m_progressIterationString += " " + std::to_string(m_iteration);
    }
    std::cout << m_progressIterationString << std::endl;
}

void ConsoleMediator::PrintRarmState() const noexcept
{
    std::string message = "**RARM ";
    if (m_rarmSocket->IsRarmConnected())
    {
        message += "\033[32mV\033[0m "; //Зеленая галка
    }
    else
    {
        message += "\033[31mX\033[0m "; //Красный красный крест
        message += "\n    Ошибка: " + m_rarmSocket->GetRarmError();
    }
    message += "\n    Адрес рарма: " + m_rarmSocket->GetRarmAdress() + " Порт рарма: " + std::to_string(m_rarmSocket->GetRarmPort());
    std::cout << message << std::endl;
}

void ConsoleMediator::PrintMoxaState() const noexcept
{
    std::string message = "**MOXA NPort-54501 ";
    if (m_moxa->IsMoxaConnected())
    {
        message += "\033[32mV\033[0m "; //Зеленая галка
    }
    else
    {
        message += "\033[31mX\033[0m "; //Красный красный крест
        message += "\n    Ошибка: Соединения нет";
    }
    message += "\n    Адрес мохи: " + m_moxa->GetMoxaIpAdress().toStdString();
    std::cout << message << std::endl;
}

void ConsoleMediator::PrintUkitState() const noexcept
{
    std::string message = "**Юстировочный СЮИТ.687263.035 ";
    if (m_ustirov->IsUstirovConnected())
    {
        message += "\033[32mV\033[0m "; //Зеленая галка
        message += "\n    Порт юк: " + std::to_string(m_ustirov->GetUstirovPort());
        message += "\n    Сообщений в очереди: " + std::to_string(m_ustirov->GetMessagesCount());
        const QList<QByteArray> messagesList = m_ustirov->GetMessageList();
        for (const QByteArray &currentMessage : messagesList)
        {
            message += "\n          Сообщение: " + currentMessage.toHex().toStdString();
        }
        message += "\n    Данные:";
    }
    else
    {
        message += "\033[31mX\033[0m "; //Красный красный крест
        message += "\n    Ошибка: " + m_ustirov->GetLastUstirovSocketErrorMessage().toStdString();
    }
    message += "\n    Порт юк: " + std::to_string(m_ustirov->GetUstirovPort());
    std::cout << message << std::endl;
}

void ConsoleMediator::PrintMeteoState() const noexcept
{
    std::string message = "**Метеокомплект АМЯ2.702.090 Д1 ";
    if (m_meteo->IsMeteoConnected())
    {
        message += "\033[32mV\033[0m "; //Зеленая галка
    }
    else
    {
        message += "\033[31mX\033[0m "; //Красный красный крест
        message += "\n    Ошибка: " + m_meteo->GetMeteoError();
    }
    message += "\n    Данные:" + m_meteo->GetMeteoData();
    message += "\n    Время последнего сообщения: " + m_meteo->GetLastMessageTime();
    message += "\n    Порт метео: " + std::to_string(m_meteo->GetMeteoPort());
    std::cout << message << std::endl;

}

void ConsoleMediator::PrintGeoState() const noexcept
{
//    std::string message = "**Геодатчик u-blox ZED-F9P ";
//    if (m_geo->IsGeoSocketConnected())
//    {
//        message += "\033[32mV\033[0m "; //Зеленая галка

//    }
//    else
//    {
//        message += "\033[31mX\033[0m "; //Красный красный крест
//        message += "\n    Ошибка: " + m_geo->GetError();
//    }
//    message += "\n    Данные:" + m_geo->GetGeoData();
//    std::cout << message << std::endl;
}
