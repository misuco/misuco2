#ifndef PITCH_H
#define PITCH_H

#include <QObject>

class Pitch : public QObject
{
    Q_OBJECT

public:
    explicit Pitch(QObject *parent = 0);

    void setBasenote(int value);
    void setPitch(int value);

//protected:
    int pitch;
    int basenote;
    int color;

signals:
    void change();

private:
    void calcColor();
};

#endif // PITCH_H
