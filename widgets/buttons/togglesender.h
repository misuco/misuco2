#ifndef ToggleSender_H
#define ToggleSender_H

#include <QObject>
#include "buttonbase.h"

class ToggleSender : public ButtonBase
{
    Q_OBJECT

public:
    ToggleSender(QString text, int senderId, int state, QObject *parent = nullptr);

signals:
    void toggleSender(int senderId, bool state);

protected:
    void releaseAction() override;
    void setAction() override;

private:
    int _senderId;

};

#endif // ToggleSender_H
