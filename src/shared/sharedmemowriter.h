#ifndef SHAREDMEMOWRITER_H
#define SHAREDMEMOWRITER_H

#include "src/shared/sharedmemowritelater.h"
#include <QObject>
#include <QSharedMemory>
#include <QVariantHash>

class SharedMemoWriter : public SharedMemoWriteLater
{
    Q_OBJECT
public:
    explicit SharedMemoWriter(const QString &sharedMemoName, const QString &semaName, const QString &write2fileName, const int &delay, const int &delay2fileMsec, const bool &verboseMode, QObject *parent = nullptr);
    QVariantHash hashMirror;
    QHash<QString,QStringList> hashMirrorLogs;

    bool useMirrorLogs;

    QVariantHash getLastSavedObj() const;

    QVariantHash convertMirrorLogs();

    QVariantHash convertMirrorLogsExt(const QHash<QString, QStringList> &hashMirrorLogs);

    QHash<QString,QStringList> convertToMirrorLogs();

    QHash<QString,QStringList> convertToMirrorLogsExt(const QVariantHash &hashMirror);

    QHash<QString,QStringList> convertToMirrorLogsExtV2(const QVariantHash &hashMirror, const QHash<QString, QStringList> &inithash);

    QStringList getDataByKey(const QString &key, const QString &splitter);

    QVariantHash getCurrentMirror();

//    QByteArray getLastSavedObjArr() const;


    bool getMirrorMode() const;

signals:
    void onRestoredMemo(QVariantHash h);

    void onRestoredMemoArr(QByteArray arr);

    void ready2flushArr();


public slots:
    void setMirrorMode(const bool &isArray);

    virtual void initStarted();

    void changeSharedMemArrDataCounter();

    void setSharedMemData(QVariantHash h);

    void setSharedMemData(QString key, QVariant data);

    void setSharedMemDataLogs(const QString &key, const QStringList &logs, const QString &splitter);


    void appendShmemData(QVariantHash h);

    void appendShmemData(QStringList keys, QVariantList datal);

    void appendLogDataList(QString key, QStringList log, QString splitter, int maxLogSize); //I need it bcs I want to use new format of connecting signals/slots

    void appendLogData(QString key, QStringList log, QString splitter, int maxLogSize);

    void appendLogData(QString key, QString line, QString splitter, int maxLogSize);

    void appendLogDataTest(QString key, QString line, QString splitter, int maxLogSize);

    void appendLogDataLine(QString key, QString line, QString splitter, int maxLogSize);

    void removeTheseKeys(QStringList keys2del);

    void checkRemoveKeys(QStringList ldonotdel);

    void flushAllNow();

    void flushAllNowAndDie();

    void restoreOldVersion();

    void flushNow();

    void flushNow2file();


    void flushNowArr(QByteArray arr);


private:
    bool isArrayMode;

//    bool isConnectionReady;


};

#endif // SHAREDMEMOWRITER_H
