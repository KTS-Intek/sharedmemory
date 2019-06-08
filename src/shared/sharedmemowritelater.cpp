#include "sharedmemowritelater.h"
#include <QTimer>



SharedMemoWriteLater::SharedMemoWriteLater(const QString &sharedMemoName, const QString &semaName, const QString &write2fileName, const int &delay, const int &delay2fileMsec, const bool &verboseMode, QObject *parent) :
    QObject(parent), verboseMode(verboseMode)
{
    lastmemosett = ShmemSett(sharedMemoName, semaName, write2fileName, delay, delay2fileMsec);
    counter = counter2file = 0;
}

void SharedMemoWriteLater::initStarted()
{
    emit onInitStarted();
}

void SharedMemoWriteLater::initObject(const bool &restoreWithDelay)
{
    initStarted();
    shmem.setKey(lastmemosett.sharedMemoName);

    if(true){
        QTimer *t = new QTimer(this);
        t->setInterval(lastmemosett.delay);
        t->setSingleShot(true);

        connect(this, SIGNAL(startTmrFlush()), t, SLOT(start()) );
        connect(this, SIGNAL(stopTmrFlush()), t, SLOT(stop()) );
        connect(t, SIGNAL(timeout()), this, SIGNAL(onFlushNow()) );

    }

    if(!lastmemosett.write2fileName.isEmpty()){
        QTimer *t = new QTimer(this);
        t->setInterval(lastmemosett.delay2fileMsec);
        t->setSingleShot(true);

        connect(this, SIGNAL(startTmrFlush()), t, SLOT(start()) );
        connect(this, SIGNAL(stopTmrFlush2file()), t, SLOT(stop()) );
        connect(t, SIGNAL(timeout()), this, SIGNAL(onFlushNow2file()) );
    }
    if(restoreWithDelay)
        QTimer::singleShot(1, this, SIGNAL(onRestoreOldVersion()));
    else
        emit onRestoreOldVersion();
}



void SharedMemoWriteLater::initObjectLtr()
{
    initObject(true);
}

void SharedMemoWriteLater::checkCanFlushNow()
{
    if(counter > 5){
        emit stopTmrFlush();
        emit onFlushNow();
    }else{
        counter++;
        emit startTmrFlush();
    }

    if(counter2file > 60){
        emit stopTmrFlush2file();
        emit onFlushNow2file();
    }else{
        counter2file++;
    }

}

void SharedMemoWriteLater::add2counterAndCheckCanFlushNow(const int &add)
{
    counter += (add > 5) ? quint8(5) : quint8(add);
    checkCanFlushNow();
}

void SharedMemoWriteLater::kickOffNow()
{
    emit onFlushNow();
    emit onFlushNow2file();
    emit stopTmrFlush();
    emit stopTmrFlush2file();
    QTimer::singleShot(1, this, SLOT(detachAndDelete()));
}

void SharedMemoWriteLater::detachAndDelete()
{
    shmem.detach();
    deleteLater();

}

bool SharedMemoWriteLater::checkResetCounter()
{
    if(counter == 0)
        return false;//noting to update
    counter = 0;
    return true;
}

bool SharedMemoWriteLater::checkResetCounter2file()
{
    if(counter2file == 0 || lastmemosett.write2fileName.isEmpty())
        return false;

    counter2file = 0;
    return true;
}
