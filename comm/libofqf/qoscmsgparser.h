#ifndef QOSCMSGPARSER_H
#define QOSCMSGPARSER_H

#include <qosctypes.h>
#include <QAbstractSocket>

class QOscMsgParser : public QOscBase
{
    Q_OBJECT
public:
    explicit QOscMsgParser(QObject *parent = 0);

private slots:
        void readyRead();

private:
        QAbstractSocket socket();
};

#endif // QOSCMSGPARSER_H
