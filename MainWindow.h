#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "muxer/VideoMuxer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void onStart();
    void onStop();
private:
    Ui::MainWindow *ui;

    VideoMuxer *m_pVideoMuxer;
};
#endif // MAINWINDOW_H
