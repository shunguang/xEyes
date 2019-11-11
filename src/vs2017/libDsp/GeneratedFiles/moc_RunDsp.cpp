/****************************************************************************
** Meta object code from reading C++ file 'RunDsp.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../libDsp/RunDsp.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RunDsp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_xeyes__RunDsp_t {
    QByteArrayData data[5];
    char stringdata0[40];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_xeyes__RunDsp_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_xeyes__RunDsp_t qt_meta_stringdata_xeyes__RunDsp = {
    {
QT_MOC_LITERAL(0, 0, 13), // "xeyes::RunDsp"
QT_MOC_LITERAL(1, 14, 12), // "frmReady2Dsp"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 8), // "uint64_t"
QT_MOC_LITERAL(4, 37, 2) // "fn"

    },
    "xeyes::RunDsp\0frmReady2Dsp\0\0uint64_t\0"
    "fn"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_xeyes__RunDsp[] = {

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
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void xeyes::RunDsp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RunDsp *_t = static_cast<RunDsp *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->frmReady2Dsp((*reinterpret_cast< uint64_t(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RunDsp::*_t)(uint64_t );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RunDsp::frmReady2Dsp)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject xeyes::RunDsp::staticMetaObject = {
    { &ThreadQt::staticMetaObject, qt_meta_stringdata_xeyes__RunDsp.data,
      qt_meta_data_xeyes__RunDsp,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *xeyes::RunDsp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *xeyes::RunDsp::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_xeyes__RunDsp.stringdata0))
        return static_cast<void*>(const_cast< RunDsp*>(this));
    return ThreadQt::qt_metacast(_clname);
}

int xeyes::RunDsp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void xeyes::RunDsp::frmReady2Dsp(uint64_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
