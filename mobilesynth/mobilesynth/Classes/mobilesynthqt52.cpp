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

#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QDebug>
#include <QVBoxLayout>
#include <qmath.h>
#include <qendian.h>

#include "mobilesynthqt52.h"

mobileSynthQT52::mobileSynthQT52()
    :   QIODevice()
    ,   m_device(QAudioDeviceInfo::defaultOutputDevice())

{
    DataSampleRateHz  = 44100;
    //BufferSize        = 4096;
    BufferSize        = 16384;

    qDebug() << "mobileSynthQT52::size " << size();
    for(auto device:QAudioDeviceInfo::availableDevices(QAudio::AudioOutput)) {
        qDebug() << "****" << device.deviceName();
        //m_device=device;
    }


    syctl = new synth::Controller();
    syctl->set_osc1_wave_type_int(0);
    syctl->set_sample_rate(DataSampleRateHz);

    m_format.setSampleRate(DataSampleRateHz);
    m_format.setChannelCount(1);
    m_format.setSampleSize(16);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    m_format=info.preferredFormat();
    qDebug() << "Preferred format is sr " << m_format.sampleRate() << " sn " << m_format.sampleSize() << " ch " << m_format.channelCount();

    if (!info.isFormatSupported(m_format)) {
        qDebug() << "Default format not supported - trying to use nearest";
        m_format = info.nearestFormat(m_format);
        info.preferredFormat();
    }

    syctl->setFormat(&m_format);

    m_audioOutput = new QAudioOutput(m_device, m_format, this);
    m_audioOutput->setBufferSize(BufferSize);
    //m_audioOutput->setBufferSize(0);

    this->start();
    qDebug() << "buffer size is " << m_audioOutput->bufferSize();

    m_audioOutput->start(this);
}

mobileSynthQT52::~mobileSynthQT52()
{
    m_audioOutput->stop();
    this->stop();
    delete(syctl);
    delete(m_audioOutput);
}

void mobileSynthQT52::start()
{
    open(QIODevice::ReadOnly);
}

void mobileSynthQT52::stop()
{
    close();
}

qint64 mobileSynthQT52::readData(char *data, qint64 len)
{
//    qDebug() << "read data " << len << " BufferSize " << m_audioOutput->bufferSize();

//TODO: why this dirty hack? why does windows request odd lens
    if(len%2!=0) {
        qDebug() << "mobileSynthQT52::readData odd len " << len;
        len-=1;
    }
    syctl->GetCharSamples(data,len);
    return len;
}

qint64 mobileSynthQT52::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);

    return 0;
}

void mobileSynthQT52::noteOn(int vid, float f)
{
    syctl->NoteOn(vid,f);
}

void mobileSynthQT52::noteOff(int vid)
{
    syctl->NoteOff(vid);
}
