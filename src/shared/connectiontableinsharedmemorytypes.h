#ifndef CONNECTIONTABLEINSHAREDMEMORYTYPES_H
#define CONNECTIONTABLEINSHAREDMEMORYTYPES_H

#include <QString>

#define NETTABLE_CONNTYPE_TCPSERVERSOCKET   0
#define NETTABLE_CONNTYPE_TCPCLIENTSOCKET   1
#define NETTABLE_CONNTYPE_UDP_SERVICE       2
#define NETTABLE_CONNTYPE_UDP_CLIENT        3
#define NETTABLE_CONNTYPE_LOCALSERVERSOCKET 4
#define NETTABLE_CONNTYPE_LOCALCLIENTSOCKET 5


struct ConnectionTableInSharedMemoryConnection
{
    QString conntype;
    QString connid;
    qint64 msecstart;
    qint64 msecend;
    qint64 rb;
    qint64 wb;
    QString lastmessage;
    bool isServerSide;

    ConnectionTableInSharedMemoryConnection() : msecstart(0), msecend(0), rb(0), wb(0), isServerSide(false) {}


};


class ConnectionTableInSharedMemoryTypes {

public:
    static QString getConnid4tcpServerSocket(const QString &remoteip, const quint16 &serverport, const qint64 &socketdescr)
    {
        return QString("%1_%2_%3_%4").arg(NETTABLE_CONNTYPE_TCPSERVERSOCKET).arg(socketdescr).arg(remoteip).arg(QString::number(serverport));
    }

    static QString getConnid4tcpClientSocket(const QString &serverip, const quint16 &serverport, const qint64 &socketdescr)
    {
        return QString("%1_%2_%3_%4").arg(NETTABLE_CONNTYPE_TCPCLIENTSOCKET).arg(socketdescr).arg(serverip).arg(QString::number(serverport));

    }


    static QString getConnid4udpServerSocket(const QString &remoteip, const quint16 &serverport, const qint64 &socketdescr)
    {
        return QString("%1_%2_%3_%4").arg(NETTABLE_CONNTYPE_UDP_SERVICE).arg(socketdescr).arg(remoteip).arg(QString::number(serverport));

    }

    static QString getConnid4updClientSocket(const QString &serverip, const quint16 &serverport, const qint64 &socketdescr)
    {
        return QString("%1_%2_%3_%4").arg(NETTABLE_CONNTYPE_UDP_CLIENT).arg(socketdescr).arg(serverip).arg(QString::number(serverport));

    }


    static QString getConnid4localServerSocket(const QString &localservername, const qint64 &socketdescr)
    {
        return QString("%1_%2_%3").arg(NETTABLE_CONNTYPE_LOCALSERVERSOCKET).arg(socketdescr).arg(localservername);

    }

    static QString getConnid4localClientSocket(const QString &localservername, const qint64 &socketdescr)
    {
        return QString("%1_%2_%3").arg(NETTABLE_CONNTYPE_LOCALCLIENTSOCKET).arg(socketdescr).arg(localservername);

    }
};


#endif // CONNECTIONTABLEINSHAREDMEMORYTYPES_H
