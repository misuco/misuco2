/*
 * Copyright (c) 2018 by misuco.org, Claudio Zopfi, Zurich, Switzerland, c@misuco.org
 *
 * This file is part of MISUCO2.
 *
 * MISUCO2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MISUCO2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MISUCO2.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef MWPLAYAREA_H
#define MWPLAYAREA_H

#include "lib/misulib/models/types.h"
#include "lib/misulib/comm/mastersender.h"
#include "lib/misulib/models/playfield.h"

#define MAX_COLS 255
#define MAX_ROWS 3
#define EVENT_STACK_SIZE 64

class PlayArea : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int rows MEMBER rows NOTIFY playRowsChanged)
    Q_PROPERTY(int cols MEMBER cols NOTIFY playRowsChanged)
    Q_PROPERTY(QList<QObject *> row0 READ row0 NOTIFY playRowsChanged)
    Q_PROPERTY(QList<QObject *> row1 READ row1 NOTIFY playRowsChanged)
    Q_PROPERTY(QList<QObject *> row2 READ row2 NOTIFY playRowsChanged)

public:
    struct eventStackElement {
        int eventId;
        int voiceId;
        int midinote;
        int row;
        int col;
        int x;
        int y;
        float f;
    };

    PlayArea(MasterSender * ms, QObject *parent);
    ~PlayArea();
    virtual void processTouchEvent(misuTouchEvent e);

    Q_INVOKABLE void resize(int w, int h);
    Q_INVOKABLE void onPressed(int id, int x, int y);
    Q_INVOKABLE void onUpdated(int id, int x, int y);
    Q_INVOKABLE void onReleased(int id, int x, int y);

    // game helper
    int getMidinoteAtField(int i);
    int getColumnCount();
    int getPlayAreaWidth();
    int getPlayAreaHeight();
    int getPlayFieldWidth();
    int getPlayFieldHeight();

public slots:
    void onSetRootNote(int p);
    void setScale(int n, bool v);
    void onSetScale(int rootNote, QList<bool> scale);
    void onPitchChange(int rootNote, int pitch);

    void onSendCc1(bool state);

    void setOctConf(int bottom, int top);
    void setBendHori(bool b);
    void setBendVertTop(int b);
    void setBendVertBot(int b);

    void onBwModeChange(bool state);
    void onSymbolsChange(int noteSymbols);
    void onShowFreqsChange(bool showFreqs);

signals:
    // QML
    void playRowsChanged();

private:
    // INFRASTRUCTURE

    // - scale model
    int _baseOct;
    int _topOct;
    int _rootNote;
    QList<bool> _scale;

    // - painting
    //QLinearGradient linearGrad;
    QString cap;
    int _noteSymbols;
    int _playAreaWidth;
    int _playAreaHeight;
    int _playFieldWidth;
    int _playFieldHeight;

    // - networking
    MasterSender * out;
    // - processing
    FreqTriple fcalc;

    // WORKING MEMORY
    // - touch field configuration
    Playfield fields[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
    // - event stack/hashmap
    eventStackElement eventStack[EVENT_STACK_SIZE];

    // CONFIGURATION
    // - bending
    bool bendHoriz;
    int bendVertTop;
    int bendVertBot;
    bool _sendCc1;

    // - visual
    int colwidth[MAX_COLS];
    int rowheight[MAX_ROWS];

    // helper functions
    void config();
    void setColumn(int col, int midinote, int pitch);
    void calcGeo();

    // QML
    QList<QObject *> row0();
    QList<QObject *> row1();
    QList<QObject *> row2();

};

#endif // MWPLAYAREA_H
