#include "CaptureScreen.h"

#include <assert.h>
#include <QCursor>
#include <QPainter>

#ifdef Q_OS_LINUX
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xfixes.h>
#else // Q_OS_WINDOWs
#include <Windows.h>
#include <wingdi.h>
#pragma comment (lib, "User32.lib")
#pragma comment (lib, "Gdi32.lib")
#include <QtWinExtras>
#endif

CaptureScreen::CaptureScreen(QObject *parent)
    : QThread(parent)
    , m_bRun(true)
{
    QRect screenRect = QApplication::desktop()->screenGeometry();
    m_nWidth = screenRect.width();
    m_nHeight = screenRect.height();
}

void CaptureScreen::StartCapture(){
    this->start();
}

void CaptureScreen::StopCapture(){
    m_bRun = false;
    this->quit();
    this->wait();
}

void CaptureScreen::run(){
    while (true) {
        if(!m_bRun)
            break;
        QPixmap pixmap = takeScreenShot(QRect(0, 0, m_nWidth, m_nHeight));
        qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
        emit sigCaptureScreen(pixmap.copy(0, 0, pixmap.width(), pixmap.height()), timestamp);
        QThread::msleep(1000/15);
    }
}

QPixmap CaptureScreen::takeScreenShot(const QRect& area) {
    QRect screen; /* interested display area */
    QImage qimage; /* result image */

#ifdef Q_OS_LINUX
    QPoint cursorPos;

    Display* display = XOpenDisplay(nullptr);
    Window root = DefaultRootWindow(display);

    XWindowAttributes gwa;
    XGetWindowAttributes(display, root, &gwa);

    const auto goodArea = QRect(0, 0, gwa.width, gwa.height).contains(area);
    if (!goodArea) {
        screen = QRect(0, 0, gwa.width, gwa.height);
        cursorPos = QCursor::pos();
    } else {
        screen = area;
        cursorPos = QCursor::pos() - screen.topLeft();
    }

    XImage* image = XGetImage(display, root, screen.x(), screen.y(), screen.width(), screen.height(), AllPlanes, ZPixmap);
    assert(nullptr != image);

    qimage = qimageFromXImage(image);

    /* draw mouse cursor into QImage
     * https://msnkambule.wordpress.com/2010/04/09/capturing-a-screenshot-showing-mouse-cursor-in-kde/
     * https://github.com/rprichard/x11-canvas-screencast/blob/master/CursorX11.cpp#L31
     * */
    {
        XFixesCursorImage* cursor = XFixesGetCursorImage(display);
        cursorPos -= QPoint(cursor->xhot, cursor->yhot);
        std::vector<uint32_t> pixels(cursor->width * cursor->height);
        for (size_t i = 0; i < pixels.size(); ++i)
            pixels[i] = cursor->pixels[i];
        QImage cursorImage((uchar*)(pixels.data()), cursor->width, cursor->height, QImage::Format_ARGB32_Premultiplied);
        QPainter painter(&qimage);
        painter.drawImage(cursorPos, cursorImage);
        XFree(cursor);
    }

    XDestroyImage(image);
    XDestroyWindow(display, root);
    XCloseDisplay(display);

#elif defined(Q_OS_WINDOWS)
    HWND hwnd = GetDesktopWindow();
    HDC hdc = GetWindowDC(hwnd);
    HDC hdcMem = CreateCompatibleDC(hdc);

    RECT rect = { 0, 0, GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES) };
    const auto goodArea = QRect(rect.left, rect.top, rect.right, rect.bottom).contains(area);
    if (!goodArea) {
        screen = QRect(rect.left, rect.top, rect.right, rect.bottom);
    } else {
        screen = area;
    }

    HBITMAP hbitmap(nullptr);
    hbitmap = CreateCompatibleBitmap(hdc, screen.width(), screen.height());
    SelectObject(hdcMem, hbitmap);
    BitBlt(hdcMem, 0, 0, screen.width(), screen.height(), hdc, screen.x(), screen.y(), SRCCOPY);

    /* draw mouse cursor into DC
     * https://stackoverflow.com/a/48925443/5446734
     * */
    CURSORINFO cursor = { sizeof(cursor) };
    if (GetCursorInfo(&cursor) && cursor.flags == CURSOR_SHOWING) {
        RECT rect;
        GetWindowRect(hwnd, &rect);
        ICONINFO info = { sizeof(info) };
        GetIconInfo(cursor.hCursor, &info);
        const int x = (cursor.ptScreenPos.x - rect.left - rect.left - info.xHotspot) - screen.left();
        const int y = (cursor.ptScreenPos.y - rect.left - rect.left - info.yHotspot) - screen.top();
        BITMAP bmpCursor = { 0 };
        GetObject(info.hbmColor, sizeof(bmpCursor), &bmpCursor);
        DrawIconEx(hdcMem, x, y, cursor.hCursor, bmpCursor.bmWidth, bmpCursor.bmHeight,
                   0, nullptr, DI_NORMAL);
    }

    qimage = QtWin::imageFromHBITMAP(hdc, hbitmap, screen.width(), screen.height());
#endif // Q_OS_LINUX

    return QPixmap::fromImage(qimage);
}
