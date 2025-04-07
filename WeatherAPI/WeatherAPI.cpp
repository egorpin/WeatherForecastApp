#include "WeatherAPI.hpp"

#include <fstream>

#include <QJsonDocument>
#include <QException>

WeatherAPI::WeatherAPI(){
    apiKey = getApiKey();
    url = "https://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2&units=metric&lang=ru";
    city = "Moscow";
}

const char* WeatherAPI::getApiKey(){
    std::ifstream file(".env");

    std::string strapi;
    file >> strapi;

    return strapi.c_str();
}

QNetworkRequest WeatherAPI::request(){
    qDebug() << "Request to " << url.arg(city).arg(apiKey);
    return QNetworkRequest(url.arg(city).arg(apiKey));
}

WeatherObject parseRequest(QNetworkReply* reply) {
    reply->deleteLater(); // !!!

    if (reply->error()) {
        throw QException();
    }

    QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());

    if (jsonResponse.isNull()) {
        throw QException();
    }

    QJsonObject jsonObject = jsonResponse.object();

    return WeatherObject(jsonObject);
}
