/****************************************************************************
** Meta object code from reading C++ file 'meteosocket.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../sockets/meteosocket.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'meteosocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MeteoServer_t {
    const uint offsetsAndSize[18];
    char stringdata0[132];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_MeteoServer_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_MeteoServer_t qt_meta_stringdata_MeteoServer = {
    {
QT_MOC_LITERAL(0, 11), // "MeteoServer"
QT_MOC_LITERAL(12, 16), // "ToRequestTimeOut"
QT_MOC_LITERAL(29, 0), // ""
QT_MOC_LITERAL(30, 21), // "ToGetStateFromMessage"
QT_MOC_LITERAL(52, 7), // "message"
QT_MOC_LITERAL(60, 11), // "OnReadyRead"
QT_MOC_LITERAL(72, 20), // "OnNewSocketConnected"
QT_MOC_LITERAL(93, 22), // "OnDisconnectedFromHost"
QT_MOC_LITERAL(116, 15) // "OnErrorOccurred"

    },
    "MeteoServer\0ToRequestTimeOut\0\0"
    "ToGetStateFromMessage\0message\0OnReadyRead\0"
    "OnNewSocketConnected\0OnDisconnectedFromHost\0"
    "OnErrorOccurred"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MeteoServer[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   50,    2, 0x06,    1 /* Public */,
       3,    1,   51,    2, 0x06,    2 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   54,    2, 0x08,    4 /* Private */,
       6,    0,   55,    2, 0x08,    5 /* Private */,
       7,    0,   56,    2, 0x08,    6 /* Private */,
       8,    0,   57,    2, 0x08,    7 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MeteoServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MeteoServer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->ToRequestTimeOut(); break;
        case 1: _t->ToGetStateFromMessage((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 2: _t->OnReadyRead(); break;
        case 3: _t->OnNewSocketConnected(); break;
        case 4: _t->OnDisconnectedFromHost(); break;
        case 5: _t->OnErrorOccurred(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MeteoServer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MeteoServer::ToRequestTimeOut)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MeteoServer::*)(const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MeteoServer::ToGetStateFromMessage)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject MeteoServer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_MeteoServer.offsetsAndSize,
    qt_meta_data_MeteoServer,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MeteoServer_t
, QtPrivate::TypeAndForceComplete<MeteoServer, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QByteArray &, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *MeteoServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MeteoServer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MeteoServer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MeteoServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void MeteoServer::ToRequestTimeOut()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MeteoServer::ToGetStateFromMessage(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
