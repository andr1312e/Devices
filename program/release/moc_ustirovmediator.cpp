/****************************************************************************
** Meta object code from reading C++ file 'ustirovmediator.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../mediators/ustirovmediator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ustirovmediator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UstirovMediator_t {
    QByteArrayData data[26];
    char stringdata0[449];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UstirovMediator_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UstirovMediator_t qt_meta_stringdata_UstirovMediator = {
    {
QT_MOC_LITERAL(0, 0, 15), // "UstirovMediator"
QT_MOC_LITERAL(1, 16, 21), // "ToSendRarmNormalState"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 26), // "DevicesAdjustingKitMessage"
QT_MOC_LITERAL(4, 66, 9), // "upcbState"
QT_MOC_LITERAL(5, 76, 19), // "ToSendRarmBparState"
QT_MOC_LITERAL(6, 96, 30), // "DevicesBparAdjustingKitMessage"
QT_MOC_LITERAL(7, 127, 9), // "bparState"
QT_MOC_LITERAL(8, 137, 18), // "ToSendRarmGeoState"
QT_MOC_LITERAL(9, 156, 22), // "DevicesGeoStateMessage"
QT_MOC_LITERAL(10, 179, 10), // "geoMessage"
QT_MOC_LITERAL(11, 190, 13), // "ToSendPcbWork"
QT_MOC_LITERAL(12, 204, 23), // "OnSetUstirovNormalState"
QT_MOC_LITERAL(13, 228, 5), // "state"
QT_MOC_LITERAL(14, 234, 21), // "OnSetUstirovBparState"
QT_MOC_LITERAL(15, 256, 17), // "OnRequestGeoState"
QT_MOC_LITERAL(16, 274, 21), // "OnGetStateFromMessage"
QT_MOC_LITERAL(17, 296, 7), // "message"
QT_MOC_LITERAL(18, 304, 19), // "OnGetGeoFromMessage"
QT_MOC_LITERAL(19, 324, 16), // "OnRequestTimeOut"
QT_MOC_LITERAL(20, 341, 12), // "OnResetQueue"
QT_MOC_LITERAL(21, 354, 24), // "OnAllNormalDataCollected"
QT_MOC_LITERAL(22, 379, 22), // "OnAllBparDataCollected"
QT_MOC_LITERAL(23, 402, 17), // "OnAllGeoCollected"
QT_MOC_LITERAL(24, 420, 17), // "OnSendNextMessage"
QT_MOC_LITERAL(25, 438, 10) // "OnSendPing"

    },
    "UstirovMediator\0ToSendRarmNormalState\0"
    "\0DevicesAdjustingKitMessage\0upcbState\0"
    "ToSendRarmBparState\0DevicesBparAdjustingKitMessage\0"
    "bparState\0ToSendRarmGeoState\0"
    "DevicesGeoStateMessage\0geoMessage\0"
    "ToSendPcbWork\0OnSetUstirovNormalState\0"
    "state\0OnSetUstirovBparState\0"
    "OnRequestGeoState\0OnGetStateFromMessage\0"
    "message\0OnGetGeoFromMessage\0"
    "OnRequestTimeOut\0OnResetQueue\0"
    "OnAllNormalDataCollected\0"
    "OnAllBparDataCollected\0OnAllGeoCollected\0"
    "OnSendNextMessage\0OnSendPing"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UstirovMediator[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x06 /* Public */,
       5,    1,   97,    2, 0x06 /* Public */,
       8,    1,  100,    2, 0x06 /* Public */,
      11,    0,  103,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    1,  104,    2, 0x0a /* Public */,
      14,    1,  107,    2, 0x0a /* Public */,
      15,    0,  110,    2, 0x0a /* Public */,
      16,    1,  111,    2, 0x08 /* Private */,
      18,    1,  114,    2, 0x08 /* Private */,
      19,    0,  117,    2, 0x08 /* Private */,
      20,    0,  118,    2, 0x08 /* Private */,
      21,    0,  119,    2, 0x08 /* Private */,
      22,    0,  120,    2, 0x08 /* Private */,
      23,    0,  121,    2, 0x08 /* Private */,
      24,    0,  122,    2, 0x08 /* Private */,
      25,    0,  123,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,   13,
    QMetaType::Void, 0x80000000 | 6,   13,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,   17,
    QMetaType::Void, QMetaType::QByteArray,   17,
    QMetaType::Void,
    QMetaType::Void,
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
        auto *_t = static_cast<UstirovMediator *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ToSendRarmNormalState((*reinterpret_cast< const DevicesAdjustingKitMessage(*)>(_a[1]))); break;
        case 1: _t->ToSendRarmBparState((*reinterpret_cast< const DevicesBparAdjustingKitMessage(*)>(_a[1]))); break;
        case 2: _t->ToSendRarmGeoState((*reinterpret_cast< const DevicesGeoStateMessage(*)>(_a[1]))); break;
        case 3: _t->ToSendPcbWork(); break;
        case 4: _t->OnSetUstirovNormalState((*reinterpret_cast< const DevicesAdjustingKitMessage(*)>(_a[1]))); break;
        case 5: _t->OnSetUstirovBparState((*reinterpret_cast< const DevicesBparAdjustingKitMessage(*)>(_a[1]))); break;
        case 6: _t->OnRequestGeoState(); break;
        case 7: _t->OnGetStateFromMessage((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 8: _t->OnGetGeoFromMessage((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 9: _t->OnRequestTimeOut(); break;
        case 10: _t->OnResetQueue(); break;
        case 11: _t->OnAllNormalDataCollected(); break;
        case 12: _t->OnAllBparDataCollected(); break;
        case 13: _t->OnAllGeoCollected(); break;
        case 14: _t->OnSendNextMessage(); break;
        case 15: _t->OnSendPing(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (UstirovMediator::*)(const DevicesAdjustingKitMessage & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UstirovMediator::ToSendRarmNormalState)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (UstirovMediator::*)(const DevicesBparAdjustingKitMessage & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UstirovMediator::ToSendRarmBparState)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (UstirovMediator::*)(const DevicesGeoStateMessage & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UstirovMediator::ToSendRarmGeoState)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (UstirovMediator::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UstirovMediator::ToSendPcbWork)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject UstirovMediator::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_UstirovMediator.data,
    qt_meta_data_UstirovMediator,
    qt_static_metacall,
    nullptr,
    nullptr
} };


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
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void UstirovMediator::ToSendRarmNormalState(const DevicesAdjustingKitMessage & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void UstirovMediator::ToSendRarmBparState(const DevicesBparAdjustingKitMessage & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void UstirovMediator::ToSendRarmGeoState(const DevicesGeoStateMessage & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void UstirovMediator::ToSendPcbWork()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
