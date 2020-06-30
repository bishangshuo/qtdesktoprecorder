#ifndef ENCODEAUDIO_H
#define ENCODEAUDIO_H

#include <QObject>
#include "common/def.h"

extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavutil/pixfmt.h"
    #include "libswscale/swscale.h"
    #include "libavutil/imgutils.h"
}

class EncodeAudio : public QObject
{
    Q_OBJECT
public:
    explicit EncodeAudio(QObject *parent = nullptr);

    void Init(const GLNK_AudioDataFormat &fmt);

    bool Encode(uint8_t *pcm, qint64 timestamp, AVPacket *pkt);

signals:
private:
    AVFormatContext *m_pFormatCtx;
    AVCodecContext* m_pCodecCtx;
    AVCodec* m_pCodec;
    uint8_t* m_pPcm_buf;
    AVFrame* m_pFrame;
};

#endif // ENCODEAUDIO_H
