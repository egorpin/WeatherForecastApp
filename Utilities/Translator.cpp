#include <QMap>
#include <QString>

namespace {
QMap<QString, QString> translations = {
    {"Monday", "Понедельник"},
    {"Tuesday", "Вторник"},
    {"Wednesday", "Среда"},
    {"Thursday", "Четверг"},
    {"Friday", "Пятница"},
    {"Saturday", "Суббота"},
    {"Sunday", "Воскресенье"},
};
}

QString Translate(QString key){
    return translations.value(key, key);
}
