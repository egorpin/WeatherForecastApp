#include "WeatherAPI.hpp"

#include "../Utilities/Filedownloader.hpp"

#include "../MainWindow/WeatherView.hpp"

#include <fstream>

#include <QException>
#include <QJsonDocument>
#include <QJsonArray>


WeatherAPI::WeatherAPI(QObject* parent) : QObject(parent) {
    weather_manager = new QNetworkAccessManager(this);
    forecast_manager = new QNetworkAccessManager(this);

    apiKey = QString(getApiKey().c_str());
    current_weather_url = "https://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2&units=metric&lang=ru";
    forecast_url = "https://api.openweathermap.org/data/2.5/forecast?q=%1&appid=%2&units=metric&lang=ru";

    connect(weather_manager, &QNetworkAccessManager::finished, this, &WeatherAPI::requestIcon);
    connect(forecast_manager, &QNetworkAccessManager::finished, this, &WeatherAPI::parseForecast);
}

std::string WeatherAPI::getApiKey() {
    std::ifstream file("../.env");

    std::string strapi;
    file >> strapi;

    return strapi;
}

void WeatherAPI::request(QString city) {
    qDebug() << "Request to " << current_weather_url.arg(city);
    weather_manager->get(QNetworkRequest(current_weather_url.arg(city).arg(apiKey)));
}

void WeatherAPI::requestForecast(QString city){
    qDebug() << "Request to " << forecast_url.arg(city);
    forecast_manager->get(QNetworkRequest(forecast_url.arg(city).arg(apiKey)));
}


void WeatherAPI::parseForecast(QNetworkReply* dataReply){
    if (dataReply->error()) {
        qCritical() << dataReply->error();
        emit forecastDataReady(nullptr);
    }

    QJsonDocument jsonResponse = QJsonDocument::fromJson(dataReply->readAll());

    if (jsonResponse.isNull()) {
        emit forecastDataReady(nullptr);
    }
    QJsonObject jsonObject = jsonResponse.object();

    int cnt = jsonObject["cnt"].toInt();
    QJsonArray array = jsonObject["list"].toArray();
    QVector<WeatherObject*> vector;
    for (auto&& jObject : array) {
        vector.append(new WeatherObject(jObject.toObject()));
    }


    QVector<WeatherObject*>* result = new QVector<WeatherObject*>();
    for (int i = 0;i < cnt / 8;i++){
        result->append(new WeatherObject(QVector<WeatherObject*>(vector.begin() + i * 8, vector.begin() + i * 8 + 8)));
        requestForecastIcon(result);
    }
}

void WeatherAPI::requestIcon(QNetworkReply* dataReply) {
    dataReply->deleteLater();

    WeatherObject* wobj = parseData(dataReply);

    if (!wobj->IsValid()) {
        emit weatherDataReady(wobj);
        return;
    }

    qDebug() << wobj->IconUrl();
    FileDownloader* fd = new FileDownloader(wobj->IconUrl(), this);

    connect(
        fd, &FileDownloader::downloaded, this,
        [=](QByteArray imageData) {
            wobj->iconImgdata = imageData;
            emit weatherDataReady(wobj);
        },
        Qt::ConnectionType::SingleShotConnection);
}

void WeatherAPI::requestForecastIcon(QVector<WeatherObject*>* wobj_vector){
    for (auto& wobj : *wobj_vector){
        FileDownloader* fd = new FileDownloader(wobj->IconUrl(), this);

        connect(
            fd, &FileDownloader::downloaded, this,
            [=](QByteArray imageData) {
                wobj->iconImgdata = imageData;
                forecastIconReady(wobj_vector);
            },
            Qt::ConnectionType::SingleShotConnection);
    }
}

void WeatherAPI::forecastIconReady(QVector<WeatherObject*>* wobj_vector){
    for (auto& wobj : *wobj_vector){
        if (wobj->iconImgdata.size() == 0){
            return;
        }
    }

    emit forecastDataReady(wobj_vector);
}

WeatherObject* WeatherAPI::parseData(QNetworkReply* dataReply) {
    if (dataReply->error()) {
        qCritical() << dataReply->error();
        return new WeatherObject(false);
    }
    QJsonDocument jsonResponse = QJsonDocument::fromJson(dataReply->readAll());

    if (jsonResponse.isNull()) {
        return new WeatherObject(false);
    }
    QJsonObject jsonObject = jsonResponse.object();

    return new WeatherObject(jsonObject);
}
