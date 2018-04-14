#ifndef ToggleMenu_H
#define ToggleMenu_H

#include <QObject>
#include "buttonbase.h"

class ToggleMenu : public ButtonBase
{
    Q_OBJECT

public:
    ToggleMenu(QString text, int state, QObject *parent = nullptr);

signals:
    void toggleMenu(bool);

protected:
    void releaseAction() override;
    void setAction() override;

};

#endif // ToggleMenu_H
