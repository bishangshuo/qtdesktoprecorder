#ifndef CAPTURESCREEN_H
#define CAPTURESCREEN_H

#include <QObject>

#include <QObject>
#include <QPixmap>
#include <QThread>
#include <QApplication>
#include <QDesktopWidget>

class CaptureScreen : public QThread
{
    Q_OBJECT
public:
    explicit CaptureScreen(QObject *parent = nullptr);

signals:
    void sigCaptureScreen(const QPixmap &pixmap, qint64 timestamp);
public:
    void StartCapture();
    void StopCapture();
protected:
    void run() override;
private:
    QPixmap takeScreenShot(const QRect& area);
private:
    int m_nWidth;
    int m_nHeight;
    bool m_bRun;
};

#endif // CAPTURESCREEN_H
