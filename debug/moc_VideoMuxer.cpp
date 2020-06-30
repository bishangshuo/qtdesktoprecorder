/****************************************************************************
** Meta object code from reading C++ file 'VideoMuxer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../muxer/VideoMuxer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoMuxer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VideoMuxer_t {
    QByteArrayData data[7];
    char stringdata0[73];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoMuxer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoMuxer_t qt_meta_stringdata_VideoMuxer = {
    {
QT_MOC_LITERAL(0, 0, 10), // "VideoMuxer"
QT_MOC_LITERAL(1, 11, 17), // "slotCaptureScreen"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 6), // "pixmap"
QT_MOC_LITERAL(4, 37, 9), // "timestamp"
QT_MOC_LITERAL(5, 47, 15), // "slotRecordAudio"
QT_MOC_LITERAL(6, 63, 9) // "audioData"

    },
    "VideoMuxer\0slotCaptureScreen\0\0pixmap\0"
    "timestamp\0slotRecordAudio\0audioData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoMuxer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x0a /* Public */,
       5,    2,   29,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QPixmap, QMetaType::LongLong,    3,    4,
    QMetaType::Void, QMetaType::QByteArray, QMetaType::LongLong,    6,    4,

       0        // eod
};

void VideoMuxer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoMuxer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotCaptureScreen((*reinterpret_cast< const QPixmap(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 1: _t->slotRecordAudio((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VideoMuxer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_VideoMuxer.data,
    qt_meta_data_VideoMuxer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VideoMuxer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoMuxer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoMuxer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VideoMuxer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
