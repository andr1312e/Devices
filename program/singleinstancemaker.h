#ifndef SINGLEINSTANCE_H
#define SINGLEINSTANCE_H

#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QCryptographicHash>

class SingleInstanceMaker
{
public:
    explicit SingleInstanceMaker(const QString &sharedMemoryKey);
    ~SingleInstanceMaker();
public:
    bool CanRunApp() noexcept;

private:
    bool IsAnotherRunning();
    void Release() noexcept;
    QString GenerateKeyHash( const QString &key, const QString &salt ) const noexcept;

private:
    QSharedMemory    *m_SharedMem;
    QSystemSemaphore *m_lockSysSemph;

    Q_DISABLE_COPY( SingleInstanceMaker )
};

#endif // SINGLEINSTANCE_H
