#ifndef OPENARCHIVE_H
#define OPENARCHIVE_H

#include <QObject>

#include "buttonbase.h"
#include "lib/misulib/models/types.h"

class OpenArchive : public ButtonBase
{
    Q_OBJECT

public:
    OpenArchive(QString text, int state, QObject *parent = nullptr);

public slots:
    void onSetRootNote(int p);
    void onSetScale(int n, bool v);

protected:
    virtual void releaseAction();

private:
    int _rootNote;
    bool _scale[SCALE_SIZE];

    QString midi2TextUrl(int midinote);
};

#endif // OPENARCHIVE_H
