/****************************************************************************
** Meta object code from reading C++ file 'ustirovsocket.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../sockets/ustirovsocket.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ustirovsocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UstirovSocket_t {
    const uint offsetsAndSize[24];
    char stringdata0[190];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_UstirovSocket_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_UstirovSocket_t qt_meta_stringdata_UstirovSocket = {
    {
QT_MOC_LITERAL(0, 13), // "UstirovSocket"
QT_MOC_LITERAL(14, 16), // "ToRequestTimeOut"
QT_MOC_LITERAL(31, 0), // ""
QT_MOC_LITERAL(32, 17), // "ToWantNextMessage"
QT_MOC_LITERAL(50, 12), // "ToResetQueue"
QT_MOC_LITERAL(63, 21), // "ToGetStateFromMessage"
QT_MOC_LITERAL(85, 7), // "message"
QT_MOC_LITERAL(93, 11), // "OnReadyRead"
QT_MOC_LITERAL(105, 15), // "OnHostConnected"
QT_MOC_LITERAL(121, 22), // "OnDisconnectedFromHost"
QT_MOC_LITERAL(144, 15), // "OnErrorOccurred"
QT_MOC_LITERAL(160, 29) // "OnCheckConnectionTimerTimeOut"

    },
    "UstirovSocket\0ToRequestTimeOut\0\0"
    "ToWantNextMessage\0ToResetQueue\0"
    "ToGetStateFromMessage\0message\0OnReadyRead\0"
    "OnHostConnected\0OnDisconnectedFromHost\0"
    "OnErrorOccurred\0OnCheckConnectionTimerTimeOut"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UstirovSocket[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   68,    2, 0x06,    1 /* Public */,
       3,    0,   69,    2, 0x06,    2 /* Public */,
       4,    0,   70,    2, 0x06,    3 /* Public */,
       5,    1,   71,    2, 0x06,    4 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       7,    0,   74,    2, 0x08,    6 /* Private */,
       8,    0,   75,    2, 0x08,    7 /* Private */,
       9,    0,   76,    2, 0x08,    8 /* Private */,
      10,    0,   77,    2, 0x08,    9 /* Private */,
      11,    0,   78,    2, 0x08,   10 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
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
        (void)_t;
        switch (_id) {
        case 0: _t->ToRequestTimeOut(); break;
        case 1: _t->ToWantNextMessage(); break;
        case 2: _t->ToResetQueue(); break;
        case 3: _t->ToGetStateFromMessage((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 4: _t->OnReadyRead(); break;
        case 5: _t->OnHostConnected(); break;
        case 6: _t->OnDisconnectedFromHost(); break;
        case 7: _t->OnErrorOccurred(); break;
        case 8: _t->OnCheckConnectionTimerTimeOut(); break;
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
    }
}

const QMetaObject UstirovSocket::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_UstirovSocket.offsetsAndSize,
    qt_meta_data_UstirovSocket,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_UstirovSocket_t
, QtPrivate::TypeAndForceComplete<UstirovSocket, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QByteArray &, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
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
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
