#include "qosctcpserverthread.h"

QOscTcpServerThread::QOscTcpServerThread(int sd, QObject *parent) :
    QThread(parent),
    socketDescr(sd)
{
    _osc_server = new QOscServer();
}
