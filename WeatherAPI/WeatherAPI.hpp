#pragma once

#include "WeatherObject.hpp"

#include <QString>
#include <QNetworkRequest>
#include <QNetworkReply>

class WeatherAPI {
public:
WeatherAPI();
~WeatherAPI() {}

QNetworkRequest request();
WeatherObject parseRequest(QNetworkReply* reply);
private:
const char* getApiKey();

QString url;
QString apiKey;
QString city;
};
