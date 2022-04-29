/****************************************************************************
** Meta object code from reading C++ file 'ustirovmediator.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mediators/ustirovmediator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ustirovmediator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UstirovMediator_t {
    QByteArrayData data[15];
    char stringdata0[242];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UstirovMediator_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UstirovMediator_t qt_meta_stringdata_UstirovMediator = {
    {
QT_MOC_LITERAL(0, 0, 15), // "UstirovMediator"
QT_MOC_LITERAL(1, 16, 22), // "ToSendRarmUstirovState"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 26), // "DevicesAdjustingKitMessage"
QT_MOC_LITERAL(4, 67, 9), // "upcbState"
QT_MOC_LITERAL(5, 77, 18), // "OnSetDataToUstirov"
QT_MOC_LITERAL(6, 96, 5), // "state"
QT_MOC_LITERAL(7, 102, 20), // "OnGetDataFromUstirov"
QT_MOC_LITERAL(8, 123, 21), // "OnGetStateFromMessage"
QT_MOC_LITERAL(9, 145, 7), // "message"
QT_MOC_LITERAL(10, 153, 16), // "OnRequestTimeOut"
QT_MOC_LITERAL(11, 170, 19), // "OnWantedNextMessage"
QT_MOC_LITERAL(12, 190, 12), // "OnResetQueue"
QT_MOC_LITERAL(13, 203, 18), // "OnAllDataCollected"
QT_MOC_LITERAL(14, 222, 19) // "OnTimeToSendMessage"

    },
    "UstirovMediator\0ToSendRarmUstirovState\0"
    "\0DevicesAdjustingKitMessage\0upcbState\0"
    "OnSetDataToUstirov\0state\0OnGetDataFromUstirov\0"
    "OnGetStateFromMessage\0message\0"
    "OnRequestTimeOut\0OnWantedNextMessage\0"
    "OnResetQueue\0OnAllDataCollected\0"
    "OnTimeToSendMessage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UstirovMediator[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   62,    2, 0x0a /* Public */,
       7,    0,   65,    2, 0x0a /* Public */,
       8,    1,   66,    2, 0x08 /* Private */,
      10,    0,   69,    2, 0x08 /* Private */,
      11,    0,   70,    2, 0x08 /* Private */,
      12,    0,   71,    2, 0x08 /* Private */,
      13,    0,   72,    2, 0x08 /* Private */,
      14,    0,   73,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void UstirovMediator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UstirovMediator *_t = static_cast<UstirovMediator *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ToSendRarmUstirovState((*reinterpret_cast< const DevicesAdjustingKitMessage(*)>(_a[1]))); break;
        case 1: _t->OnSetDataToUstirov((*reinterpret_cast< const DevicesAdjustingKitMessage(*)>(_a[1]))); break;
        case 2: _t->OnGetDataFromUstirov(); break;
        case 3: _t->OnGetStateFromMessage((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 4: _t->OnRequestTimeOut(); break;
        case 5: _t->OnWantedNextMessage(); break;
        case 6: _t->OnResetQueue(); break;
        case 7: _t->OnAllDataCollected(); break;
        case 8: _t->OnTimeToSendMessage(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (UstirovMediator::*)(const DevicesAdjustingKitMessage & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UstirovMediator::ToSendRarmUstirovState)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject UstirovMediator::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_UstirovMediator.data,
      qt_meta_data_UstirovMediator,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *UstirovMediator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UstirovMediator::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UstirovMediator.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int UstirovMediator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void UstirovMediator::ToSendRarmUstirovState(const DevicesAdjustingKitMessage & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
