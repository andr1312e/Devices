/****************************************************************************
** Meta object code from reading C++ file 'geosocket.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../sockets/geosocket.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'geosocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GeoSocket_t {
    const uint offsetsAndSize[14];
    char stringdata0[79];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_GeoSocket_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_GeoSocket_t qt_meta_stringdata_GeoSocket = {
    {
QT_MOC_LITERAL(0, 9), // "GeoSocket"
QT_MOC_LITERAL(10, 18), // "ToSendRarmGeoState"
QT_MOC_LITERAL(29, 0), // ""
QT_MOC_LITERAL(30, 22), // "DevicesGeoStateMessage"
QT_MOC_LITERAL(53, 5), // "state"
QT_MOC_LITERAL(59, 11), // "OnReadyRead"
QT_MOC_LITERAL(71, 7) // "OnClose"

    },
    "GeoSocket\0ToSendRarmGeoState\0\0"
    "DevicesGeoStateMessage\0state\0OnReadyRead\0"
    "OnClose"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GeoSocket[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   32,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   35,    2, 0x08,    3 /* Private */,
       6,    0,   36,    2, 0x08,    4 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void GeoSocket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GeoSocket *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->ToSendRarmGeoState((*reinterpret_cast< std::add_pointer_t<DevicesGeoStateMessage>>(_a[1]))); break;
        case 1: _t->OnReadyRead(); break;
        case 2: _t->OnClose(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GeoSocket::*)(const DevicesGeoStateMessage & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GeoSocket::ToSendRarmGeoState)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject GeoSocket::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_GeoSocket.offsetsAndSize,
    qt_meta_data_GeoSocket,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_GeoSocket_t
, QtPrivate::TypeAndForceComplete<GeoSocket, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const DevicesGeoStateMessage &, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *GeoSocket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GeoSocket::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GeoSocket.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int GeoSocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void GeoSocket::ToSendRarmGeoState(const DevicesGeoStateMessage & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
