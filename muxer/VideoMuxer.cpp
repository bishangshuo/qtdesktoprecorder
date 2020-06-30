#include "VideoMuxer.h"
#include "capture/CaptureScreen.h"
#include "capture/RecordAudio.h"
#include "encode/EncodeVideo.h"
#include "common/Loger.h"

VideoMuxer::VideoMuxer(QObject *parent)
    : QObject(parent)
    , m_pCaptureScreen(NULL)
    , m_pRecordAudio(NULL)
    , m_pEncodeVideo(NULL)
{

}

void VideoMuxer::Init(const GLNK_VideoDataFormat &videoFmt, const GLNK_AudioDataFormat &audioFmt){
    avcodec_register_all();
    av_register_all();

    m_pCaptureScreen = new CaptureScreen();
    connect(m_pCaptureScreen, SIGNAL(sigCaptureScreen(const QPixmap &, qint64)),
            this, SLOT(slotCaptureScreen(const QPixmap &, qint64)));

    m_pEncodeVideo = new EncodeVideo();
    m_pEncodeVideo->Init(videoFmt);

    m_pRecordAudio = new RecordAudio();
    connect(m_pRecordAudio, SIGNAL(sigRecordAudio(const QByteArray &, qint64)),
            this, SLOT(slotRecordAudio(const QByteArray &, qint64)));

    m_pRecordAudio->Init(audioFmt);
}

void VideoMuxer::Start(){
    m_pCaptureScreen->StartCapture();
    m_pRecordAudio->StartRecord();
}

void VideoMuxer::Stop(){
    m_pCaptureScreen->StopCapture();
    m_pRecordAudio->StopRecord();
}

void VideoMuxer::slotCaptureScreen(const QPixmap &pixmap, qint64 timestamp){
    LOGER << "slotCaptureScreen" << EOL;
    AVPacket pkt;
    memset(&pkt, 0, sizeof(AVPacket));
    av_init_packet(&pkt);
    bool ret = m_pEncodeVideo->Encode(pixmap, timestamp, &pkt);
    if(ret){
        LOGER << "h264 encoded success" << EOL;
    }else{
        LOGER << "h264 encoded failed" << EOL;
    }
}

void VideoMuxer::slotRecordAudio(const QByteArray &audioData, qint64 timestamp){
    LOGER << "slotRecordAudio" << EOL;
}

