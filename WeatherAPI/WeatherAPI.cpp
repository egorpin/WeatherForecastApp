#include "WeatherAPI.hpp"

#include "../Utilities/filedownloader.hpp"

#include "../MainWindow/WeatherView.hpp"

#include <fstream>

#include <QJsonDocument>
#include <QException>

WeatherAPI::WeatherAPI(QObject* parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);

    apiKey = QString(getApiKey().c_str());
    url = "https://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2&units=metric&lang=ru";
}

std::string WeatherAPI::getApiKey(){
    std::ifstream file("../.env");

    std::string strapi;
    file >> strapi;

    return strapi;
}

void WeatherAPI::request(QString city){
    qDebug() << "Request to " << url.arg(city);
    manager->get(QNetworkRequest(url.arg(city).arg(apiKey)));

    connect(manager, &QNetworkAccessManager::finished, this, &WeatherAPI::requestIcon, Qt::ConnectionType::SingleShotConnection);
}

void WeatherAPI::requestIcon(QNetworkReply* dataReply) {
    dataReply->deleteLater();

    WeatherObject* wobj = parseData(dataReply);

    qDebug() << wobj->IconUrl();
    FileDownloader* fd = new FileDownloader(wobj->IconUrl(), this);

    connect(fd, &FileDownloader::downloaded, this, [=] (QByteArray imageData) {
        wobj->iconImgdata = imageData;
        emit weatherDataReady(wobj);
    }, Qt::ConnectionType::SingleShotConnection);
}


WeatherObject* WeatherAPI::parseData(QNetworkReply* dataReply) {\
    if (dataReply->error()) {
        qCritical() << dataReply->error();
        throw QException();
    }
    QJsonDocument jsonResponse = QJsonDocument::fromJson(dataReply->readAll());

    if (jsonResponse.isNull()) {
        throw QException();
    }
    QJsonObject jsonObject = jsonResponse.object();

    return new WeatherObject(jsonObject);
}
