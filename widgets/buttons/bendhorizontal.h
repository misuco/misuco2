#ifndef BendHorizontal_H
#define BendHorizontal_H

#include <QObject>
#include "buttonbase.h"

class BendHorizontal : public ButtonBase
{
    Q_OBJECT

public:
    BendHorizontal(QString text, int state, QObject *parent = nullptr);

signals:
    void setBendHori(bool);

protected:
    void releaseAction() override;
    void setAction() override;

};

#endif // BendHorizontal_H
