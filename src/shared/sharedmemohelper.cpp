#include "sharedmemohelper.h"
#include <QDebug>
#include <QBuffer>
#include <QDataStream>
#include <QThread>
#include <QSystemSemaphore>


#include <QSaveFile>
#include <QDir>


//---------------------------------------------------------------------------------

bool SharedMemoHelper::write2sharedMemory(const QVariantHash &h, QSharedMemory &shmem, const QString &semaKey, const bool verboseMode)
{

    return write2sharedMemory(getArrFromVarHash(h), shmem, semaKey, verboseMode);
}

//---------------------------------------------------------------------------------

QByteArray SharedMemoHelper::getArrFromVarHash(const QVariantHash &h)
{
    QByteArray arr;
    QDataStream out(&arr, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6);
    out << h;
    return arr;
}

//---------------------------------------------------------------------------------

bool SharedMemoHelper::write2sharedMemory(const QByteArray &arr, QSharedMemory &shmem, const QString &semaKey, const bool verboseMode)
{
    Q_UNUSED(verboseMode);

    if(semaKey.isEmpty())
        return false;
    const int size = arr.size();


    QSystemSemaphore sema(semaKey, 1);// SemaName(), 1);
    sema.acquire();

    if(shmem.isAttached())
        shmem.detach();

    bool memoReady = false;

    for(int i = 0; i < 3 && !memoReady; i++){
        memoReady = shmem.create(size);
        if(!memoReady){
//#ifdef HASGUI4USR
//            if(verboseMode)
//                qDebug() << "shmem " << i << shmem.key() << shmem.errorString() ;
//#endif
            shmem.attach();
            shmem.detach();
            QThread::msleep(300);
        }
    }

    if(!memoReady){
        shmem.attach();
        shmem.detach();
        sema.release();
        return memoReady;
    }

    shmem.lock();
    char *to = (char *)shmem.data();
    const char *from = arr.data();
    memcpy(to, from, qMin(shmem.size(), size));
    shmem.unlock();

    sema.release();
    return memoReady;
}
//---------------------------------------------------------------------------------
QByteArray SharedMemoHelper::readFromSharedMemoryArr(const QString &sharedMemoKey, const QString &semaName)
{
    QByteArray readArr;

    if(sharedMemoKey.isEmpty() || semaName.isEmpty())
        return readArr;

    QSharedMemory memory(sharedMemoKey);


    QSystemSemaphore sema(semaName, 1);
    sema.acquire();

    if(memory.attach(QSharedMemory::ReadOnly)){
        QBuffer buffer;

        memory.lock();
        buffer.setData((char *)memory.constData(), memory.size());
        buffer.open(QBuffer::ReadOnly);
        readArr = buffer.buffer();

        memory.unlock();
        memory.detach();
    }else{
//#ifndef HASGUI4USR

//        qDebug() << "SharedMemoHelper read can't attach error = " << memory.errorString() << memory.key();
//#endif
        memory.detach();
    }
    sema.release();



    return readArr;
}

//---------------------------------------------------------------------------------

QVariantHash SharedMemoHelper::readFromSharedMemory(const QString &sharedMemoKey, const QString &semaName)
{
    QVariantHash hash;

    if(sharedMemoKey.isEmpty() || semaName.isEmpty())
        return hash;


    QSharedMemory memory(sharedMemoKey);


    QSystemSemaphore sema(semaName, 1);
    sema.acquire();



    if(memory.attach(QSharedMemory::ReadOnly)){
        QBuffer buffer;

        memory.lock();
        buffer.setData((char *)memory.constData(), memory.size());
        buffer.open(QBuffer::ReadOnly);
        QDataStream in(&buffer);
        in.setVersion(QDataStream::Qt_5_6);
        in >> hash;
        memory.unlock();
        memory.detach();


    }else{
//#ifndef HASGUI4USR

//        qDebug() << "SharedMemoHelper can't attach error = " << memory.errorString() << memory.key();
//#endif
        memory.detach();
    }
    sema.release();


    return hash;
}

//---------------------------------------------------------------------------------

