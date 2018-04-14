#ifndef PITCHCOLOR_H
#define PITCHCOLOR_H

#include <QObject>
#include <QColor>

class PitchColor : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QColor colorOff MEMBER _colorOff NOTIFY colorChanged)
    Q_PROPERTY(QColor colorOn MEMBER _colorOn NOTIFY colorChanged)

public:
    explicit PitchColor(int rootNote, QObject *parent = nullptr);

signals:
    void colorChanged();

public slots:
    void onPitchChange(int rootNote, int value);
    void onBwModeChange(bool bwMode);

private:
    int _rootNote;
    int _pitch;
    bool _bwMode;
    bool _pianoWhite;

    QColor _colorOff;
    QColor _colorOn;

    void calcColor();

};

#endif // PITCHCOLOR_H
