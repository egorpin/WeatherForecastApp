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


void requestForecast(QString city);


signals:
void weatherDataReady(WeatherObject*);
void forecastDataReady(QVector<WeatherObject*>*);


private slots:
void requestIcon(QNetworkReply* dataReply);
void parseForecast(QNetworkReply* dataReply);
void forecastIconReady(QVector<WeatherObject*>* wobj_vector);
void requestForecastIcon(QVector<WeatherObject*>* wobj_vector);

private:
std::string getApiKey();
WeatherObject* parseData(QNetworkReply* reply);


QNetworkAccessManager* weather_manager;
QNetworkAccessManager* forecast_manager;
QString current_weather_url;
QString forecast_url;
QString apiKey;
};
