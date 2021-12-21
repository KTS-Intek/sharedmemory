#ifndef SHAREDMEMOHELPER_H
#define SHAREDMEMOHELPER_H

#include <QSharedMemory>
#include <QVariantHash>
#include <QVariantList>


#include "fireflyschedulerv2types.h"

/*
 * в цьому класі виконуються операції з спільною пам'яттю,
 * запис/читання в спільну пам'ять
 * запис/читання проміжних даних в постійну пам'ять
*/

struct ShmemSett
{
    QString sharedMemoName;
    QString semaName;
    QString write2fileName;
    int delay;
    int delay2fileMsec;
    ShmemSett() {}
    ShmemSett(const QString &sharedMemoName, const QString &semaName, const QString &write2fileName, const int &delay, const int &delay2fileMsec) :
        sharedMemoName(sharedMemoName), semaName(semaName), write2fileName(write2fileName), delay(delay), delay2fileMsec(delay2fileMsec) {}
} ;

class SharedMemoHelper
{


public:

    struct LastFireflyStateStrct
    {
        QStringList listNi;
        QHash<QString, QHash<QString,QString> > hashNi2conf;//кешована версія БД
        ScheduleStatesStruct  schedulestates;

        QString schedulestatusActive;
        QString schedulestatusConfirmed;
        QString schedulestatusFuture;

        LastFireflyStateStrct() {}
    };


    static bool write2sharedMemory(const QVariantHash &h, QSharedMemory &shmem, const QString &semaKey, const bool verboseMode = false);

    static QByteArray getArrFromVarHash(const QVariantHash &h);

    static bool write2sharedMemory(const QByteArray &arr, QSharedMemory &shmem, const QString &semaKey, const bool verboseMode = false);

    static QByteArray readFromSharedMemoryArr(const QString &sharedMemoKey, const QString &semaName);

    static QVariantHash readFromSharedMemory(const QString &sharedMemoKey, const QString &semaName);


    static LastFireflyStateStrct getFromSharedMemoryFFledStatuses(const QString &sharedMemoKey, const QString &semaName, const bool &verboseMode);

    static QVariantList readFromSharedMemoryFFledListFormat(const QString &sharedMemoKey, const QString &semaName, const bool &verboseMode);


    static QVariantList readFromSharedMemoryFFtaskFormat(const QString &sharedMemoKey, const QString &semaName, const bool &verboseMode);

    static QVariantList readFromSharedMemoryFFscheduleFormat(const QString &sharedMemoKey, const QString &semaName, const bool &verboseMode);

    static bool saveSharedMemory2file(const QVariantHash &hash, const QString &fileName, QString &errString);

    static QVariantHash readShmemVarHashFile(const QString &fileName, QString &errString);


    static QString defSemaName();

    static QString defSharedMemoName();



//procManager
    static QString defProcManagerMemoName();

    static QString defProcManagerSemaName();



    //matilda-server
    static QString defMatildaServerMemoName();

    static QString defMatildaServerSemaName();

    static QString defPeredavatorMemoName();

    static QString defZbyratorMemoName();

    static QString defZbyratorCounterMemoName();

    static QString defZbyratorQuickPollName();

    static QString defZbyratorTaskTableMemoName();

    static QString defZbyratorTaskTableSemaName();


    static QString defPeredavatorSemaName();

    static QString defZbyratorSemaName();

    static QString defZbyratorCounterSemaName();

    static QString defZbyratorQuickPollSemaName();


    static QString defAboutGsmModemMemoName();

    static QString defAboutGsmModemSemaName();

    static QString defAboutZbModemMemoName();

    static QString defAboutZbModemSemaName();


    static QString defSntpServerMemoName();

    static QString defSntpServerSemaName();


    static QString defSvahaServerMemoName();

    static QString defSvahaServerSemaName();


    static QString defHttpServerMemoName();

    static QString defHttpServerSemaName();

    static QString defHttpServicesStateMemoName();

    static QString defHttpServicesStateSemaName();



    ///led lamp
    static QString defFireflyLedListMemoName();

    static QString defFireflyLedListSemaName();

//    static QString defFireflyScheduleMemoName();

//    static QString defFireflyScheduleSemaName();

//    static QString defFireflyTaskMemoName();

//    static QString defFireflyTaskSemaName();

    static QString defFireflyRelayMemoName();

    static QString defFireflyRelaySemaName();

    static QString defFireflyStatusMemoName();

    static QString defFireflyStatusSemaName();

    static QString defFireflyTempScheduleMemoName();

    static QString defFireflyTempScheduleSemaName();


    static QString defFireflyTaskTableMemoName();

    static QString defFireflyTaskTableSemaName();


    static QString defFireflyCounterMemoName();

    static QString defFireflyCounterSemaName();


    static QString defFireflyAtndStatusMemoName();

    static QString defFireflyAtndStatusSemaName();

    //vpn

    static QString defVpnManagerMemoName();

    static QString defVpnManagerSemaName();


    //prepaid
//    static QString defPrepaidFileSemaName(const QString &dateStr); wtf???

    //relay

    static QString defElMeterRelayStateMemoName();

    static QString defElMeterRelayStateSemaName();



    //licensor
    static QString defLicensorMemoName();

    static QString defLicensorSemaName();

    static QStringList getSemaList();


    static QString defMeterExchangeMemoName();

    static QString defMeterExchangeSemaName();


    //tcp medium
    static QString defTcpMediumServerMemoName();

    static QString defTcpMediumServerSemaName();


    static QString defTcpMediumServerLogsMemoName();

    static QString defTcpMediumServerLogsSemaName();

    //matilda-uart
    static QString defMatildaUartMemoName();

    static QString defMatildaUartSemaName();


    //peredavator-bbb state protocol v8
    static QString defPeredavatorStateMemoName();

    static QString defPeredavatorStateSemaName();


    static QString defUcServicesStateMemoName();

    static QString defUcServicesStateSemaName();


    static QString defDaAdditionalChannelsLogsMemoName();

    static QString defDaAdditionalChannelsLogsSemaName();

    //sqlite-medium
    static QString defSqliteMediumEventsMemoName();

    static QString defSqliteMediumEventsSemaName();


    //data-holder
    static QString defDataHolderMemoName();

    static QString defDataHolderSemaName();

    //modbus-bbb
    static QString defModbusBBBLosMemoName();

    static QString defModbusBBBLosSemaName();

    static QString defModbusBBBServicesStateMemoName();

    static QString defModbusBBBServicesStateSemaName();



};

#endif // SHAREDMEMOHELPER_H
