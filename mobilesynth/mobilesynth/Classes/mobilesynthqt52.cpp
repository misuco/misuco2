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
    ,   m_pullTimer(new QTimer(this))
    ,   m_device(QAudioDeviceInfo::defaultOutputDevice())

{
    DataSampleRateHz  = 44100;
    const int BufferSize        = 32768;
    //BufferSize        = 8192;

    m_buffer.resize(BufferSize);
    m_buffer.fill(0);

    syctl = new synth::Controller();
    //syctl->set_modulation_amount(0);
    //syctl->set_modulation_frequency(0);
    //syctl->set_modulation_destination(synth::Controller::LFO_DEST_NONE);
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

    //delete m_audioOutput;
    //m_audioOutput = 0;
    m_audioOutput = new QAudioOutput(m_device, m_format, this);
    m_audioOutput->setBufferSize(BufferSize);
//    connect(m_audioOutput, SIGNAL(notify()), SLOT(notified()));
//    connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), SLOT(handleStateChanged(QAudio::State)));
    this->start();

    // way 1: push mode
    //m_audioOutput->start(this);

    // way 2: pull mode
    connect(m_pullTimer, SIGNAL(timeout()), SLOT(pullTimerExpired()));
    m_output=m_audioOutput->start();
    m_pullTimer->start(20);
}

mobileSynthQT52::~mobileSynthQT52()
{
    m_pullTimer->stop();
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
    /*
    qint64 total = 0;
    while (len - total > 0) {
        const qint64 chunk = qMin((m_buffer.size() - m_pos), len - total);
        memcpy(data + total, m_buffer.constData() + m_pos, chunk);
        m_pos = (m_pos + chunk) % m_buffer.size();
        total += chunk;
    }
    */

    //qDebug() << "readData len " << len;
//TODO: why this dirty hack? why does windows request odd lens
    if(len%2!=0) len-=1;
    syctl->GetCharSamples(data,len);
//    return total;
    return len;

}

qint64 mobileSynthQT52::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);

    return 0;
}

qint64 mobileSynthQT52::bytesAvailable() const
{
    return m_buffer.size() + QIODevice::bytesAvailable();
}

void mobileSynthQT52::noteOn(int vid, float f)
{
    syctl->NoteOn(vid,f);
}

void mobileSynthQT52::noteOff(int vid)
{
    syctl->NoteOff(vid);
}

void mobileSynthQT52::pullTimerExpired()
{
    if (m_audioOutput && m_audioOutput->state() != QAudio::StoppedState) {
        int chunks = m_audioOutput->bytesFree()/m_audioOutput->periodSize();
        //qDebug() << "pullTimerExpired chunks " << chunks << " bytes free " << m_audioOutput->bytesFree();
        while (chunks) {
           //qDebug() << "pull timer read " << m_audioOutput->periodSize();
           const qint64 len = this->read(m_buffer.data(), m_audioOutput->periodSize());
           //qDebug() << "pull timer loop len " << len << " chunks " << chunks;
           if (len)
               m_output->write(m_buffer.data(), len);
           if (len != m_audioOutput->periodSize())
               break;
           --chunks;
        }
    }
}
