#ifndef TogglePresets_H
#define TogglePresets_H

#include <QObject>
#include "buttonbase.h"

class TogglePresets : public ButtonBase
{
    Q_OBJECT

public:
    TogglePresets(QString text, int state, QObject *parent = nullptr);

signals:
    void togglePresets(bool);

protected:
    void releaseAction() override;
    void setAction() override;

};

#endif // TogglePresets_H
