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

#ifndef MWFADDER_H
#define MWFADDER_H

#include <QObject>

class MWFader : public QObject
{
    Q_OBJECT

    Q_PROPERTY(float faderY MEMBER _faderY NOTIFY geoChanged)
    Q_PROPERTY(QString text1 MEMBER _text1 NOTIFY geoChanged)
    Q_PROPERTY(QString text2 MEMBER _text2 NOTIFY geoChanged)

public:
    MWFader(QObject *parent) : QObject(parent)
    {
        //qDebug() << "MWFadder::MWFadder";
        orient=vertical;
        value=0;
        inverted=false;
        _valueDisplay=value;
        pressed=0;
        setMinValue(-100);
        setMaxValue(100);
        fineness=5;
        height=500;
        _knobSize=height/8;
        calcGeo();
    }
    enum orientation {
        horizontal,
        vertical
    };
    enum FadeMode {
        coarse,
        fine
    };

    Q_INVOKABLE void onResize(int h);
    Q_INVOKABLE virtual void onPressed(int id, int x, int y, int h, int w);
    Q_INVOKABLE virtual void onUpdated(int id, int y, int h, int w);
    Q_INVOKABLE virtual void onReleased(int id);

    void setMinValue(int value);
    void setMaxValue(int value);    
    void setInverted(bool v);
    void setValue(int v);
    int getValue();

protected:
    virtual void valueChange();

    int _valueDisplay;
    float _faderY;
    int _knobSize;

    QString _text1;
    QString _text2;

    int pressed;

signals:
    // QML
    void geoChanged();

private:
    // Fadder state
    int value;
    bool inverted;
    int minValue;
    int maxValue;
    int valRange;
    int step1;
    int step2;
    int pressedTouchId;
    int orient;
    int fineness;

    int height;

    // Touch event state
    int xTouchBegin;
    int yTouchBegin;
    int valTouchBegin;
    int fadeMode;

    void calcGeo();
};

#endif // MWFADDER_H
