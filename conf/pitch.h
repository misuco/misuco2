#ifndef PITCH_H
#define PITCH_H

#include <QObject>
#include "color.h"

class Pitch : public QObject, public Color
{
    Q_OBJECT

public:
    explicit Pitch(QObject *parent = 0);

    void setBasenote(int value);

    int getHue() const override;
    int getBW() const;

//protected:
    int pitch;
    int basenote;
    int color;

signals:
    void change();

public slots:
    void setPitch(int value);

private:
    void calcColor();
};

#endif // PITCH_H
