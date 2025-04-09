#pragma once

#include "WeatherObject.hpp"

#include <QString>
#include <QNetworkRequest>
#include <QNetworkReply>

class WeatherAPI : public QObject {
public:
WeatherAPI(QObject* parent = 0);
~WeatherAPI() {}

QNetworkRequest request(QString city);
WeatherObject parseRequest(QNetworkReply* reply);
void requestIcon(WeatherObject& wobj);


private:
std::string getApiKey();

QString url;
QString apiKey;
};