SharedMemoHelper::LastFireflyStateStrct SharedMemoHelper::getFromSharedMemoryFFledStatuses(const QString &sharedMemoKey, const QString &semaName)
{
    LastFireflyStateStrct rez;
    QByteArray bufArrCompressed;

    QSharedMemory memory(sharedMemoKey);

    QSystemSemaphore sema(semaName, 1);
    sema.acquire();

    const bool r = memory.attach(QSharedMemory::ReadOnly);

    if(r){
        QBuffer buffer;
        memory.lock();
        buffer.setData((char *)memory.constData(), memory.size());
        buffer.open(QBuffer::ReadOnly);
        bufArrCompressed = buffer.data();
        memory.unlock();
    }

    memory.detach();
    sema.release();

    if(!r){
#ifndef HASGUI4USR

        qDebug() << "can't attach error = " << memory.errorString()<< memory.key();
#endif
        return rez;
    }

    if(!bufArrCompressed.isEmpty()){
        bufArrCompressed = qUncompress(bufArrCompressed);
#ifndef HASGUI4USR

        if(bufArrCompressed.isEmpty())
            qDebug() << "uncompresss error ";
#endif
    }



    if(!bufArrCompressed.isEmpty()){
        QDataStream in(&bufArrCompressed, QIODevice::ReadOnly);
        //            in >> keysList >> hash;


        in >> rez.listNi >> rez.hashNi2conf
                >> rez.schedulestates.activeScheduleLine >> rez.schedulestates.confirmedScheduleLine
                >> rez.schedulestatusActive >> rez.schedulestatusConfirmed >> rez.schedulestates.isWaiting2changePower
                >> rez.schedulestates.futureScheduleLine >> rez.schedulestates.futuremsec >> rez.schedulestatusFuture
                >> rez.schedulestates.isWaiting2changePowerConfirmed >> rez.schedulestates.isWaiting2changePowerFuture

                //protocol v8,
                >> rez.schedulestates.activeGroupsPower.regularScheduleLine >> rez.schedulestates.activeGroupsPower.specialScheduleLine     >> rez.schedulestates.activeGroupsPower.tempScheduleLine
                >> rez.schedulestates.activeGroupsPower.defScheduleLine     >> rez.schedulestates.activeGroupsPower.emergencyScheduleLine   >> rez.schedulestates.activeGroupsPower.grpIdsDynamic

                >> rez.schedulestates.confirmedGroupsPower.regularScheduleLine >> rez.schedulestates.confirmedGroupsPower.specialScheduleLine     >> rez.schedulestates.confirmedGroupsPower.tempScheduleLine
                >> rez.schedulestates.confirmedGroupsPower.defScheduleLine     >> rez.schedulestates.confirmedGroupsPower.emergencyScheduleLine   >> rez.schedulestates.confirmedGroupsPower.grpIdsDynamic

                >> rez.schedulestates.futureGroupsPower.regularScheduleLine >> rez.schedulestates.futureGroupsPower.specialScheduleLine     >> rez.schedulestates.futureGroupsPower.tempScheduleLine
                >> rez.schedulestates.futureGroupsPower.defScheduleLine     >> rez.schedulestates.futureGroupsPower.emergencyScheduleLine   >> rez.schedulestates.futureGroupsPower.grpIdsDynamic
                ;


    }


    return rez;// QPair<QStringList, QHash<QString, QHash<QString, QString> > >();
}

//---------------------------------------------------------------------------------

QVariantList SharedMemoHelper::readFromSharedMemoryFFledListFormat(const QString &sharedMemoKey, const QString &semaName)
{
    QVariantList list;

    const SharedMemoHelper::LastFireflyStateStrct indata = getFromSharedMemoryFFledStatuses(sharedMemoKey, semaName);
    for(int i = 0, iMax = indata.listNi.size(); i < iMax; i++){
        const QHash<QString,QString> h = indata.hashNi2conf.value(indata.listNi.at(i));
        const QList<QString> lk = h.keys();
        QVariantHash vh;
        for(int j = 0, jMax = lk.size(); j < jMax; j++ )
            vh.insert(lk.at(j), h.value(lk.at(j)));
        if(!vh.isEmpty())
            list.append(vh);
    }

    return list;
}

