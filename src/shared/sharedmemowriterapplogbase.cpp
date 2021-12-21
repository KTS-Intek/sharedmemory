#include "sharedmemowriterapplogbase.h"


#include <QDebug>

SharedMemoWriterAppLogBase::SharedMemoWriterAppLogBase(const QString &sharedMemoName, const QString &semaName, const QString &write2fileName,
                                                       const int &delay, const int &delay2fileMsec, const bool &verboseMode, QObject *parent) :
    SharedMemoWriter(sharedMemoName, semaName, write2fileName, delay, delay2fileMsec, verboseMode, parent)
{


}

QStringList SharedMemoWriterAppLogBase::getLines(const QString &s)
{
    return QString("%1 %2")
            .arg(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz"), s).split("\n");
}


void SharedMemoWriterAppLogBase::add2systemLogError(QString err)
{
    add2systemLogErrorList(getLines(err));
}

void SharedMemoWriterAppLogBase::add2systemLogWarn(QString warn)
{
    add2systemLogWarnList(getLines(warn));

}

void SharedMemoWriterAppLogBase::add2systemLogEvent(QString evnt)
{
    add2systemLogEventList(getLines(evnt));

}

void SharedMemoWriterAppLogBase::add2systemLogErrorList(QStringList list)
{
    appendLogDataSmart(myLogKeys.logErr, list);
}

void SharedMemoWriterAppLogBase::add2systemLogWarnList(QStringList list)
{
    appendLogDataSmart(myLogKeys.logWarn, list);

}

void SharedMemoWriterAppLogBase::add2systemLogEventList(QStringList list)
{
    appendLogDataSmart(myLogKeys.logEvnt, list);

}


void SharedMemoWriterAppLogBase::appendLogDataSmart(const QString &key, const QStringList &log)
{
    appendLogData(key, log, "\n", 500);
    if(verboseMode)
        qDebug() << "appendLogDataSmart " << key << log;

}
