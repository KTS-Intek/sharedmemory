#include "sharedmemoprotocolhelper.h"
#include "moji_defy.h"

#include "src/shared/sharedmemohelper.h"
#include <QDebug>

//-----------------------------------------------------------------------------------------
QString SharedMemoProtocolHelper::getSemaNameAndSharedMemoryName(const qint8 &mtdExtNameIndx, QString &sharedMemoKey)
{
    QString semaName("");
    switch(mtdExtNameIndx){
    case MTD_EXT_NAME_ZBYRATOR              : sharedMemoKey = SharedMemoHelper::defZbyratorMemoName()     ; semaName = SharedMemoHelper::defZbyratorSemaName()                ; break;
    case MTD_EXT_NAME_PEREDAVATOR           : sharedMemoKey = SharedMemoHelper::defPeredavatorMemoName()  ; semaName = SharedMemoHelper::defPeredavatorSemaName()             ; break;
    case MTD_EXT_NAME_MATILDA_PROC_MANAGER  : sharedMemoKey = SharedMemoHelper::defProcManagerMemoName()  ; semaName = SharedMemoHelper::defProcManagerSemaName()             ; break;
    case MTD_EXT_NAME_MATILDA_SERVER        : sharedMemoKey = SharedMemoHelper::defMatildaServerMemoName(); semaName = SharedMemoHelper::defMatildaServerSemaName()           ; break;
    case MTD_EXT_NAME_ABOUT_GSM             : sharedMemoKey = SharedMemoHelper::defAboutGsmModemMemoName(); semaName = SharedMemoHelper::defAboutGsmModemSemaName()           ; break;
    case MTD_EXT_NAME_ABOUT_ZB              : sharedMemoKey = SharedMemoHelper::defAboutZbModemMemoName() ; semaName = SharedMemoHelper::defAboutZbModemSemaName()            ; break;

    //PROTOCOL V2 led lamps
    case MTD_EXT_NAME_FIREFLY_LEDLIST       : sharedMemoKey = SharedMemoHelper::defFireflyLedListMemoName() ; semaName = SharedMemoHelper::defFireflyLedListSemaName()        ; break;
//    case MTD_EXT_NAME_FIREFLY_SCHEDULE      : sharedMemoKey = SharedMemoHelper::defFireflysScheduleMemoName(); semaName = SharedMemoHelper::defFireflyScheduleSemaName()       ; break;
    case MTD_EXT_NAME_FIREFLY_TASK          : sharedMemoKey = SharedMemoHelper::defFireflyTaskTableMemoName();semaName = SharedMemoHelper::defFireflyTaskTableSemaName()      ; break;
    case MTD_EXT_NAME_FIREFLY_RELAY         : sharedMemoKey = SharedMemoHelper::defFireflyRelayMemoName()   ; semaName = SharedMemoHelper::defFireflyRelaySemaName()          ; break;

    case MTD_EXT_NAME_ZBYRATOR_QUICK_POLL   : sharedMemoKey = SharedMemoHelper::defZbyratorQuickPollName()  ; semaName = SharedMemoHelper::defZbyratorQuickPollSemaName()     ; break;

    case MTD_EXT_NAME_FIREFLY_STATUS        : sharedMemoKey = SharedMemoHelper::defFireflyStatusMemoName()  ; semaName = SharedMemoHelper::defFireflyStatusSemaName()         ; break;

    case MTD_EXT_NAME_SNTP_SERVICE          : sharedMemoKey = SharedMemoHelper::defSntpServerMemoName()     ; semaName = SharedMemoHelper::defSntpServerSemaName()            ; break;
    case MTD_EXT_NAME_SVAHA_SERVICE         : sharedMemoKey = SharedMemoHelper::defSvahaServerMemoName()    ; semaName = SharedMemoHelper::defSvahaServerSemaName()           ; break;
    case MTD_EXT_NAME_RELAY_STATE           : sharedMemoKey = SharedMemoHelper::defElMeterRelayStateMemoName(); semaName = SharedMemoHelper::defElMeterRelayStateSemaName()   ; break;

    default: qDebug() << "LocalSocket::readFromSharedMemory unknown index = " << mtdExtNameIndx ; break;
    }
    return semaName;
}
//-----------------------------------------------------------------------------------------
QVariantHash SharedMemoProtocolHelper::readFromSharedMemory(const qint8 &mtdExtNameIndx)
{
    QString sharedMemoKey("");
    QString semaName = SharedMemoProtocolHelper::getSemaNameAndSharedMemoryName(mtdExtNameIndx, sharedMemoKey);
    return (!sharedMemoKey.isEmpty() && !semaName.isEmpty()) ? SharedMemoHelper::readFromSharedMemory(sharedMemoKey, semaName) : QVariantHash();
}
//-----------------------------------------------------------------------------------------
QVariantList SharedMemoProtocolHelper::readFromSharedMemoryFFledListFormat(const qint8 &mtdExtNameIndx)
{
    QString sharedMemoKey("");
    QString semaName = SharedMemoProtocolHelper::getSemaNameAndSharedMemoryName(mtdExtNameIndx, sharedMemoKey);
    return (!sharedMemoKey.isEmpty() && !semaName.isEmpty()) ? SharedMemoHelper::readFromSharedMemoryFFledListFormat(sharedMemoKey, semaName) : QVariantList();
}
//-----------------------------------------------------------------------------------------
QVariantList SharedMemoProtocolHelper::readFromSharedMemoryFFtaskFormat(const qint8 &mtdExtNameIndx)
{
    QString sharedMemoKey("");
    QString semaName = SharedMemoProtocolHelper::getSemaNameAndSharedMemoryName(mtdExtNameIndx, sharedMemoKey);
    return (!sharedMemoKey.isEmpty() && !semaName.isEmpty()) ? SharedMemoHelper::readFromSharedMemoryFFtaskFormat(sharedMemoKey, semaName) : QVariantList();
}
//-----------------------------------------------------------------------------------------
QVariantList SharedMemoProtocolHelper::readFromSharedMemoryFFscheduleFormat(const qint8 &mtdExtNameIndx)
{
    QString sharedMemoKey("");
    QString semaName = SharedMemoProtocolHelper::getSemaNameAndSharedMemoryName(mtdExtNameIndx, sharedMemoKey);

    return (!sharedMemoKey.isEmpty() && !semaName.isEmpty()) ? SharedMemoHelper::readFromSharedMemoryFFscheduleFormat(sharedMemoKey, semaName) : QVariantList();

}
//-----------------------------------------------------------------------------------------