//---------------------------------------------------------------------------------

QVariantList SharedMemoHelper::readFromSharedMemoryFFtaskFormat(const QString &sharedMemoKey, const QString &semaName)
{
    QVariantList list;

    QSharedMemory memory(sharedMemoKey);

    QSystemSemaphore sema(semaName, 1);
    sema.acquire();

    if(memory.attach(QSharedMemory::ReadOnly)){

        QList<int> lTaskQueue;
        QHash<int, QVariantHash> hashTaskTable;

        if(true){
            QBuffer buffer;
            memory.lock();
            buffer.setData((char *)memory.constData(), memory.size());
            buffer.open(QBuffer::ReadOnly);
            QDataStream in(&buffer);
            in.setVersion(QDataStream::Qt_5_6);
            in >> lTaskQueue >> hashTaskTable;

            memory.unlock();
            memory.detach();
        }

        for(int i = 0, iMax = lTaskQueue.size(); i < iMax; i++){
            const QVariantHash vh = hashTaskTable.value(lTaskQueue.at(i));
            if(!vh.isEmpty())
                list.append(vh);
        }


    }else{
#ifndef HASGUI4USR

        qDebug() << "can't attach error = " << memory.errorString()<< memory.key();
#endif
        memory.detach();
    }
    sema.release();
    return list;
}

//---------------------------------------------------------------------------------

QVariantList SharedMemoHelper::readFromSharedMemoryFFscheduleFormat(const QString &sharedMemoKey, const QString &semaName)
{
    QVariantList list;
    QSharedMemory memory(sharedMemoKey);
    QSystemSemaphore sema(semaName, 1);
    sema.acquire();
    if(memory.attach(QSharedMemory::ReadOnly)){
        QHash<QString, QString> h;
        if(true){
            QBuffer buffer;
            memory.lock();
            buffer.setData((char *)memory.constData(), memory.size());
            buffer.open(QBuffer::ReadOnly);
            QDataStream in(&buffer);
            in.setVersion(QDataStream::Qt_5_6);
            in >> h;
            memory.unlock();
            memory.detach();
        }
        QList<QString> lk = h.keys();
        std::sort(lk.begin(), lk.end());
        for(int i = 0, iMax = lk.size(); i < iMax; i++){
            if(h.value(lk.at(i)).isEmpty())
                continue;
            QVariantHash vh;
            vh.insert(lk.at(i), h.value(lk.at(i)));
            list.append(vh);
        }
    }else{
#ifndef HASGUI4USR

        qDebug() << "can't attach error = " << memory.errorString()<< memory.key();
#endif
        memory.detach();
    }
    sema.release();
    return list;
}
//---------------------------------------------------------------------------------

bool SharedMemoHelper::saveSharedMemory2file(const QVariantHash &hash, const QString &fileName, QString &errString)
{
    if(fileName.isEmpty())
        return false;
    QString path2dir = fileName.left(fileName.lastIndexOf("/"));
    QDir dir(path2dir);
    if(!dir.exists())
        dir.mkpath(path2dir);

    QSaveFile sFile(fileName);
    if(sFile.open(QSaveFile::WriteOnly|QSaveFile::Unbuffered)){
        QBuffer buffer;
        buffer.open(QBuffer::WriteOnly);
        QDataStream out(&buffer);
        out.setVersion(QDataStream::Qt_5_6);
        out << hash;
        sFile.write(qCompress(buffer.data(), 9));
        errString.clear();
        if(sFile.commit())
            return true;
    }
    errString = sFile.errorString();
    return false;
}
//---------------------------------------------------------------------------------
QVariantHash SharedMemoHelper::readShmemVarHashFile(const QString &fileName, QString &errString)
{
    QVariantHash hash;
    QFile file(fileName);
    if(file.open(QFile::ReadOnly)){
        QByteArray buffer;
        buffer = qUncompress(file.readAll());
        QDataStream in(buffer);
        in.setVersion(QDataStream::Qt_5_6);
        in >> hash;
        errString.clear();
    }else{
        errString = file.errorString();
    }
    file.close();
    return hash;
}

//---------------------------------------------------------------------------------

