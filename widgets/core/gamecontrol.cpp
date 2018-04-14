#include "gamecontrol.h"
#include <QDateTime>

GameControl::GameControl(PlayArea * playArea, QObject *parent) : QObject(parent), _playArea(playArea)
{
    _noteOn = false;
    _buttonsVisible = false;
    _gameVisible = false;
    _text = "";
}

void GameControl::yes()
{
    qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());
    if(_status == intro ) {
        emit gameStarted();
        _status = play;
        _text = "watch and LISTEN...";
        _buttonsVisible = false;
        if(_played.size()<1) {
            addRandNote();
            addRandNote();
            addRandNote();
        }
        _playIndex=0;
        _hearIndex=0;
        emit textChanged();
        emit buttonsChanged();
    }
}

void GameControl::no()
{
    _status = off;
    _gameVisible = false;
    _played.clear();
    killTimer(_timerId);
    emit buttonsChanged();
}

void GameControl::start()
{
    if(!_gameVisible) {
        _status = intro;
        _text = "Play an introductory game?";
        _buttonsVisible = true;
        _gameVisible = true;
        emit textChanged();
        emit buttonsChanged();
        _timerId = startTimer(500);
    }
}

int GameControl::noteOn(float, int, int, int)
{
    return 0;
}

void GameControl::noteOn(int, float, int midinote, int, int)
{
    if(_status == listen) {
        if(_playArea->getMidinoteAtField(_played[_hearIndex]) != midinote) {
            _text = "WRONG... Retry?";
            _buttonsVisible = true;
            _status = intro;
            emit buttonsChanged();
        } else if(_hearIndex+1 == _played.size()) {
            _text = QString("GOOD.... Your score is %1.\nReady for the next?").arg(_played.size());
            _buttonsVisible = true;
            _status = intro;
            addRandNote();
            emit buttonsChanged();
        }
        _hearIndex++;
        emit textChanged();
    }
}

void GameControl::timerEvent(QTimerEvent *) {
    // off, intro, play, listen, compare
    _statusDuration++;
    misuTouchEvent touchEvent;
    switch(_status) {
    case off:
        break;
    case intro:
        // will be transfered from yes interaction
        break;
    case play:
        touchEvent.id=9999;
        touchEvent.x = _played[_playIndex]*_playArea->getPlayFieldWidth();
        touchEvent.x += _playArea->getPlayFieldWidth()/2;
        touchEvent.y = _playArea->getPlayAreaHeight()/2;
        touchEvent.t = 0;
        if(_noteOn) {
            touchEvent.state=Qt::TouchPointReleased;
            _noteOn = false;
            _playIndex++;
            if(_playIndex>=_played.size()) {
                _text = "Now its your turn:\nPLAY what you have heard...";
                emit textChanged();
                _status = listen;
            }
        } else {
            touchEvent.state=Qt::TouchPointPressed;
            _noteOn = true;
        }
        _playArea->processTouchEvent(touchEvent);
        break;
    case listen:
        break;
    case compare:
        break;
    }
}

void GameControl::addRandNote()
{
    int note = (float)_playArea->getColumnCount() * (float)qrand()/(float)RAND_MAX;
    _played.append(note);
}

int GameControl::playedErrors()
{
    int errors=0;
    for(int i=0;i<_played.size();i++) {
    }
    return errors;
}
