#ifndef QOSCTCPSERVERTHREAD_H
#define QOSCTCPSERVERTHREAD_H

#include <QThread>
#include <qoscserver.h>

class QOscTcpServerThread : public QThread
{
    Q_OBJECT
public:
    explicit QOscTcpServerThread(int sd, QObject *parent = 0);

signals:

public slots:

private:
    int socketDescr;
    QOscServer * _osc_server;
};

#endif // QOSCTCPSERVERTHREAD_H
