/****************************************************************************
** Meta object code from reading C++ file 'clientpinger.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/Lib/clientpinger.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'clientpinger.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_lc__ClientPinger_t {
    QByteArrayData data[7];
    char stringdata0[76];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_lc__ClientPinger_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_lc__ClientPinger_t qt_meta_stringdata_lc__ClientPinger = {
    {
QT_MOC_LITERAL(0, 0, 16), // "lc::ClientPinger"
QT_MOC_LITERAL(1, 17, 12), // "PingFinished"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 7), // "state_t"
QT_MOC_LITERAL(4, 39, 5), // "state"
QT_MOC_LITERAL(5, 45, 6), // "doPing"
QT_MOC_LITERAL(6, 52, 23) // "setStateToZLEAF_RUNNING"

    },
    "lc::ClientPinger\0PingFinished\0\0state_t\0"
    "state\0doPing\0setStateToZLEAF_RUNNING"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_lc__ClientPinger[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   32,    2, 0x0a /* Public */,
       6,    0,   33,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void lc::ClientPinger::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ClientPinger *_t = static_cast<ClientPinger *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->PingFinished((*reinterpret_cast< state_t(*)>(_a[1]))); break;
        case 1: _t->doPing(); break;
        case 2: _t->setStateToZLEAF_RUNNING(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< state_t >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ClientPinger::*)(state_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientPinger::PingFinished)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject lc::ClientPinger::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_lc__ClientPinger.data,
      qt_meta_data_lc__ClientPinger,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *lc::ClientPinger::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *lc::ClientPinger::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_lc__ClientPinger.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int lc::ClientPinger::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void lc::ClientPinger::PingFinished(state_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
