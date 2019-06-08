#ifndef SHAREDMEMOWRITELATER_H
#define SHAREDMEMOWRITELATER_H

#include <QObject>
#include "src/shared/sharedmemohelper.h"

class SharedMemoWriteLater : public QObject
{
    Q_OBJECT
public:
    explicit SharedMemoWriteLater(const QString &sharedMemoName, const QString &semaName, const QString &write2fileName, const int &delay, const int &delay2fileMsec, const bool &verboseMode, QObject *parent = nullptr);
    bool verboseMode;

    ShmemSett lastmemosett;

    QSharedMemory shmem;

    quint8 counter, counter2file;

    bool checkResetCounter();

    bool checkResetCounter2file();


signals:
    void startTmrFlush();

    void stopTmrFlush();

    void stopTmrFlush2file();


    void onRestoreOldVersion();
    void onFlushNow();
    void onFlushNow2file();
    void onInitStarted();

public slots:
    virtual void initStarted();

    void initObject(const bool &restoreWithDelay);

    void initObjectLtr();

    void checkCanFlushNow();

    void add2counterAndCheckCanFlushNow(const int &add);

    void kickOffNow();

private slots:
    void detachAndDelete();

};

#endif // SHAREDMEMOWRITELATER_H
