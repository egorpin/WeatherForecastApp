#include "WeatherObject.hpp"

#include <QJsonArray>

WeatherObject::WeatherObject(bool is_valid){
    isValid = is_valid;
}

WeatherObject::WeatherObject(const QJsonObject& json, bool is_valid) : WeatherObject(is_valid) {
    city = json["name"].toString();
    temp = json["main"].toObject()["temp"].toDouble();
    humidity = json["main"].toObject()["humidity"].toInt();
    windSpeed = json["wind"].toObject()["speed"].toDouble();
    description = json["weather"].toArray()[0].toObject()["description"].toString();
    iconCode = json["weather"].toArray()[0].toObject()["icon"].toString();

    qDebug() << "Reply: " << *this;
}

QUrl WeatherObject::IconUrl() const {
    return QUrl(QString("https://openweathermap.org/img/wn/%1@2x.png").arg(iconCode));
}

QDebug operator<<(QDebug debug, const WeatherObject& wobj){
    debug << "City: " << wobj.city << '\n';
    debug << "Temp: " << wobj.temp << '\n';
    debug << "Humidity: " << wobj.humidity << '\n';
    debug << "WindSpeed: " << wobj.windSpeed << '\n';
    debug << "Description: " << wobj.description << '\n';
    return debug;
}

bool WeatherObject::IsValid() {
    return isValid;
}