QString SharedMemoHelper::defSemaName()                 { return QString("kts-intek.com.ua/sema")                               ; }

QString SharedMemoHelper::defSharedMemoName()           { return QString("kts-intek.com.ua/shared_memory")                      ; }

QString SharedMemoHelper::defProcManagerMemoName()      { return QString("%1/matilda/proc-manager").arg(defSharedMemoName())    ; }

QString SharedMemoHelper::defProcManagerSemaName()      { return QString("%1/matilda/proc-manager").arg(defSemaName())          ; }

QString SharedMemoHelper::defMatildaServerMemoName()    { return QString("%1/matilda/matilda-server").arg(defSharedMemoName())  ; }

QString SharedMemoHelper::defMatildaServerSemaName()    { return QString("%1/matilda/matilda-server").arg(defSemaName())        ; }


QString SharedMemoHelper::defPeredavatorMemoName()      { return QString("%1/peredavator").arg(defSharedMemoName())             ; }

QString SharedMemoHelper::defZbyratorMemoName()         { return QString("%1/zbyrator").arg(defSharedMemoName())                ; }

QString SharedMemoHelper::defZbyratorCounterMemoName()  { return QString("%1/zbyrator_counter").arg(defSharedMemoName())        ; }

QString SharedMemoHelper::defZbyratorQuickPollName()    {  return QString("%1/zbyrator_quick_poll").arg(defSharedMemoName())     ; }


QString SharedMemoHelper::defZbyratorTaskTableMemoName()
{
    return QString("%1/zbyratortasktable").arg(defSharedMemoName())        ;
}

QString SharedMemoHelper::defZbyratorTaskTableSemaName()
{
    return QString("%1/zbyratortasktable").arg(defSemaName())     ;
}


QString SharedMemoHelper::defPeredavatorSemaName()      { return QString("%1/peredavator").arg(defSemaName())                   ; }

QString SharedMemoHelper::defZbyratorSemaName()         { return QString("%1/zbyrator").arg(defSemaName())                      ; }

QString SharedMemoHelper::defZbyratorCounterSemaName()  { return QString("%1/zbyrator_counter").arg(defSemaName())              ; }

QString SharedMemoHelper::defZbyratorQuickPollSemaName(){return QString("%1/zbyrator_quick_poll").arg(defSemaName())          ; }

QString SharedMemoHelper::defAboutGsmModemMemoName()    { return QString("%1/about-gsm").arg(defSharedMemoName())                     ; }

QString SharedMemoHelper::defAboutGsmModemSemaName()    { return QString("%1/about-gsm").arg(defSemaName())              ; }

QString SharedMemoHelper::defAboutZbModemMemoName()     { return QString("%1/about-zb").arg(defSharedMemoName())                      ; }

QString SharedMemoHelper::defAboutZbModemSemaName()     { return QString("%1/about-zb").arg(defSemaName())                ; }


QString SharedMemoHelper::defSntpServerMemoName()       { return QString("%1/sntp-bbb").arg(defSharedMemoName())                      ; }

QString SharedMemoHelper::defSntpServerSemaName()       { return QString("%1/sntp-bbb").arg(defSemaName())                ; }


QString SharedMemoHelper::defSvahaServerMemoName()      { return QString("%1/svaha-bbb").arg(defSharedMemoName())                     ; }

QString SharedMemoHelper::defSvahaServerSemaName()      { return QString("%1/svaha-bbb").arg(defSemaName())               ; }


///led lamp
QString SharedMemoHelper::defFireflyLedListMemoName()   { return QString("%1/fireflyList").arg(defSharedMemoName())    ; }//it will be a lamp status

QString SharedMemoHelper::defFireflyLedListSemaName()   { return QString("%1/fireflyList").arg(defSemaName())          ; }//it will be a lamp status

//QString SharedMemoHelper::defFireflyScheduleMemoName()  { return QString("%1/fireflySchedule").arg(defSharedMemoName()); }//deprecated

//QString SharedMemoHelper::defFireflyScheduleSemaName()  { return QString("%1/fireflySchedule").arg(defSemaName())      ; }//deprecated

