/****************************************************************************
** Meta object code from reading C++ file 'meteomediator.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../mediators/meteomediator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'meteomediator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MeteoMediator_t {
    const uint offsetsAndSize[20];
    char stringdata0[156];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_MeteoMediator_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_MeteoMediator_t qt_meta_stringdata_MeteoMediator = {
    {
QT_MOC_LITERAL(0, 13), // "MeteoMediator"
QT_MOC_LITERAL(14, 20), // "ToSendRarmMeteoState"
QT_MOC_LITERAL(35, 0), // ""
QT_MOC_LITERAL(36, 25), // "DevicesMeteoKitGetMessage"
QT_MOC_LITERAL(62, 10), // "meteoState"
QT_MOC_LITERAL(73, 16), // "OnMakeNewRequest"
QT_MOC_LITERAL(90, 21), // "OnGetStateFromMessage"
QT_MOC_LITERAL(112, 7), // "message"
QT_MOC_LITERAL(120, 16), // "OnRequestTimeOut"
QT_MOC_LITERAL(137, 18) // "OnAllDataCollected"

    },
    "MeteoMediator\0ToSendRarmMeteoState\0\0"
    "DevicesMeteoKitGetMessage\0meteoState\0"
    "OnMakeNewRequest\0OnGetStateFromMessage\0"
    "message\0OnRequestTimeOut\0OnAllDataCollected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MeteoMediator[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   44,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   47,    2, 0x08,    3 /* Private */,
       6,    1,   48,    2, 0x08,    4 /* Private */,
       8,    0,   51,    2, 0x08,    6 /* Private */,
       9,    0,   52,    2, 0x08,    7 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    7,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MeteoMediator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MeteoMediator *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->ToSendRarmMeteoState((*reinterpret_cast< std::add_pointer_t<DevicesMeteoKitGetMessage>>(_a[1]))); break;
        case 1: _t->OnMakeNewRequest(); break;
        case 2: _t->OnGetStateFromMessage((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 3: _t->OnRequestTimeOut(); break;
        case 4: _t->OnAllDataCollected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MeteoMediator::*)(const DevicesMeteoKitGetMessage & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MeteoMediator::ToSendRarmMeteoState)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MeteoMediator::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_MeteoMediator.offsetsAndSize,
    qt_meta_data_MeteoMediator,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MeteoMediator_t
, QtPrivate::TypeAndForceComplete<MeteoMediator, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const DevicesMeteoKitGetMessage &, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QByteArray &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *MeteoMediator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MeteoMediator::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MeteoMediator.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MeteoMediator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void MeteoMediator::ToSendRarmMeteoState(const DevicesMeteoKitGetMessage & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
