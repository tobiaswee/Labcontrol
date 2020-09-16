/****************************************************************************
** Meta object code from reading C++ file 'receipts_handler.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/Lib/receipts_handler.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'receipts_handler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_lc__ReceiptsHandler_t {
    QByteArrayData data[9];
    char stringdata0[136];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_lc__ReceiptsHandler_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_lc__ReceiptsHandler_t qt_meta_stringdata_lc__ReceiptsHandler = {
    {
QT_MOC_LITERAL(0, 0, 19), // "lc::ReceiptsHandler"
QT_MOC_LITERAL(1, 20, 16), // "PrintingFinished"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 29), // "DeleteReceiptsPrinterInstance"
QT_MOC_LITERAL(4, 68, 17), // "DisplayMessageBox"
QT_MOC_LITERAL(5, 86, 8), // "QString*"
QT_MOC_LITERAL(6, 95, 15), // "argErrorMessage"
QT_MOC_LITERAL(7, 111, 10), // "argHeading"
QT_MOC_LITERAL(8, 122, 13) // "PrintReceipts"

    },
    "lc::ReceiptsHandler\0PrintingFinished\0"
    "\0DeleteReceiptsPrinterInstance\0"
    "DisplayMessageBox\0QString*\0argErrorMessage\0"
    "argHeading\0PrintReceipts"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_lc__ReceiptsHandler[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   35,    2, 0x08 /* Private */,
       4,    2,   36,    2, 0x08 /* Private */,
       8,    0,   41,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 5,    6,    7,
    QMetaType::Void,

       0        // eod
};

void lc::ReceiptsHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ReceiptsHandler *_t = static_cast<ReceiptsHandler *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->PrintingFinished(); break;
        case 1: _t->DeleteReceiptsPrinterInstance(); break;
        case 2: _t->DisplayMessageBox((*reinterpret_cast< QString*(*)>(_a[1])),(*reinterpret_cast< QString*(*)>(_a[2]))); break;
        case 3: _t->PrintReceipts(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ReceiptsHandler::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ReceiptsHandler::PrintingFinished)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject lc::ReceiptsHandler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_lc__ReceiptsHandler.data,
      qt_meta_data_lc__ReceiptsHandler,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *lc::ReceiptsHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *lc::ReceiptsHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_lc__ReceiptsHandler.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int lc::ReceiptsHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void lc::ReceiptsHandler::PrintingFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
