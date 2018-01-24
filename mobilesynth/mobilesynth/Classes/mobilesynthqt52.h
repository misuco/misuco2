/*
 
 Copyright (C) 2013 by Claudio Zopfi, Zurich, Suisse, z@x21.ch
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 */

#ifndef MOBILESYNTHQT52_H
#define MOBILESYNTHQT52_H

#include <QAudioOutput>
#include <QByteArray>
#include <QIODevice>
#include <QMainWindow>
#include <QObject>
#include <QTimer>

#include "synth/controller.h"

class mobileSynthQT52 : public QIODevice
{
    Q_OBJECT

public:
    mobileSynthQT52();
    ~mobileSynthQT52();

    void start();
    void stop();

    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

    void noteOn(int vid, float f);
    void noteOff(int vid);

    synth::Controller * getSyctl() {return syctl;}

private:
    int DataSampleRateHz;
    int BufferSize;
    synth::Controller * syctl;
    QAudioOutput *m_audioOutput;
    QIODevice *m_output; // not owned
    QAudioFormat m_format;
    QAudioDeviceInfo m_device;
};

#endif // MOBILESYNTHQT52_H
