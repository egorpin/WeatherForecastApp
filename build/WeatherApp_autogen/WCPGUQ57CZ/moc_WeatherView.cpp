/****************************************************************************
** Meta object code from reading C++ file 'WeatherView.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../MainWindow/WeatherView.hpp"
#include <QtGui/qtextcursor.h>
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WeatherView.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_WeatherView_t {
    uint offsetsAndSizes[34];
    char stringdata0[12];
    char stringdata1[20];
    char stringdata2[1];
    char stringdata3[5];
    char stringdata4[15];
    char stringdata5[14];
    char stringdata6[5];
    char stringdata7[16];
    char stringdata8[22];
    char stringdata9[9];
    char stringdata10[17];
    char stringdata11[8];
    char stringdata12[21];
    char stringdata13[22];
    char stringdata14[15];
    char stringdata15[5];
    char stringdata16[23];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_WeatherView_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_WeatherView_t qt_meta_stringdata_WeatherView = {
    {
        QT_MOC_LITERAL(0, 11),  // "WeatherView"
        QT_MOC_LITERAL(12, 19),  // "citySearchRequested"
        QT_MOC_LITERAL(32, 0),  // ""
        QT_MOC_LITERAL(33, 4),  // "city"
        QT_MOC_LITERAL(38, 14),  // "displayWeather"
        QT_MOC_LITERAL(53, 13),  // "WeatherObject"
        QT_MOC_LITERAL(67, 4),  // "data"
        QT_MOC_LITERAL(72, 15),  // "displayForecast"
        QT_MOC_LITERAL(88, 21),  // "QList<WeatherObject*>"
        QT_MOC_LITERAL(110, 8),  // "forecast"
        QT_MOC_LITERAL(119, 16),  // "showErrorMessage"
        QT_MOC_LITERAL(136, 7),  // "message"
        QT_MOC_LITERAL(144, 20),  // "showLoadingIndicator"
        QT_MOC_LITERAL(165, 21),  // "onWeatherDataReceived"
        QT_MOC_LITERAL(187, 14),  // "WeatherObject*"
        QT_MOC_LITERAL(202, 4),  // "wobj"
        QT_MOC_LITERAL(207, 22)   // "onForecastDataReceived"
    },
    "WeatherView",
    "citySearchRequested",
    "",
    "city",
    "displayWeather",
    "WeatherObject",
    "data",
    "displayForecast",
    "QList<WeatherObject*>",
    "forecast",
    "showErrorMessage",
    "message",
    "showLoadingIndicator",
    "onWeatherDataReceived",
    "WeatherObject*",
    "wobj",
    "onForecastDataReceived"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_WeatherView[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   56,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    1,   59,    2, 0x0a,    3 /* Public */,
       7,    1,   62,    2, 0x0a,    5 /* Public */,
      10,    1,   65,    2, 0x0a,    7 /* Public */,
      12,    0,   68,    2, 0x0a,    9 /* Public */,
      13,    1,   69,    2, 0x08,   10 /* Private */,
      16,    1,   72,    2, 0x08,   12 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, 0x80000000 | 8,   15,

       0        // eod
};

Q_CONSTINIT const QMetaObject WeatherView::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_WeatherView.offsetsAndSizes,
    qt_meta_data_WeatherView,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_WeatherView_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<WeatherView, std::true_type>,
        // method 'citySearchRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'displayWeather'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const WeatherObject &, std::false_type>,
        // method 'displayForecast'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVector<WeatherObject*> &, std::false_type>,
        // method 'showErrorMessage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'showLoadingIndicator'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onWeatherDataReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<WeatherObject *, std::false_type>,
        // method 'onForecastDataReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QVector<WeatherObject*>, std::false_type>
    >,
    nullptr
} };

void WeatherView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WeatherView *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->citySearchRequested((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->displayWeather((*reinterpret_cast< std::add_pointer_t<WeatherObject>>(_a[1]))); break;
        case 2: _t->displayForecast((*reinterpret_cast< std::add_pointer_t<QList<WeatherObject*>>>(_a[1]))); break;
        case 3: _t->showErrorMessage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->showLoadingIndicator(); break;
        case 5: _t->onWeatherDataReceived((*reinterpret_cast< std::add_pointer_t<WeatherObject*>>(_a[1]))); break;
        case 6: _t->onForecastDataReceived((*reinterpret_cast< std::add_pointer_t<QList<WeatherObject*>>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (WeatherView::*)(const QString & );
            if (_t _q_method = &WeatherView::citySearchRequested; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *WeatherView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WeatherView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WeatherView.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int WeatherView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void WeatherView::citySearchRequested(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
