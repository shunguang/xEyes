/****************************************************************************
** Meta object code from reading C++ file 'RunXEyes.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/libRun/RunXEyes.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RunXEyes.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_xeyes__RunXEyes_t {
    QByteArrayData data[7];
    char stringdata0[105];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_xeyes__RunXEyes_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_xeyes__RunXEyes_t qt_meta_stringdata_xeyes__RunXEyes = {
    {
QT_MOC_LITERAL(0, 0, 15), // "xeyes::RunXEyes"
QT_MOC_LITERAL(1, 16, 23), // "on_actionExit_triggered"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 15), // "respns_dispImg0"
QT_MOC_LITERAL(4, 57, 15), // "respns_dispImg1"
QT_MOC_LITERAL(5, 73, 15), // "respns_dispImg2"
QT_MOC_LITERAL(6, 89, 15) // "respns_dispImg3"

    },
    "xeyes::RunXEyes\0on_actionExit_triggered\0"
    "\0respns_dispImg0\0respns_dispImg1\0"
    "respns_dispImg2\0respns_dispImg3"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_xeyes__RunXEyes[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x09 /* Protected */,
       3,    0,   40,    2, 0x09 /* Protected */,
       4,    0,   41,    2, 0x09 /* Protected */,
       5,    0,   42,    2, 0x09 /* Protected */,
       6,    0,   43,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void xeyes::RunXEyes::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RunXEyes *_t = static_cast<RunXEyes *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_actionExit_triggered(); break;
        case 1: _t->respns_dispImg0(); break;
        case 2: _t->respns_dispImg1(); break;
        case 3: _t->respns_dispImg2(); break;
        case 4: _t->respns_dispImg3(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject xeyes::RunXEyes::staticMetaObject = {
    { &RunGui::staticMetaObject, qt_meta_stringdata_xeyes__RunXEyes.data,
      qt_meta_data_xeyes__RunXEyes,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *xeyes::RunXEyes::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *xeyes::RunXEyes::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_xeyes__RunXEyes.stringdata0))
        return static_cast<void*>(this);
    return RunGui::qt_metacast(_clname);
}

int xeyes::RunXEyes::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RunGui::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
