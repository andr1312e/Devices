/****************************************************************************
** Meta object code from reading C++ file 'ustirovsocket.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../sockets/ustirovsocket.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ustirovsocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UstirovSocket_t {
    QByteArrayData data[13];
    char stringdata0[210];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UstirovSocket_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UstirovSocket_t qt_meta_stringdata_UstirovSocket = {
    {
QT_MOC_LITERAL(0, 0, 13), // "UstirovSocket"
QT_MOC_LITERAL(1, 14, 16), // "ToRequestTimeOut"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 17), // "ToWantNextMessage"
QT_MOC_LITERAL(4, 50, 12), // "ToResetQueue"
QT_MOC_LITERAL(5, 63, 21), // "ToGetStateFromMessage"
QT_MOC_LITERAL(6, 85, 7), // "message"
QT_MOC_LITERAL(7, 93, 19), // "ToGetGeoFromMessage"
QT_MOC_LITERAL(8, 113, 11), // "OnReadyRead"
QT_MOC_LITERAL(9, 125, 15), // "OnHostConnected"
QT_MOC_LITERAL(10, 141, 22), // "OnDisconnectedFromHost"
QT_MOC_LITERAL(11, 164, 15), // "OnErrorOccurred"
QT_MOC_LITERAL(12, 180, 29) // "OnCheckConnectionTimerTimeOut"

    },
    "UstirovSocket\0ToRequestTimeOut\0\0"
    "ToWantNextMessage\0ToResetQueue\0"
    "ToGetStateFromMessage\0message\0"
    "ToGetGeoFromMessage\0OnReadyRead\0"
    "OnHostConnected\0OnDisconnectedFromHost\0"
    "OnErrorOccurred\0OnCheckConnectionTimerTimeOut"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UstirovSocket[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    0,   65,    2, 0x06 /* Public */,
       4,    0,   66,    2, 0x06 /* Public */,
       5,    1,   67,    2, 0x06 /* Public */,
       7,    1,   70,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   73,    2, 0x08 /* Private */,
       9,    0,   74,    2, 0x08 /* Private */,
      10,    0,   75,    2, 0x08 /* Private */,
      11,    0,   76,    2, 0x08 /* Private */,
      12,    0,   77,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    6,
    QMetaType::Void, QMetaType::QByteArray,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void UstirovSocket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UstirovSocket *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ToRequestTimeOut(); break;
        case 1: _t->ToWantNextMessage(); break;
        case 2: _t->ToResetQueue(); break;
        case 3: _t->ToGetStateFromMessage((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 4: _t->ToGetGeoFromMessage((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 5: _t->OnReadyRead(); break;
        case 6: _t->OnHostConnected(); break;
        case 7: _t->OnDisconnectedFromHost(); break;
        case 8: _t->OnErrorOccurred(); break;
        case 9: _t->OnCheckConnectionTimerTimeOut(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (UstirovSocket::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UstirovSocket::ToRequestTimeOut)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (UstirovSocket::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UstirovSocket::ToWantNextMessage)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (UstirovSocket::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UstirovSocket::ToResetQueue)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (UstirovSocket::*)(const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UstirovSocket::ToGetStateFromMessage)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (UstirovSocket::*)(const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UstirovSocket::ToGetGeoFromMessage)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject UstirovSocket::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_UstirovSocket.data,
    qt_meta_data_UstirovSocket,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *UstirovSocket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UstirovSocket::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UstirovSocket.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int UstirovSocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void UstirovSocket::ToRequestTimeOut()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void UstirovSocket::ToWantNextMessage()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void UstirovSocket::ToResetQueue()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void UstirovSocket::ToGetStateFromMessage(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void UstirovSocket::ToGetGeoFromMessage(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
