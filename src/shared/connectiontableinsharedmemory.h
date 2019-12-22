#ifndef CONNECTIONTABLEINSHAREDMEMORY_H
#define CONNECTIONTABLEINSHAREDMEMORY_H


///[!] sharedmemory
#include "src/shared/sharedmemowriter.h"

#include "connectiontableinsharedmemorytypes.h"
#include "matildalimits.h"

class ConnectionTableInSharedMemory : public SharedMemoWriter
{
    Q_OBJECT
public:
    explicit ConnectionTableInSharedMemory(const QString &sharedMemoName, const QString &semaName, const int &delay, const bool &verboseMode, QObject *parent = nullptr);

    ~ConnectionTableInSharedMemory();


signals:

public slots:
    void onThreadStarted4table();

    void onRestoredMemoSlot(QVariantHash h);


    void setServerInConnIdExtData(QString conntype, QString connid, qint64 msecstart, qint64 msecend, qint64 rb, qint64 wb, QString lastmessage); //список вхідних IP адрес (АйПі збиратора ігнорується)

private:
     QVariantList markAsClosed(const QString &conntype, const qint64 &msecend, const QVariantHash &hashmirror);

     QVariantList markAsClosedOneConnType(const QString &connid, const qint64 &msecend, const QVariantHash &hashoneconntype);

//     bool removeOldEntries(QVariantHash &hashoneconntype);

};

#endif // CONNECTIONTABLEINSHAREDMEMORY_H
