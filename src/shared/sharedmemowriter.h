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

    QVariantHash getLastSavedObj() const;

//    QByteArray getLastSavedObjArr() const;


    bool getMirrorMode() const;

signals:
    void onRestoredMemo(QVariantHash h);


    void ready2flushArr();


public slots:
    void setMirrorMode(const bool &isArray);

    virtual void initStarted();

    void setSharedMemArrData();

    void setSharedMemData(QVariantHash h);

    void setSharedMemData(QString key, QVariant data);

    void appendShmemData(QVariantHash h);

    void appendShmemData(QStringList keys, QVariantList datal);


    void appendLogData(QString key, QStringList log, QString splitter, int maxLogSize);

    void appendLogData(QString key, QString line, QString splitter, int maxLogSize);

    void removeTheseKeys(QStringList keys2del);

    void checkRemoveKeys(QStringList ldonotdel);

    void flushAllNow();


    void restoreOldVersion();

    void flushNow();

    void flushNow2file();


    void flushNowArr(QByteArray arr);


private:
    bool isArrayMode;

//    bool isConnectionReady;


};

#endif // SHAREDMEMOWRITER_H
