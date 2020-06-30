#ifndef ENCODEVIDEO_H
#define ENCODEVIDEO_H

#include <QObject>
#include <QPixmap>

#include "common/def.h"

extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavutil/pixfmt.h"
    #include "libswscale/swscale.h"
    #include "libavutil/imgutils.h"
}

class EncodeVideo : public QObject
{
    Q_OBJECT
public:
    explicit EncodeVideo(QObject *parent = nullptr);

    void Init(const GLNK_VideoDataFormat &fmt);

    bool Encode(const QPixmap &pixmap, qint64 timestamp, AVPacket *pkt);
signals:
private:
    AVFrame *convertToYUV420P(const QPixmap &pixmap);
private:
    AVFormatContext *m_pFormatCtx;
    AVCodecContext* m_pCodecCtx;
    AVCodec* m_pCodec;
    uint8_t* m_pPicture_buf;
    AVFrame* m_pPicture;
};

#endif // ENCODEVIDEO_H
