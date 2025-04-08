#pragma once

#include "WeatherObject.hpp"

#include <QString>
#include <QNetworkRequest>
#include <QNetworkReply>

class WeatherAPI {
public:
WeatherAPI();
~WeatherAPI() {}

QNetworkRequest request(QString city);
WeatherObject parseRequest(QNetworkReply* reply);
private:
std::string getApiKey();

QString url;
QString apiKey;
};
