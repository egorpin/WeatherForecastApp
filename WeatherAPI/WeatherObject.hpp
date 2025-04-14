#pragma once

#include <QDebug>
#include <QJsonObject>

class WeatherObject {
private:
bool isValid;

public:
WeatherObject(bool is_valid=true);
WeatherObject(const QJsonObject& json, bool is_valid=true);
~WeatherObject() {}

QUrl IconUrl() const;

bool IsValid();

friend QDebug operator<<(QDebug debug, const WeatherObject& wobj);

QString city;
double temp;
int humidity;
double windSpeed;
QString description;
QString iconCode;
QByteArray iconImgdata;
};
