#include "EncodeVideo.h"
#include "common/Loger.h"
#include <QApplication>

EncodeVideo::EncodeVideo(QObject *parent) : QObject(parent)
{

}

void EncodeVideo::Init(const GLNK_VideoDataFormat &fmt){
    int size;

    m_pCodec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if(!m_pCodec)
    {
        LOGER << "can not find h264 encoder" << EOL;
        QApplication::exit(0);
    }

    m_pCodecCtx = avcodec_alloc_context3(m_pCodec);
    m_pCodecCtx->codec_id = AV_CODEC_ID_H264;
    m_pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    m_pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    m_pCodecCtx->width = fmt.width;
    m_pCodecCtx->height = fmt.height;
    m_pCodecCtx->time_base.num = 1;
    m_pCodecCtx->time_base.den = fmt.framerate;
    m_pCodecCtx->framerate = {1, fmt.framerate};
    m_pCodecCtx->bit_rate = fmt.bitrate;
    m_pCodecCtx->gop_size = 12;

    if (m_pCodecCtx->codec_id == AV_CODEC_ID_H264)
    {
        m_pCodecCtx->refs = 3;
        m_pCodecCtx->qmin = 10;
        m_pCodecCtx->qmax = 51;
        m_pCodecCtx->qcompress = 0.6;
    }
    if (m_pCodecCtx->codec_id == AV_CODEC_ID_MPEG2VIDEO)
        m_pCodecCtx->max_b_frames = 2;
    if (m_pCodecCtx->codec_id == AV_CODEC_ID_MPEG1VIDEO)
        m_pCodecCtx->mb_decision = 2;

    // some formats want stream headers to be separate
    if (m_pCodecCtx->flags & AVFMT_GLOBALHEADER)
        m_pCodecCtx->flags |= CODEC_FLAG_GLOBAL_HEADER;

    // Set Option
    AVDictionary *param = 0;
    av_dict_set(&param, "preset", "superfast", 0);
    av_dict_set(&param, "tune", "zerolatency", 0);


    m_pCodec = avcodec_find_encoder(m_pCodecCtx->codec_id);
    if (!m_pCodec){
        LOGER << "Can not find video encoder!" << EOL;
        QApplication::exit(0);
    }

    if (avcodec_open2(m_pCodecCtx, m_pCodec, &param) < 0){
        LOGER << "Failed to open video encoder!" << EOL;
        QApplication::exit(0);
    }

    m_pPicture = avcodec_alloc_frame();
    size = avpicture_get_size(m_pCodecCtx->pix_fmt, m_pCodecCtx->width, m_pCodecCtx->height);
    m_pPicture_buf = (uint8_t *)av_malloc(size);
    avpicture_fill((AVPicture *)m_pPicture, m_pPicture_buf, m_pCodecCtx->pix_fmt,
                   m_pCodecCtx->width, m_pCodecCtx->height);
}

AVFrame *EncodeVideo::convertToYUV420P(const QPixmap &pixmap){
    QImage image = pixmap.toImage();
    int w = image.width();
    int h = image.height();

    AVFrame *pFrameRGB = av_frame_alloc();
    int numBytes1 = av_image_get_buffer_size(AV_PIX_FMT_RGB32, w, h, 1);
    uint8_t *buffer1 = (uint8_t *)av_malloc(numBytes1*sizeof(uint8_t));

    av_image_fill_arrays(pFrameRGB->data, pFrameRGB->linesize, buffer1, AV_PIX_FMT_RGB32, w, h, 1);
    pFrameRGB->data[0] = image.bits();

    SwsContext *  rgb_to_yuv_ctx = sws_getContext(w,h, AV_PIX_FMT_RGB32,
                                  w,h, AV_PIX_FMT_YUV420P,
                                  SWS_BICUBIC, NULL,NULL,NULL);

    sws_scale(rgb_to_yuv_ctx, pFrameRGB->data, pFrameRGB->linesize, 0,
                h, m_pPicture->data, m_pPicture->linesize);

    sws_freeContext(rgb_to_yuv_ctx);
    av_free(buffer1);
    av_free(pFrameRGB);

    return m_pPicture;
}

bool EncodeVideo::Encode(const QPixmap &pixmap, qint64 timestamp, AVPacket *pkt){
    if(pkt  == NULL){
        return false;
    }
    m_pPicture = convertToYUV420P(pixmap);
    if(m_pPicture){
        int got_packet;
        int result = avcodec_encode_video2(m_pCodecCtx, pkt, m_pPicture, &got_packet);
        if (got_packet && result == 0){
            return true;
        }
    }
    return false;
}
