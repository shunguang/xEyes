/****************************************************************************
** Meta object code from reading C++ file 'DspThread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../libDsp/DspThread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DspThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_xeyes__DspThread_t {
    QByteArrayData data[3];
    char stringdata0[32];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_xeyes__DspThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_xeyes__DspThread_t qt_meta_stringdata_xeyes__DspThread = {
    {
QT_MOC_LITERAL(0, 0, 16), // "xeyes::DspThread"
QT_MOC_LITERAL(1, 17, 13), // "sigReady2Disp"
QT_MOC_LITERAL(2, 31, 0) // ""

    },
    "xeyes::DspThread\0sigReady2Disp\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_xeyes__DspThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void xeyes::DspThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DspThread *_t = static_cast<DspThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigReady2Disp(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DspThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DspThread::sigReady2Disp)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject xeyes::DspThread::staticMetaObject = {
    { &ThreadQt::staticMetaObject, qt_meta_stringdata_xeyes__DspThread.data,
      qt_meta_data_xeyes__DspThread,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *xeyes::DspThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *xeyes::DspThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_xeyes__DspThread.stringdata0))
        return static_cast<void*>(const_cast< DspThread*>(this));
    return ThreadQt::qt_metacast(_clname);
}

int xeyes::DspThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ThreadQt::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void xeyes::DspThread::sigReady2Disp()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
