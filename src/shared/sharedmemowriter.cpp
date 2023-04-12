#include "sharedmemowriter.h"
#include <QTimer>
#include <QDebug>

SharedMemoWriter::SharedMemoWriter(const QString &sharedMemoName, const QString &semaName, const QString &write2fileName,
                                   const int &delay, const int &delay2fileMsec, const bool &verboseMode, QObject *parent) :
    SharedMemoWriteLater(sharedMemoName, semaName, write2fileName, delay, delay2fileMsec, verboseMode, parent)
{
    setMirrorMode(false);

    useMirrorLogs = false;

//    isConnectionReady = false;
}

QVariantHash SharedMemoWriter::getLastSavedObj() const
{
    return hashMirror;
}

QVariantHash SharedMemoWriter::convertMirrorLogs()
{
    return convertMirrorLogsExt(hashMirrorLogs);
}

QVariantHash SharedMemoWriter::convertMirrorLogsExt(const QHash<QString, QStringList> &hashMirrorLogs)
{
    QVariantHash h;
    const QList<QString> lk = hashMirrorLogs.keys();
    for(int i = 0, imax = lk.size(); i < imax; i++){
        const QStringList l = hashMirrorLogs.value(lk.at(i));
        const QString line = l.join("\n");
        const QVariant v = QVariant(line);
        h.insert(lk.at(i), v);

    }
    return h;
}

QHash<QString, QStringList> SharedMemoWriter::convertToMirrorLogs()
{
    return convertToMirrorLogsExt(hashMirror);
}

QHash<QString, QStringList> SharedMemoWriter::convertToMirrorLogsExt(const QVariantHash &hashMirror)
{
    QHash<QString, QStringList> h;
    return convertToMirrorLogsExtV2(hashMirror, h);
}

QHash<QString, QStringList> SharedMemoWriter::convertToMirrorLogsExtV2(const QVariantHash &hashMirror, const QHash<QString, QStringList> &inithash)
{
    QHash<QString, QStringList> h = inithash;
    const QList<QString> lk = hashMirror.keys();
    for(int i = 0, imax = lk.size(); i < imax; i++)
        h.insert(lk.at(i), hashMirror.value(lk.at(i)).toString().split("\n"));
    return h;
}

QStringList SharedMemoWriter::getDataByKey(const QString &key, const QString &splitter)
{
    return useMirrorLogs ?
                hashMirrorLogs.value(key) :
                hashMirror.value(key).toString().split(splitter,
                                                   #if QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
                                                       Qt::SkipEmptyParts
                                                   #else
                                                       QString::SkipEmptyParts
                                                   #endif
                                                       );
}

QVariantHash SharedMemoWriter::getCurrentMirror()
{
    return useMirrorLogs ?
                convertMirrorLogs() :
                hashMirror;
}

//QByteArray SharedMemoWriter::getLastSavedObjArr() const
//{
//    return arrMirror;
//}

bool SharedMemoWriter::getMirrorMode() const
{
    return isArrayMode;
}

void SharedMemoWriter::setMirrorMode(const bool &isArray)
{
    isArrayMode = isArray;
}

void SharedMemoWriter::initStarted()
{
    connect(this, &SharedMemoWriter::onFlushNow, this, &SharedMemoWriter::flushNow);
    connect(this, &SharedMemoWriter::onRestoreOldVersion, this, &SharedMemoWriter::restoreOldVersion);
    connect(this, &SharedMemoWriter::onFlushNow2file, this, &SharedMemoWriter::flushNow2file);
}

void SharedMemoWriter::changeSharedMemArrDataCounter()//only for counter, you must wait for ready2flushArr()
{
    if(!isArrayMode)
        return;
    checkCanFlushNow();
}





void SharedMemoWriter::setSharedMemData(QVariantHash h)
{    
    if(isArrayMode)
        return;


    if(useMirrorLogs){
        hashMirrorLogs = convertToMirrorLogsExt(h);//it takes data from
    }else{
         hashMirror = h;
    }

    checkCanFlushNow();
}



void SharedMemoWriter::setSharedMemData(QString key, QVariant data)
{
//    if(verboseMode)
//        qDebug() << "setSharedMemData " << key << data;
    if(isArrayMode)
        return;

    if(useMirrorLogs)
        hashMirrorLogs.insert(key, data.toString().split("\n"));
    else
        hashMirror.insert(key, data);

    checkCanFlushNow();
}

void SharedMemoWriter::setSharedMemDataLogs(const QString &key, const QStringList &logs, const QString &splitter)
{
    if(useMirrorLogs){
        hashMirrorLogs.insert(key, logs);
        checkCanFlushNow();
    }else{
        setSharedMemData(key, logs.join(splitter));
    }
}



void SharedMemoWriter::appendShmemData(QVariantHash h)
{
    if(isArrayMode)
        return;

    const int iMax = h.size();

    if(useMirrorLogs){
        hashMirrorLogs = convertToMirrorLogsExtV2(h, hashMirrorLogs);
    }else{

        const QList<QString> lk = h.keys();
        for(int i = 0; i < iMax; i++)
            hashMirror.insert(lk.at(i), h.value(lk.at(i)));

    }
    add2counterAndCheckCanFlushNow(iMax);


}



