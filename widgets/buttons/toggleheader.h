#ifndef ToggleHeader_H
#define ToggleHeader_H

#include <QObject>
#include "buttonbase.h"

class ToggleHeader : public ButtonBase
{
    Q_OBJECT

public:
    ToggleHeader(QString text, int headerId, QObject *parent = nullptr);

signals:
    void toggleHeader(int);

protected:
    void releaseAction() override;

private:
    int _headerId;

};

#endif // ToggleHeader_H
