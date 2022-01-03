#ifndef SHAREDMEMOWRITERAPPLOGBASE_H
#define SHAREDMEMOWRITERAPPLOGBASE_H

///[!] sharedmemory
#include "src/shared/sharedmemowriter.h"


class SharedMemoWriterAppLogBase : public SharedMemoWriter
{
    Q_OBJECT
public:
    explicit SharedMemoWriterAppLogBase(const QString &sharedMemoName, const QString &semaName, const QString &write2fileName,
                                        const int &delay, const int &delay2fileMsec, const bool &verboseMode, QObject *parent = nullptr);

    struct MyAppLogBaseKeys
    {
        QString logErr;
        QString logWarn;
        QString logEvnt;


        MyAppLogBaseKeys() : logErr("err"), logWarn("wrn"), logEvnt("evnt") {}
    } myLogKeys;

    QStringList getLines(const QString &s);

    QStringList getLinesExt(const qint64 &msec, const QString &s);





signals:

public slots:
    void add2systemLogError(QString err);

    void add2systemLogWarn(QString warn);

    void add2systemLogEvent(QString evnt);

    void add2systemLogErrorExt(qint64 msec, QString err);

    void add2systemLogWarnExt(qint64 msec, QString warn);

    void add2systemLogEventExt(qint64 msec, QString evnt);


    void add2systemLogErrorList(QStringList list);

    void add2systemLogWarnList(QStringList list);

    void add2systemLogEventList(QStringList list);



    void appendLogDataSmart(const QString &key, const QStringList &log);

};

#endif // SHAREDMEMOWRITERAPPLOGBASE_H
