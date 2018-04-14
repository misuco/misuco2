#ifndef OctaveShift_H
#define OctaveShift_H

#include <QObject>
#include "buttonbase.h"

class OctaveShift : public ButtonBase
{
    Q_OBJECT

public:
    OctaveShift(QString text, bool up, QObject *parent = nullptr);

signals:
    void octUp();
    void octDown();

protected:
    void releaseAction() override;

private:
    bool _up;

};

#endif // OctaveShift_H
