/****************************************************************************
** Meta object code from reading C++ file 'ustirovmediator.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../mediators/ustirovmediator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ustirovmediator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UstirovMediator_t {
    const uint offsetsAndSize[28];
    char stringdata0[216];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_UstirovMediator_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_UstirovMediator_t qt_meta_stringdata_UstirovMediator = {
    {
QT_MOC_LITERAL(0, 15), // "UstirovMediator"
QT_MOC_LITERAL(16, 22), // "ToSendRarmUstirovState"
QT_MOC_LITERAL(39, 0), // ""
QT_MOC_LITERAL(40, 26), // "DevicesAdjustingKitMessage"
QT_MOC_LITERAL(67, 9), // "upcbState"
QT_MOC_LITERAL(77, 18), // "OnSetDataToUstirov"
QT_MOC_LITERAL(96, 5), // "state"
QT_MOC_LITERAL(102, 20), // "OnGetDataFromUstirov"
QT_MOC_LITERAL(123, 21), // "OnGetStateFromMessage"
QT_MOC_LITERAL(145, 7), // "message"
QT_MOC_LITERAL(153, 16), // "OnRequestTimeOut"
QT_MOC_LITERAL(170, 12), // "OnResetQueue"
QT_MOC_LITERAL(183, 18), // "OnAllDataCollected"
QT_MOC_LITERAL(202, 13) // "OnSendMessage"

    },
    "UstirovMediator\0ToSendRarmUstirovState\0"
    "\0DevicesAdjustingKitMessage\0upcbState\0"
    "OnSetDataToUstirov\0state\0OnGetDataFromUstirov\0"
    "OnGetStateFromMessage\0message\0"
    "OnRequestTimeOut\0OnResetQueue\0"
    "OnAllDataCollected\0OnSendMessage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UstirovMediator[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   62,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    1,   65,    2, 0x0a,    3 /* Public */,
       7,    0,   68,    2, 0x0a,    5 /* Public */,
       8,    1,   69,    2, 0x08,    6 /* Private */,
      10,    0,   72,    2, 0x08,    8 /* Private */,
      11,    0,   73,    2, 0x08,    9 /* Private */,
      12,    0,   74,    2, 0x08,   10 /* Private */,
      13,    0,   75,    2, 0x08,   11 /* Private */,

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

       0        // eod
};

void UstirovMediator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UstirovMediator *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->ToSendRarmUstirovState((*reinterpret_cast< std::add_pointer_t<DevicesAdjustingKitMessage>>(_a[1]))); break;
        case 1: _t->OnSetDataToUstirov((*reinterpret_cast< std::add_pointer_t<DevicesAdjustingKitMessage>>(_a[1]))); break;
        case 2: _t->OnGetDataFromUstirov(); break;
        case 3: _t->OnGetStateFromMessage((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 4: _t->OnRequestTimeOut(); break;
        case 5: _t->OnResetQueue(); break;
        case 6: _t->OnAllDataCollected(); break;
        case 7: _t->OnSendMessage(); break;
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

const QMetaObject UstirovMediator::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_UstirovMediator.offsetsAndSize,
    qt_meta_data_UstirovMediator,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_UstirovMediator_t
, QtPrivate::TypeAndForceComplete<UstirovMediator, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const DevicesAdjustingKitMessage &, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const DevicesAdjustingKitMessage &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QByteArray &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
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
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void UstirovMediator::ToSendRarmUstirovState(const DevicesAdjustingKitMessage & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
