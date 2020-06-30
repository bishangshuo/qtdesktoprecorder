#ifndef RECORDAUDIO_H
#define RECORDAUDIO_H

#include <QObject>
#include "common/def.h"
#include <QAudioInput>
#include <QAudioDeviceInfo>
#include <QIODevice>

class RecordAudio : public QObject
{
    Q_OBJECT
public:
    explicit RecordAudio(QObject *parent = nullptr);

    void Init(const GLNK_AudioDataFormat &fmt);
    void StartRecord();
    void StopRecord();

signals:
    void sigRecordAudio(const QByteArray &arr, qint64 timestamp);
public slots:
    void slotAudioNotify();
    void slotAudioReadyRead();
private:
    QAudioDeviceInfo m_nAudioDeviceInfo;
    QAudioInput *m_pAudioInput;
    QIODevice *m_pIODevice;
    QAudioFormat m_nAudioFormat;
};

#endif // RECORDAUDIO_H