void SharedMemoWriter::appendShmemData(QStringList keys, QVariantList datal)
{
    if(isArrayMode)
        return;
    const int iMax0 = keys.size();
    const int iMax1 = datal.size();

    if(iMax0 != iMax1 && verboseMode)
        qDebug() << "SharedMemoWriter::appendShmemData iMax0 != iMax1" << iMax0 << iMax1 << shmem.key();

    for(int i = 0; i < iMax0 && i < iMax1; i++)
        hashMirror.insert(keys.at(i), datal.at(i));
    add2counterAndCheckCanFlushNow(iMax0);
}

void SharedMemoWriter::appendLogDataList(QString key, QStringList log, QString splitter, int maxLogSize)
{
    appendLogData(key, log, splitter, maxLogSize);
}



void SharedMemoWriter::appendLogData(QString key, QStringList log, QString splitter, int maxLogSize)
{
    if(isArrayMode)
        return;
    QStringList l = getDataByKey(key, splitter);
    l.append(log);
    const int r = l.size();
    if(r > (maxLogSize * 1.1))
        l = l.mid(r - maxLogSize, maxLogSize);

    setSharedMemDataLogs(key, l, splitter);

}

void SharedMemoWriter::appendLogData(QString key, QString line, QString splitter, int maxLogSize)
{
    appendLogData(key, QStringList(line), splitter, maxLogSize);
}

void SharedMemoWriter::appendLogDataTest(QString key, QString line, QString splitter, int maxLogSize)
{

    appendLogData(key, QStringList(line), splitter, maxLogSize);
}

void SharedMemoWriter::appendLogDataLine(QString key, QString line, QString splitter, int maxLogSize)
{
    appendLogData(key, QStringList(line), splitter, maxLogSize);

}

void SharedMemoWriter::removeTheseKeys(QStringList keys2del)
{
    if(isArrayMode)
        return;

    if(useMirrorLogs){
        for(int i = 0, imax = keys2del.size(); i < imax; i++)
            hashMirrorLogs.remove(keys2del.at(i));
    }else{

        for(int i = 0, imax = keys2del.size(); i < imax; i++)
            hashMirror.remove(keys2del.at(i));
    }
}

void SharedMemoWriter::checkRemoveKeys(QStringList ldonotdel)
{
    if(isArrayMode)
        return;
    const QList<QString> lk = hashMirror.keys();
    QStringList keys2del;
    for(int i = 0, imax = lk.size(); i < imax; i++){
        if(ldonotdel.contains(lk.at(i)))
            continue;
        keys2del.append(lk.at(i));
    }
    removeTheseKeys(keys2del);
}

void SharedMemoWriter::flushAllNow()
{
    flushNow();
    flushNow2file();
}

void SharedMemoWriter::flushAllNowAndDie()
{
    flushAllNow();
    deleteLater();
}



void SharedMemoWriter::restoreOldVersion()
{
    if(getMirrorMode()){
       const QByteArray arr = SharedMemoHelper::readFromSharedMemoryArr(lastmemosett.sharedMemoName, lastmemosett.semaName);
        if(!arr.isEmpty()){
//            flushNowArr(arr);
            emit onRestoredMemoArr(arr);
        }
        return;
    }
    const QVariantHash h = SharedMemoHelper::readFromSharedMemory(lastmemosett.sharedMemoName, lastmemosett.semaName);
    if(!h.isEmpty()){
        setSharedMemData(h);
        emit onRestoredMemo(h);
        return;
    }

    if(!lastmemosett.write2fileName.isEmpty()){
        QString err;
        const QVariantHash hh = SharedMemoHelper::readShmemVarHashFile(lastmemosett.write2fileName, err);
        if(!hh.isEmpty()){
            setSharedMemData(hh);
            emit onRestoredMemo(hh);
            return;
        }
    }

    setSharedMemData(QVariantHash());
    emit onRestoredMemo(QVariantHash());
}



void SharedMemoWriter::flushNow()
{
    if(!checkResetCounter())
        return;//noting to update

    if(isArrayMode){
        emit ready2flushArr();//кажу щоб мені переслали дані
        return;
    }

    if(lastmemosett.sharedMemoName.isEmpty())
        return;
    const QVariantHash h = getCurrentMirror();

    const bool r =
            SharedMemoHelper::write2sharedMemory(h, shmem, lastmemosett.semaName, verboseMode);

    if(verboseMode)
        qDebug() << "SharedMemoWriter::flushNow " << lastmemosett.sharedMemoName << r <<  hashMirror.keys() << shmem.errorString();
    //const QVariantHash &h, QSharedMemory &shmem, const QString &semaKey, const bool verboseMode)

}



void SharedMemoWriter::flushNow2file()
{
    if(isArrayMode)
        return;

    if(!checkResetCounter2file())
        return;

    if(lastmemosett.write2fileName.isEmpty())
        return;
    QString err;
    const bool b = SharedMemoHelper::saveSharedMemory2file(getCurrentMirror(), lastmemosett.write2fileName, err);
    if(!b || verboseMode)
        qDebug() << "SharedMemoWriter save 2 file " << b << err << lastmemosett.write2fileName << hashMirror.keys() ;

}

void SharedMemoWriter::flushNowArr(QByteArray arr)
{
    if(!isArrayMode)
        return;
    if(lastmemosett.sharedMemoName.isEmpty())
        return;
    const bool r =
            SharedMemoHelper::write2sharedMemory(arr, shmem, lastmemosett.semaName, verboseMode);

    if(verboseMode)
        qDebug() << "SharedMemoWriter::flushNow " << lastmemosett.sharedMemoName << r << arr.length() ;

    //const QVariantHash &h, QSharedMemory &shmem, const QString &semaKey, const bool verboseMode)

}




