#include "EncodeAudio.h"
#include "common/Loger.h"
#include <QApplication>

EncodeAudio::EncodeAudio(QObject *parent) : QObject(parent)
{

}

void EncodeAudio::Init(const GLNK_AudioDataFormat &fmt){
    m_pCodec = avcodec_find_encoder(AV_CODEC_ID_AAC);
    if(!m_pCodec)
    {
        LOGER << "can not find AAC encoder" << EOL;
        QApplication::exit(0);
    }

    m_pCodecCtx = avcodec_alloc_context3(m_pCodec);
    m_pCodecCtx->codec_type = AVMEDIA_TYPE_AUDIO;
    m_pCodecCtx->sample_fmt = AV_SAMPLE_FMT_S16;
    m_pCodecCtx->sample_rate= fmt.samplesRate;
    m_pCodecCtx->channel_layout = AV_CH_LAYOUT_STEREO;
    m_pCodecCtx->channels = av_get_channel_layout_nb_channels(m_pCodecCtx->channel_layout);
    m_pCodecCtx->bit_rate = fmt.bitrate;

    if (avcodec_open2(m_pCodecCtx, m_pCodec, NULL) < 0){
        LOGER << "Failed to open encoder!\n" << EOL;
        QApplication::exit(0);
    }
    m_pFrame = av_frame_alloc();
    m_pFrame->nb_samples= m_pCodecCtx->frame_size;
    m_pFrame->format= m_pCodecCtx->sample_fmt;

    int size = av_samples_get_buffer_size(NULL, m_pCodecCtx->channels, m_pCodecCtx->frame_size,
                                          m_pCodecCtx->sample_fmt, 1);
    m_pPcm_buf = (uint8_t *)av_malloc(size);
    avcodec_fill_audio_frame(m_pFrame, m_pCodecCtx->channels, m_pCodecCtx->sample_fmt,
                             (const uint8_t*)m_pPcm_buf, size, 1);
}

bool EncodeAudio::Encode(uint8_t *pcm, qint64 timestamp, AVPacket *pkt){
     m_pFrame->data[0] = pcm;
     int got_packet;
     int result = avcodec_encode_audio2(m_pCodecCtx, pkt, m_pFrame, &got_packet);
     if (got_packet && result == 0){
         return true;
     }
     return false;
}
