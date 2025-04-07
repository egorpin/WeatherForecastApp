#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QJsonObject>
#include <QNetworkAccessManager>

#include "../WeatherAPI/WeatherAPI.hpp"

class WeatherView : public QWidget {
public:
    explicit WeatherView(QWidget *parent = nullptr);

    std::string getSelectedCity() const;
    void displayWeather(const WeatherObject &data);
    void showErrorMessage(const QString &message);
    void showLoadingIndicator();

signals:
    void citySearchRequested(const QString& city);

private slots:
    void onWeatherDataReceived(QNetworkReply *reply);

private:
    void setupUI();
    void setupConnections();

    QNetworkAccessManager *manager;
    QLineEdit *cityInput;
    QPushButton *searchButton;
    QLabel *weatherIconLabel;
    QLabel *weatherLabel;

    WeatherAPI api;
};
