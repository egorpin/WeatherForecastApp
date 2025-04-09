#pragma once

#include <QDebug>
#include <QJsonObject>

class WeatherObject {
public:
WeatherObject(const QJsonObject& json);
~WeatherObject() {}

QUrl IconUrl() const;

friend QDebug operator<<(QDebug debug, const WeatherObject& wobj);

QString city;
double temp;
int humidity;
double windSpeed;
QString description;
QString iconCode;
QByteArray iconImgdata;
};
