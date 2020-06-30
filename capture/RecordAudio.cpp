#include "RecordAudio.h"

#include <QDateTime>
#include <QCoreApplication>

//#include "common/Loger.h"

RecordAudio::RecordAudio(QObject *parent)
    : QObject(parent)
    , m_nAudioDeviceInfo(QAudioDeviceInfo::defaultInputDevice())
{

}

void RecordAudio::Init(const GLNK_AudioDataFormat &fmt){
    m_nAudioFormat.setSampleRate(fmt.samplesRate);
    m_nAudioFormat.setChannelCount(fmt.channelNumber);
    m_nAudioFormat.setSampleSize(fmt.bitrate);
    m_nAudioFormat.setSampleType(QAudioFormat::SignedInt);
    m_nAudioFormat.setByteOrder(QAudioFormat::LittleEndian);
    m_nAudioFormat.setCodec("AUDIO_S16");

    if (!m_nAudioDeviceInfo.isFormatSupported(m_nAudioFormat))
    {
        m_nAudioFormat = m_nAudioDeviceInfo.nearestFormat(m_nAudioFormat);
    }

    m_pAudioInput = new QAudioInput(m_nAudioDeviceInfo, m_nAudioFormat, this);
    m_pAudioInput->setNotifyInterval(512);
}

void RecordAudio::StartRecord(){
    connect(m_pAudioInput, SIGNAL(notify()),this, SLOT(slotAudioNotify()));
    m_pIODevice = m_pAudioInput->start();
    connect(m_pIODevice, SIGNAL(readyRead()), this, SLOT(slotAudioReadyRead()));
}

void RecordAudio::StopRecord(){
    if (m_pAudioInput) {
        m_pAudioInput->stop();
        QCoreApplication::instance()->processEvents();
        m_pAudioInput->disconnect();
    }
    m_pIODevice->close();
    m_pIODevice = 0;
}

void RecordAudio::slotAudioNotify(){
    //LOGER << "audio notify" << EOL;
}

void RecordAudio::slotAudioReadyRead(){
    QByteArray array = m_pIODevice->readAll();
    if(array.size() > 0){
        //LOGER << "audio readyread" << EOL;
        qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
        emit sigRecordAudio(array, timestamp);
    }
}
