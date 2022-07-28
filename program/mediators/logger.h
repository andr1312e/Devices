#ifndef MEDIATORS_LOGGER_H
#define MEDIATORS_LOGGER_H
#include <fstream>
#include <cstdio>

class Logger
{
public:
    explicit Logger(const std::string &logFileName)
        : m_logFileName(logFileName)
    {
        ClearLogFile();
    }

    ~Logger()
    {

    }
public:
    inline const std::string &GetLogFileName() const noexcept
    {
        return m_logFileName;
    }

    void Appends(std::string &&message) const noexcept
    {
        std::ofstream fout;
        fout.open(m_logFileName, std::ios::app);
        fout << message << std::endl;
        fout.flush();
        fout.close();
    }

    void Appends(const char *message) const noexcept
    {
        std::ofstream fout;
        fout.open(m_logFileName, std::ios::app);
        fout << message << std::endl;
        fout.flush();
        fout.close();
    }

private:
    inline void ClearLogFile() const noexcept
    {
        std::remove(m_logFileName.c_str());
    }


private:
    const std::string m_logFileName;
};



#endif // LOGGER_H