//QString SharedMemoHelper::defFireflyTaskMemoName()      { return QString("%1/fireflyTask").arg(defSharedMemoName())    ; }//deprecated

//QString SharedMemoHelper::defFireflyTaskSemaName()      { return QString("%1/fireflyTask").arg(defSemaName())          ; }//deprecated

QString SharedMemoHelper::defFireflyRelayMemoName()     { return QString("%1/fireflyRelay").arg(defSharedMemoName())   ; }

QString SharedMemoHelper::defFireflyRelaySemaName()     { return QString("%1/fireflyRelay").arg(defSemaName())         ; }

QString SharedMemoHelper::defFireflyStatusMemoName()    { return QString("%1/fireflyStatus").arg(defSharedMemoName())   ; }

QString SharedMemoHelper::defFireflyStatusSemaName()    { return QString("%1/fireflyStatus").arg(defSemaName())         ; }


QString SharedMemoHelper::defFireflyTempScheduleMemoName(){return QString("%1/fireflyTempSchedule").arg(defSharedMemoName()); }//deprecated

QString SharedMemoHelper::defFireflyTempScheduleSemaName(){return QString("%1/fireflyTempSchedule").arg(defSemaName())      ; }//deprecated

QString SharedMemoHelper::defFireflyTaskTableMemoName()
{
    return QString("%1/fireflytasktable").arg(defSharedMemoName())        ;

}

QString SharedMemoHelper::defFireflyTaskTableSemaName()
{
    return QString("%1/fireflytasktable").arg(defSemaName())     ;

}

QString SharedMemoHelper::defFireflyCounterMemoName()
{
    return QString("%1/firefly_counter").arg(defSharedMemoName())              ;
}

QString SharedMemoHelper::defFireflyCounterSemaName()
{
    return QString("%1/firefly_counter").arg(defSemaName())              ;
}

QString SharedMemoHelper::defFireflyAtndStatusMemoName()
{
    return QString("%1/firelyatndtool").arg(defSharedMemoName())              ;

}

QString SharedMemoHelper::defFireflyAtndStatusSemaName()
{
    return QString("%1/firelyatndtool").arg(defSemaName())              ;

}




QString SharedMemoHelper::defVpnManagerMemoName()       { return QString("%1/vpnmanager").arg(defSharedMemoName())      ; }

QString SharedMemoHelper::defVpnManagerSemaName()       { return QString("%1/vpnmanager").arg(defSemaName())            ; }


QString SharedMemoHelper::defElMeterRelayStateMemoName(){ return QString("%1/elmeterrelay").arg(defSharedMemoName())    ; }

QString SharedMemoHelper::defElMeterRelayStateSemaName(){ return QString("%1/elmeterrelay").arg(defSemaName())          ; }



//licensor
QString SharedMemoHelper::defLicensorMemoName()         { return QString("%1/licensor").arg(defSharedMemoName())                      ; }

QString SharedMemoHelper::defLicensorSemaName()         { return QString("%1/licensor").arg(defSemaName())                ; }


QStringList SharedMemoHelper::getSemaList()
{
    return QStringList() << defProcManagerSemaName()            <<
                            defMatildaServerSemaName()          <<
                            defPeredavatorSemaName()            <<
                            defZbyratorSemaName()               <<
                            defZbyratorCounterSemaName()        <<
                            defZbyratorQuickPollSemaName()      <<
                            defAboutGsmModemSemaName()          <<
                            defAboutZbModemSemaName()           <<
                            defSntpServerSemaName()             <<
                            defSvahaServerSemaName()            <<
                            defFireflyLedListSemaName()         <<
                            //                            defFireflyScheduleSemaName()        <<
                            //                            defFireflyTaskSemaName()            <<
                            defFireflyRelaySemaName()           <<
                            defFireflyStatusSemaName()          <<
                            defFireflyTempScheduleSemaName()    <<
                            defVpnManagerSemaName()             <<
                            defLicensorSemaName()               <<
                            defZbyratorTaskTableSemaName()      <<
                            defElMeterRelayStateSemaName()      <<
                            defFireflyTaskTableSemaName()       <<
                            defFireflyCounterSemaName()         <<
                            defFireflyAtndStatusSemaName()      <<
                            defMeterExchangeSemaName()          <<
                            defTcpMediumServerSemaName()        <<
                            defMatildaUartSemaName()            <<
                            defTcpMediumServerLogsSemaName()    <<
                            defUcServicesStateMemoName()            <<
                            defPeredavatorStateSemaName()       <<
                            defUcServicesStateSemaName()        <<
                            defDaAdditionalChannelsLogsSemaName() <<
                            defSqliteMediumEventsSemaName()     <<
                            defDataHolderSemaName()             <<
                            defModbusBBBLosSemaName()           <<
                            defModbusBBBServicesStateSemaName()
                            ;
}

