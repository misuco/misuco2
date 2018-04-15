#ifndef SendCc1_H
#define SendCc1_H

#include <QObject>
#include "buttonbase.h"

class SendCc1 : public ButtonBase
{
    Q_OBJECT

public:
    SendCc1(QString text, int state, QObject *parent = nullptr);

signals:
    void sendCc1(bool);

protected:
    void releaseAction() override;
    void setAction() override;

};

#endif // SendCc1_H
