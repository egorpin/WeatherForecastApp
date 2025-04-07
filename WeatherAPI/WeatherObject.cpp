#include "WeatherObject.hpp"

#include <QJsonArray>

WeatherObject::WeatherObject(const QJsonObject& json){
    city = json["name"].toString();
    temp = json["main"].toObject()["temp"].toDouble();
    humidity = json["main"].toObject()["humidity"].toInt();
    windSpeed = json["wind"].toObject()["speed"].toDouble();
    description = json["weather"].toArray()[0].toObject()["description"].toString();
    iconCode = json["weather"].toArray()[0].toObject()["icon"].toString();

    qDebug() << "Reply: " << *this;
}

QDebug operator<<(QDebug debug, const WeatherObject& wobj){
    debug << "City: " << wobj.city << '\n';
    debug << "Temp: " << wobj.temp << '\n';
    debug << "Humidity: " << wobj.humidity << '\n';
    debug << "WindSpeed: " << wobj.windSpeed << '\n';
    debug << "Description: " << wobj.description << '\n';
    return debug;
}
