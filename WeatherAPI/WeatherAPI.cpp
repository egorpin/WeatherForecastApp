#include "WeatherAPI.hpp"

#include <fstream>

#include <QJsonDocument>
#include <QException>

WeatherAPI::WeatherAPI(){
    apiKey = QString(getApiKey().c_str());
    url = "https://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2&units=metric&lang=ru";
}

std::string WeatherAPI::getApiKey(){
    std::ifstream file("../.env");

    std::string strapi;
    file >> strapi;

    return strapi;
}

QNetworkRequest WeatherAPI::request(QString city){
    qDebug() << "Request to " << url.arg(city);
    return QNetworkRequest(url.arg(city).arg(apiKey));
}

WeatherObject WeatherAPI::parseRequest(QNetworkReply* reply) {\
    if (reply->error()) {
        qCritical() << reply->error();
        throw QException();
    }
    QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());

    if (jsonResponse.isNull()) {
        throw QException();
    }
    QJsonObject jsonObject = jsonResponse.object();

    return WeatherObject(jsonObject);
}
