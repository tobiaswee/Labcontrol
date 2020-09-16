/****************************************************************************
** Meta object code from reading C++ file 'manualprintingsetup.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/manualprintingsetup.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'manualprintingsetup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_lc__ManualPrintingSetup_t {
    QByteArrayData data[14];
    char stringdata0[293];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_lc__ManualPrintingSetup_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_lc__ManualPrintingSetup_t qt_meta_stringdata_lc__ManualPrintingSetup = {
    {
QT_MOC_LITERAL(0, 0, 23), // "lc::ManualPrintingSetup"
QT_MOC_LITERAL(1, 24, 22), // "RequestReceiptsHandler"
QT_MOC_LITERAL(2, 47, 0), // ""
QT_MOC_LITERAL(3, 48, 22), // "argzTreeDataTargetPath"
QT_MOC_LITERAL(4, 71, 26), // "argReceiptsForLocalClients"
QT_MOC_LITERAL(5, 98, 31), // "argAnonymousReceiptsPlaceholder"
QT_MOC_LITERAL(6, 130, 18), // "argLatexHeaderName"
QT_MOC_LITERAL(7, 149, 13), // "argDateString"
QT_MOC_LITERAL(8, 163, 29), // "on_CBReceiptsHeader_activated"
QT_MOC_LITERAL(9, 193, 8), // "argIndex"
QT_MOC_LITERAL(10, 202, 36), // "on_ChBPrintAnonymousReceipts_..."
QT_MOC_LITERAL(11, 239, 10), // "argChecked"
QT_MOC_LITERAL(12, 250, 18), // "on_PBPrint_clicked"
QT_MOC_LITERAL(13, 269, 23) // "on_PBSelectFile_clicked"

    },
    "lc::ManualPrintingSetup\0RequestReceiptsHandler\0"
    "\0argzTreeDataTargetPath\0"
    "argReceiptsForLocalClients\0"
    "argAnonymousReceiptsPlaceholder\0"
    "argLatexHeaderName\0argDateString\0"
    "on_CBReceiptsHeader_activated\0argIndex\0"
    "on_ChBPrintAnonymousReceipts_clicked\0"
    "argChecked\0on_PBPrint_clicked\0"
    "on_PBSelectFile_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_lc__ManualPrintingSetup[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    5,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   50,    2, 0x08 /* Private */,
      10,    1,   53,    2, 0x08 /* Private */,
      12,    0,   56,    2, 0x08 /* Private */,
      13,    0,   57,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Bool, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,    4,    5,    6,    7,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void lc::ManualPrintingSetup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ManualPrintingSetup *_t = static_cast<ManualPrintingSetup *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->RequestReceiptsHandler((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5]))); break;
        case 1: _t->on_CBReceiptsHeader_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_ChBPrintAnonymousReceipts_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->on_PBPrint_clicked(); break;
        case 4: _t->on_PBSelectFile_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ManualPrintingSetup::*)(QString , bool , QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ManualPrintingSetup::RequestReceiptsHandler)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject lc::ManualPrintingSetup::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_lc__ManualPrintingSetup.data,
      qt_meta_data_lc__ManualPrintingSetup,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *lc::ManualPrintingSetup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *lc::ManualPrintingSetup::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_lc__ManualPrintingSetup.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int lc::ManualPrintingSetup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void lc::ManualPrintingSetup::RequestReceiptsHandler(QString _t1, bool _t2, QString _t3, QString _t4, QString _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
