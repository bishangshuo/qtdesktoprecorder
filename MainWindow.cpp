#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QApplication>
#include <QDesktopWidget>

extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavutil/pixfmt.h"
    #include "libswscale/swscale.h"
    #include "libavutil/imgutils.h"
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->btnStop->setEnabled(false);
    ui->btnStart->setEnabled(true);

    connect(ui->btnStart, &QPushButton::clicked, [=](){
        onStart();
    });
    connect(ui->btnStop, &QPushButton::clicked, [=](){
        onStop();
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onStart(){
    ui->btnStop->setEnabled(true);
    ui->btnStart->setEnabled(false);

    QRect screenRect = QApplication::desktop()->screenGeometry();
    int width = screenRect.width();
    int height = screenRect.height();

    m_pVideoMuxer = new VideoMuxer();

    GLNK_VideoDataFormat videoFmt;
    videoFmt.codec = AV_CODEC_ID_H264;
    videoFmt.bitrate = 2048*1024;
    videoFmt.width = width;
    videoFmt.height = height;
    videoFmt.framerate = 15;
    videoFmt.colorDepth = 1;
    videoFmt.frameInterval = 1000/15;
    videoFmt.reserve = 0;

    GLNK_AudioDataFormat audioFmt;
    audioFmt.samplesRate = 44100;
    audioFmt.bitrate = 16;
    audioFmt.waveFormat = 0;
    audioFmt.channelNumber = 2;
    audioFmt.blockAlign = 0;
    audioFmt.bitsPerSample = 0;
    audioFmt.frameInterval = 0;
    audioFmt.reserve = 0;

    m_pVideoMuxer->Init(videoFmt, audioFmt);
    m_pVideoMuxer->Start();
}

void MainWindow::onStop(){
    if(m_pVideoMuxer){
        ui->btnStop->setEnabled(false);
        ui->btnStart->setEnabled(true);
        m_pVideoMuxer->Stop();
        m_pVideoMuxer = NULL;
    }
}

