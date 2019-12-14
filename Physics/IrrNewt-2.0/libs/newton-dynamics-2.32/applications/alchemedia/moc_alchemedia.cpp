/****************************************************************************
** Meta object code from reading C++ file 'alchemedia.h'
**
** Created: Thu Feb 17 13:26:50 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "alchemedia.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'alchemedia.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_alchemedia[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      25,   11,   11,   11, 0x0a,
      39,   11,   11,   11, 0x0a,
      54,   11,   11,   11, 0x0a,
      68,   11,   11,   11, 0x0a,
      84,   11,   11,   11, 0x0a,
      95,   11,   11,   11, 0x0a,
     106,   11,   11,   11, 0x0a,
     115,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_alchemedia[] = {
    "alchemedia\0\0OnNewScene()\0OnOpenScene()\0"
    "OnMergeScene()\0OnSaveScene()\0"
    "OnSaveSceneAs()\0OnImport()\0OnExport()\0"
    "OnUndo()\0OnRedo()\0"
};

const QMetaObject alchemedia::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_alchemedia,
      qt_meta_data_alchemedia, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &alchemedia::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *alchemedia::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *alchemedia::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_alchemedia))
        return static_cast<void*>(const_cast< alchemedia*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int alchemedia::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnNewScene(); break;
        case 1: OnOpenScene(); break;
        case 2: OnMergeScene(); break;
        case 3: OnSaveScene(); break;
        case 4: OnSaveSceneAs(); break;
        case 5: OnImport(); break;
        case 6: OnExport(); break;
        case 7: OnUndo(); break;
        case 8: OnRedo(); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
