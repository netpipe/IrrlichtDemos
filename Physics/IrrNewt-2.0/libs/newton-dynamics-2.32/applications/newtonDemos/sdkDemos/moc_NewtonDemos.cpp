/****************************************************************************
** Meta object code from reading C++ file 'NewtonDemos.h'
**
** Created: Thu Feb 17 13:04:50 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "toolbox_stdafx.h"
#include "NewtonDemos.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NewtonDemos.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_newtonDemos[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x0a,
      22,   12,   12,   12, 0x0a,
      31,   12,   12,   12, 0x0a,
      43,   12,   12,   12, 0x0a,
      55,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_newtonDemos[] = {
    "newtonDemos\0\0OnLoad()\0OnSave()\0"
    "OnRunDemo()\0OnNotUsed()\0OnIdle()\0"
};

const QMetaObject newtonDemos::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_newtonDemos,
      qt_meta_data_newtonDemos, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &newtonDemos::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *newtonDemos::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *newtonDemos::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_newtonDemos))
        return static_cast<void*>(const_cast< newtonDemos*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int newtonDemos::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnLoad(); break;
        case 1: OnSave(); break;
        case 2: OnRunDemo(); break;
        case 3: OnNotUsed(); break;
        case 4: OnIdle(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
