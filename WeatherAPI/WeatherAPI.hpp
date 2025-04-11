#pragma once

#include "WeatherObject.hpp"

#include <QString>
#include <QNetworkRequest>
#include <QNetworkReply>

class WeatherAPI : public QObject {
Q_OBJECT
public:
WeatherAPI(QObject* parent = 0);
~WeatherAPI() {}

void request(QString city);
WeatherObject* parseData(QNetworkReply* reply);
void requestIcon(QNetworkReply* dataReply);

signals:
void weatherDataReady(WeatherObject*);

private:
std::string getApiKey();

QNetworkAccessManager* manager;
QString url;
QString apiKey;
};
