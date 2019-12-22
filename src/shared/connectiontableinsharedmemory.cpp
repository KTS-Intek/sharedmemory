#include "connectiontableinsharedmemory.h"

ConnectionTableInSharedMemory::ConnectionTableInSharedMemory(const QString &sharedMemoName, const QString &semaName, const int &delay, const bool &verboseMode, QObject *parent)
    :  SharedMemoWriter(sharedMemoName, semaName, "", delay, 999999, verboseMode, parent)
{

}

//--------------------------------------------------------------------------------------------------------------

ConnectionTableInSharedMemory::~ConnectionTableInSharedMemory()
{
    flushAllNow();
}

//--------------------------------------------------------------------------------------------------------------

void ConnectionTableInSharedMemory::onThreadStarted4table()
{
    connect(this, &ConnectionTableInSharedMemory::onRestoredMemo, this, &ConnectionTableInSharedMemory::onRestoredMemoSlot);
    initObjectLtr();
}

//--------------------------------------------------------------------------------------------------------------

void ConnectionTableInSharedMemory::onRestoredMemoSlot(QVariantHash h)
{
    const QList<QString> lk = h.keys();
    const qint64 msecend = QDateTime::currentMSecsSinceEpoch();

    QVariantList history = h.value("history").toList();

    for(int i = 0, imax = lk.size(); i < imax; i++){
        const QVariantList l = markAsClosed(lk.at(i), msecend, h);
        if(!l.isEmpty())
            history.append(l);
    }

    if(history.size() > MAX_SIZE_NETTABLE_HISTORY){
        history = history.mid(history.size() - MAX_SIZE_NETTABLE_HISTORY);
    }
    hashMirror.clear();//clear old data

    setSharedMemData("history", history);

}

//--------------------------------------------------------------------------------------------------------------

void ConnectionTableInSharedMemory::setServerInConnIdExtData(QString conntype, QString connid, qint64 msecstart, qint64 msecend, qint64 rb, qint64 wb, QString lastmessage)
{
    QVariantHash oneconntype = hashMirror.value(conntype).toHash();

    QVariantHash oneconnection = oneconntype.value(connid).toHash();

    if(msecend != 0 && oneconnection.isEmpty())
        return;//nothing to add

    oneconnection.insert("msec", msecstart);
    oneconnection.insert("end", msecend);//if it is still alive must be 0
    oneconnection.insert("rb", rb);
    oneconnection.insert("wb", wb);
    oneconnection.insert("etc", lastmessage);

    if(msecend == 0){ //the connection is still alive, keep it in the memory
        oneconntype.insert(connid, oneconnection);

    }else{ //conn is dead, move it to the history
        oneconntype.remove(connid);
        oneconnection.insert("id", connid);
        QVariantList history = hashMirror.value("history").toList();
        history.append(oneconnection);
        if(history.size() > MAX_SIZE_NETTABLE_HISTORY){
            history = history.mid(history.size() - MAX_SIZE_NETTABLE_HISTORY);
        }
        hashMirror.insert("history", history);
    }

//    removeOldEntries(oneconntype);
    setSharedMemData(conntype, oneconntype);
}

//--------------------------------------------------------------------------------------------------------------

QVariantList ConnectionTableInSharedMemory::markAsClosed(const QString &conntype, const qint64 &msecend, const QVariantHash &hashmirror)
{
    QVariantList history;

    QVariantHash oneconntype = hashmirror.value(conntype).toHash();
    if(oneconntype.isEmpty())
        return history;


    const QList<QString> lk = oneconntype.keys();
    for(int i = 0, imax = lk.size(); i < imax; i++){
        const QVariantList l = markAsClosedOneConnType(lk.at(i), msecend, oneconntype);
        if(!l.isEmpty())
            history.append(l);
    }
    return history;
}

//--------------------------------------------------------------------------------------------------------------

QVariantList ConnectionTableInSharedMemory::markAsClosedOneConnType(const QString &connid, const qint64 &msecend, const QVariantHash &hashoneconntype)
{
    QVariantList history;

    QVariantHash oneconnection = hashoneconntype.value(connid).toHash();

    if(oneconnection.isEmpty())
        return history;//no data

    if(oneconnection.value("end", 0).toLongLong() < 1){
        oneconnection.insert("end", msecend);


    }
    oneconnection.insert("id", connid);
    history.append(oneconnection);

    return history;
}

//--------------------------------------------------------------------------------------------------------------

//bool ConnectionTableInSharedMemory::removeOldEntries(QVariantHash &hashoneconntype)
//{
//    const QList<QString> lk = hashoneconntype.keys();
//    const int countertotal = lk.size();

//    if(countertotal <= MAX_SIZE_NETTABLE_HISTORY)
//        return false;

//    int counteralive = 0;

//    QMap<qint64,QString> mapDeadIds;

//    for(int i = 0, imax = countertotal; i < imax; i++){

//        const QVariantHash oneconnection = hashoneconntype.value(lk.at(i)).toHash();
//        if(oneconnection.value("end", 0).toLongLong() == 0){
//            counteralive++;// do not touch, it is still alive
//        }else{
//            mapDeadIds.insertMulti(oneconnection.value("end").toLongLong(), lk.at(i));
//        }
//    }

//    if(mapDeadIds.isEmpty()) //it can remove only dead connections
//        return false;

//    int theCountOfVictimsIs = countertotal - MAX_SIZE_NETTABLE_HISTORY;

//    QList<qint64> deadkeys = mapDeadIds.keys();
//    std::sort(deadkeys.begin(), deadkeys.end());

//    for(int i = 0, imax = deadkeys.size(); i < imax; i++){
//        const QList<QString> deadids = mapDeadIds.values(deadkeys.at(i));
//        const int deadidslen = deadids.size();

//        for(int j = 0; j < deadidslen; j++)
//            hashoneconntype.remove(deadids.at(j));

//        theCountOfVictimsIs -= deadidslen;
//        if(theCountOfVictimsIs < 0)
//            break;
//    }

//    return true;//some ids were deleted
//}

//--------------------------------------------------------------------------------------------------------------
