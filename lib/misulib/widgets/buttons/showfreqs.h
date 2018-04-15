#ifndef ShowFreqs_H
#define ShowFreqs_H

#include <QObject>
#include "buttonbase.h"

class ShowFreqs : public ButtonBase
{
    Q_OBJECT

public:
    ShowFreqs(QString text, int state, QObject *parent = nullptr);

signals:
    void toggleShowFreqs(bool state);

protected:
    void releaseAction() override;
    void setAction() override;

};

#endif // ShowFreqs_H
