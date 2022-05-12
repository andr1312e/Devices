/****************************************************************************
** Meta object code from reading C++ file 'rarmsocket.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../sockets/rarmsocket.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rarmsocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RarmSocket_t {
    const uint offsetsAndSize[46];
    char stringdata0[395];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_RarmSocket_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_RarmSocket_t qt_meta_stringdata_RarmSocket = {
    {
QT_MOC_LITERAL(0, 10), // "RarmSocket"
QT_MOC_LITERAL(11, 17), // "ToSetUstirovState"
QT_MOC_LITERAL(29, 0), // ""
QT_MOC_LITERAL(30, 26), // "DevicesAdjustingKitMessage"
QT_MOC_LITERAL(57, 5), // "state"
QT_MOC_LITERAL(63, 17), // "ToGetUstirovState"
QT_MOC_LITERAL(81, 17), // "OnConnectedToRarm"
QT_MOC_LITERAL(99, 11), // "OnReadyRead"
QT_MOC_LITERAL(111, 17), // "OnConnectionError"
QT_MOC_LITERAL(129, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(158, 11), // "socketError"
QT_MOC_LITERAL(170, 14), // "OnDisconnected"
QT_MOC_LITERAL(185, 17), // "OnCheckConnection"
QT_MOC_LITERAL(203, 24), // "OnSendRarmMoxaWorksState"
QT_MOC_LITERAL(228, 24), // "DevicesMoxaStateMessage&"
QT_MOC_LITERAL(253, 9), // "moxaState"
QT_MOC_LITERAL(263, 20), // "OnSendRarmMeteoState"
QT_MOC_LITERAL(284, 25), // "DevicesMeteoKitGetMessage"
QT_MOC_LITERAL(310, 10), // "meteoState"
QT_MOC_LITERAL(321, 19), // "OnSendRarmUPCBState"
QT_MOC_LITERAL(341, 9), // "upcbState"
QT_MOC_LITERAL(351, 20), // "OnSendRarmGeoMessage"
QT_MOC_LITERAL(372, 22) // "DevicesGeoStateMessage"

    },
    "RarmSocket\0ToSetUstirovState\0\0"
    "DevicesAdjustingKitMessage\0state\0"
    "ToGetUstirovState\0OnConnectedToRarm\0"
    "OnReadyRead\0OnConnectionError\0"
    "QAbstractSocket::SocketError\0socketError\0"
    "OnDisconnected\0OnCheckConnection\0"
    "OnSendRarmMoxaWorksState\0"
    "DevicesMoxaStateMessage&\0moxaState\0"
    "OnSendRarmMeteoState\0DevicesMeteoKitGetMessage\0"
    "meteoState\0OnSendRarmUPCBState\0upcbState\0"
    "OnSendRarmGeoMessage\0DevicesGeoStateMessage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RarmSocket[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   80,    2, 0x06,    1 /* Public */,
       5,    0,   83,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       6,    0,   84,    2, 0x08,    4 /* Private */,
       7,    0,   85,    2, 0x08,    5 /* Private */,
       8,    1,   86,    2, 0x08,    6 /* Private */,
      11,    0,   89,    2, 0x08,    8 /* Private */,
      12,    0,   90,    2, 0x08,    9 /* Private */,
      13,    1,   91,    2, 0x0a,   10 /* Public */,
      16,    1,   94,    2, 0x0a,   12 /* Public */,
      19,    1,   97,    2, 0x0a,   14 /* Public */,
      21,    1,  100,    2, 0x0a,   16 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void, 0x80000000 | 3,   20,
    QMetaType::Void, 0x80000000 | 22,    4,

       0        // eod
};

void RarmSocket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RarmSocket *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->ToSetUstirovState((*reinterpret_cast< std::add_pointer_t<DevicesAdjustingKitMessage>>(_a[1]))); break;
        case 1: _t->ToGetUstirovState(); break;
        case 2: _t->OnConnectedToRarm(); break;
        case 3: _t->OnReadyRead(); break;
        case 4: _t->OnConnectionError((*reinterpret_cast< std::add_pointer_t<QAbstractSocket::SocketError>>(_a[1]))); break;
        case 5: _t->OnDisconnected(); break;
        case 6: _t->OnCheckConnection(); break;
        case 7: _t->OnSendRarmMoxaWorksState((*reinterpret_cast< std::add_pointer_t<DevicesMoxaStateMessage&>>(_a[1]))); break;
        case 8: _t->OnSendRarmMeteoState((*reinterpret_cast< std::add_pointer_t<DevicesMeteoKitGetMessage>>(_a[1]))); break;
        case 9: _t->OnSendRarmUPCBState((*reinterpret_cast< std::add_pointer_t<DevicesAdjustingKitMessage>>(_a[1]))); break;
        case 10: _t->OnSendRarmGeoMessage((*reinterpret_cast< std::add_pointer_t<DevicesGeoStateMessage>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RarmSocket::*)(const DevicesAdjustingKitMessage & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RarmSocket::ToSetUstirovState)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RarmSocket::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RarmSocket::ToGetUstirovState)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject RarmSocket::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_RarmSocket.offsetsAndSize,
    qt_meta_data_RarmSocket,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_RarmSocket_t
, QtPrivate::TypeAndForceComplete<RarmSocket, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const DevicesAdjustingKitMessage &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QAbstractSocket::SocketError, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<DevicesMoxaStateMessage &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const DevicesMeteoKitGetMessage &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const DevicesAdjustingKitMessage &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const DevicesGeoStateMessage &, std::false_type>


>,
    nullptr
} };


const QMetaObject *RarmSocket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RarmSocket::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RarmSocket.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int RarmSocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void RarmSocket::ToSetUstirovState(const DevicesAdjustingKitMessage & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RarmSocket::ToGetUstirovState()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
