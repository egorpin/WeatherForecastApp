#include "WeatherObject.hpp"

#include <QJsonArray>

bool iconCodeComparator(const QPair<QString, int>& l, const QPair<QString, int>& r){
    return (l.second < r.second) || (l.second == r.second) && (l.first < r.first);
}

WeatherObject::WeatherObject(bool is_valid){
    isValid = is_valid;
    iconImgdata = QByteArray(0, Qt::Initialization::Uninitialized);
}

QString WeatherObject::getDayOfWeek() const {
    QDateTime dateTime;
    dateTime.setSecsSinceEpoch(dt); // dt нужно добавить как поле класса
    return dateTime.toString("dddd"); // Полное название дня недели
    // Или для короткого названия: return dateTime.toString("ddd");
}

WeatherObject::WeatherObject(const QJsonObject& json, bool is_valid) : WeatherObject(is_valid) {
    dt = json["dt"].toVariant().toLongLong();
    city = json["name"].toString();
    temp = json["main"].toObject()["temp"].toDouble();
    humidity = json["main"].toObject()["humidity"].toInt();
    windSpeed = json["wind"].toObject()["speed"].toDouble();
    description = json["weather"].toArray()[0].toObject()["description"].toString();
    iconCode = json["weather"].toArray()[0].toObject()["icon"].toString();
    weatherId = json["weather"].toArray()[0].toObject()["id"].toInt();

    qDebug() << "Reply: " << *this;
}

WeatherObject::WeatherObject(QVector<WeatherObject*> day_forecast, bool is_valid) : WeatherObject(is_valid) {
    std::map<QString, int> iconMap;
    if (!day_forecast.isEmpty()) {
        dt = day_forecast[0]->dt; // Берем timestamp из первого объекта дня
        weatherId = day_forecast[0]->weatherId;
    }

    for (auto& wobj : day_forecast){
        temp += wobj->temp / day_forecast.size();
        qDebug() << "TEMP=" << temp << " WOBJ TEMP=" << wobj->temp;
        humidity += static_cast<double>(wobj->humidity) / day_forecast.size();
        windSpeed += wobj->windSpeed / day_forecast.size();

        if (iconMap.find(wobj->iconCode) == iconMap.end()){
            iconMap[wobj->iconCode] = 0;
        }

        iconMap[wobj->iconCode]++;
    }

    iconCode = std::max_element(iconMap.begin(), iconMap.end(), iconCodeComparator)->first;

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
    debug << "Weather ID: " << wobj.weatherId << '\n';
    return debug;
}

bool WeatherObject::IsValid() {
    return isValid;
}
