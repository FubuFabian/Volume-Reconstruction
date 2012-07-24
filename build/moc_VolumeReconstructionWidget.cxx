/****************************************************************************
** Meta object code from reading C++ file 'VolumeReconstructionWidget.h'
**
** Created: Tue Jul 24 11:50:33 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../VolumeReconstructionWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VolumeReconstructionWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_VolumeReconstructionWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      28,   27,   27,   27, 0x08,
      35,   27,   27,   27, 0x08,
      50,   46,   27,   27, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_VolumeReconstructionWidget[] = {
    "VolumeReconstructionWidget\0\0save()\0"
    "generate()\0idx\0setResolution(int)\0"
};

const QMetaObject VolumeReconstructionWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_VolumeReconstructionWidget,
      qt_meta_data_VolumeReconstructionWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &VolumeReconstructionWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *VolumeReconstructionWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *VolumeReconstructionWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VolumeReconstructionWidget))
        return static_cast<void*>(const_cast< VolumeReconstructionWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int VolumeReconstructionWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: save(); break;
        case 1: generate(); break;
        case 2: setResolution((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
