#ifndef SetMainView_H
#define SetMainView_H

#include <QObject>
#include "buttonbase.h"

class SetMainView : public ButtonBase
{
    Q_OBJECT

public:
    SetMainView(QString text, int viewId, QObject *parent = nullptr);

signals:
    void setMainView(int);

protected:
    void releaseAction() override;

private:
    int _viewId;

};

#endif // SetMainView_H
