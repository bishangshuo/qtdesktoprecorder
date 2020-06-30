#ifndef VIDEOMUXER_H
#define VIDEOMUXER_H

#include <QObject>
#include "common/def.h"

class CaptureScreen;
class RecordAudio;
class EncodeVideo;
class EncodeAudio;

class VideoMuxer : public QObject
{
    Q_OBJECT
public:
    explicit VideoMuxer(QObject *parent = nullptr);
    void Init(const GLNK_VideoDataFormat &videoFmt, const GLNK_AudioDataFormat &audioFmt);
    void Start();
    void Stop();
signals:
public slots:
    void slotCaptureScreen(const QPixmap &pixmap, qint64 timestamp);
    void slotRecordAudio(const QByteArray &audioData, qint64 timestamp);
private:
    CaptureScreen *m_pCaptureScreen;
    RecordAudio *m_pRecordAudio;

    EncodeVideo *m_pEncodeVideo;
    EncodeAudio *m_pEncodeAudio;
};

#endif // VIDEOMUXER_H