QString SharedMemoHelper::defMeterExchangeMemoName()    { return QString("%1/meterexchange").arg(defSharedMemoName())   ; }

QString SharedMemoHelper::defMeterExchangeSemaName()    { return QString("%1/meterexchange").arg(defSemaName())         ; }


QString SharedMemoHelper::defTcpMediumServerMemoName()  { return QString("%1/tcpmediumserver").arg(defSharedMemoName()) ; }

QString SharedMemoHelper::defTcpMediumServerSemaName()  { return QString("%1/tcpmediumserver").arg(defSemaName())       ; }


QString SharedMemoHelper::defTcpMediumServerLogsMemoName()  { return QString("%1/tcpmediumserverlogs").arg(defSharedMemoName()) ; }

QString SharedMemoHelper::defTcpMediumServerLogsSemaName()  { return QString("%1/tcpmediumserverlogs").arg(defSemaName())       ; }


QString SharedMemoHelper::defMatildaUartMemoName()
{
    return QString("%1/matildauartgsmlog").arg(defSharedMemoName())   ;
}

QString SharedMemoHelper::defMatildaUartSemaName()
{
    return QString("%1/matildauartgsmlog").arg(defSemaName())         ;
}

QString SharedMemoHelper::defPeredavatorStateMemoName()
{
    return QString("%1/peredavatorbbbstate").arg(defSharedMemoName())   ;

}

QString SharedMemoHelper::defPeredavatorStateSemaName()
{
    return QString("%1/peredavatorbbbstate").arg(defSemaName())         ;

}

QString SharedMemoHelper::defUcServicesStateMemoName()
{
    return QString("%1/ucservicesstate").arg(defSharedMemoName())   ;

}

QString SharedMemoHelper::defUcServicesStateSemaName()
{
    return QString("%1/ucservicesstate").arg(defSemaName())         ;

}

QString SharedMemoHelper::defDaAdditionalChannelsLogsMemoName()
{
    return QString("%1/daadditionalchannels").arg(defSharedMemoName())   ;

}

QString SharedMemoHelper::defDaAdditionalChannelsLogsSemaName()
{
    return QString("%1/daadditionalchannels").arg(defSemaName())         ;

}

QString SharedMemoHelper::defSqliteMediumEventsMemoName()
{
    return QString("%1/sqlitemediumevents").arg(defSharedMemoName())   ;

}

QString SharedMemoHelper::defSqliteMediumEventsSemaName()
{
    return QString("%1/sqlitemediumevents").arg(defSemaName())   ;

}

QString SharedMemoHelper::defDataHolderMemoName()
{
    return QString("%1/dataholder").arg(defSharedMemoName())        ;

}

QString SharedMemoHelper::defDataHolderSemaName()
{
    return QString("%1/dataholder").arg(defSemaName())     ;

}

QString SharedMemoHelper::defModbusBBBLosMemoName()
{
    return QString("%1/modbusbbb").arg(defSharedMemoName())        ;

}

QString SharedMemoHelper::defModbusBBBLosSemaName()
{
    return QString("%1/modbusbbb").arg(defSemaName())     ;

}

QString SharedMemoHelper::defModbusBBBServicesStateMemoName()
{
    return QString("%1/modbusbbbservice").arg(defSharedMemoName())        ;
}

QString SharedMemoHelper::defModbusBBBServicesStateSemaName()
{
    return QString("%1/modbusbbbservice").arg(defSemaName())     ;

}

