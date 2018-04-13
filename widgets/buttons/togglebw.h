#ifndef ToggleBw_H
#define ToggleBw_H

#include <QObject>
#include "buttonbase.h"

class ToggleBw : public ButtonBase
{
    Q_OBJECT

public:
    ToggleBw(QString text, int state, QObject *parent = nullptr);

signals:
    void toggleBW(bool);

protected:
    void releaseAction() override;
    void setAction() override;

};

#endif // ToggleBw_H
