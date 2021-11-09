#ifndef SHAREDMEMOPROTOCOLHELPER_H
#define SHAREDMEMOPROTOCOLHELPER_H

#include <QSharedMemory>
#include <QVariantHash>
#include <QVariantList>
#include <QList>
#include <QString>

class SharedMemoProtocolHelper
{
public:
    static QString getSemaNameAndSharedMemoryName(const qint8 &mtdExtNameIndx, QString &sharedMemoKey);

    static QVariantHash readFromSharedMemory(const qint8 &mtdExtNameIndx);

    static QVariantList readFromSharedMemoryFFledListFormat(const qint8 &mtdExtNameIndx, const bool &verboseMode);

    static QVariantList readFromSharedMemoryFFtaskFormat(const qint8 &mtdExtNameIndx, const bool &verboseMode);

    static QVariantList readFromSharedMemoryFFscheduleFormat(const qint8 &mtdExtNameIndx, const bool &verboseMode);



};

#endif // SHAREDMEMOPROTOCOLHELPER_H
