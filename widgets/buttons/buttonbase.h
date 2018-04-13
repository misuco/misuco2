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

#ifndef ButtonBase_H
#define ButtonBase_H

#include <QObject>

class ButtonBase : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString text MEMBER _text CONSTANT)
    Q_PROPERTY(bool selected READ selected NOTIFY selectedChanged)

public:
    ButtonBase(QString text, QObject *parent = nullptr);
    ButtonBase(QString text, int state, QObject *parent = nullptr);

    int getState();

    bool selected();

    Q_INVOKABLE void onPressed();
    Q_INVOKABLE void onReleased();

signals:
    void selectedChanged();

public slots:
    void setState(int s);

protected:
    virtual void pressAction();
    virtual void releaseAction();
    virtual void setAction();
    int _state;

private:
    int _pressed;
    QString _text;

};

#endif // ButtonBase_H
