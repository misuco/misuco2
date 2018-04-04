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

#ifndef MWHEADERSETTER_H
#define MWHEADERSETTER_H

#include "conf/mglob.h"

class MWHeaderSetter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int functionId MEMBER _functionId CONSTANT)
    Q_PROPERTY(int pressed MEMBER _pressed NOTIFY selectedChanged)
    Q_PROPERTY(int state MEMBER _state NOTIFY selectedChanged)
    Q_PROPERTY(QString text MEMBER _text CONSTANT)
    Q_PROPERTY(bool selected READ selected NOTIFY selectedChanged)

public:
    MWHeaderSetter(int functionId, QObject *parent);
    MWHeaderSetter(int functionId, int state, QObject *parent);

    int getState();

    bool selected();

    Q_INVOKABLE void onPressed(int id);
    Q_INVOKABLE void onReleased(int id);

signals:
    void currentHeader(int i);
    void currentMainView(int i);
    void setBendHori(bool b);
    void setBendVertTop(int b);
    void setBendVertBot(int b);
    void togglePresets();
    void toggleMenu();
    void toggleBW();
    void scaleupdate();
    void toggleSender(int i);
    void toggleShowFreqs();
    void octUp();
    void octDown();

    // QML
    void selectedChanged();

public slots:
    void setState(int id, int s);

private:
    int _functionId;
    int _pressed;
    int _pressedTouchId;
    int _state;
    QString _text;
    QString midi2TextUrl(int midinote);

    void setText();
};

#endif // MWHEADERSETTER_H
