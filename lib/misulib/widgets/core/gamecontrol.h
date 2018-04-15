#ifndef MWGAME_H
#define MWGAME_H

#include <QObject>
#include "playarea.h"
#include "lib/misulib/comm/isender.h"

class GameControl : public QObject, public ISender
{
    Q_OBJECT

    Q_PROPERTY(QString text MEMBER _text NOTIFY textChanged)
    Q_PROPERTY(bool buttonsVisible MEMBER _buttonsVisible  NOTIFY buttonsChanged)
    Q_PROPERTY(bool visible MEMBER _gameVisible  NOTIFY buttonsChanged)

public:
    GameControl(PlayArea *playArea, QObject *parent);
    ~GameControl() {}

    Q_INVOKABLE void yes();
    Q_INVOKABLE void no();

    void start();

    virtual void cc(int, int, float, float) {}
    virtual void pc(int) {}
    virtual int noteOn(float f, int midinote, int pitch, int vel);
    virtual void noteOn(int voiceId, float f, int midinote, int pitch, int v);
    virtual void noteOff(int) {}
    virtual void pitch(int, float, int, int ) {}

    virtual void setDestination(char *,int) {}
    virtual void reconnect() {}
    virtual int getPort() {return 0;}
    virtual char* getAddress() {return 0;}

signals:
    void textChanged();
    void buttonsChanged();
    void gameStarted();

protected:
    virtual void timerEvent(QTimerEvent *e);

private:
    enum Status { off, intro, play, listen, compare};
    PlayArea * _playArea;
    Status _status;
    int _statusDuration;
    int _playIndex;
    int _hearIndex;

    QList<int> _played;

    bool _gameVisible;
    QString _text;
    bool _buttonsVisible;
    bool _noteOn;
    bool _voiceId;

    int _timerId;

    void addRandNote();
    int playedErrors();

};

#endif // MWGAME_H
