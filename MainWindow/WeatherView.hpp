#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QJsonObject>
#include <QNetworkAccessManager>

#include "../WeatherAPI/WeatherAPI.hpp"
#include "../WeatherAPI/WeatherObject.hpp"

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
    void onWeatherDataReceived(WeatherObject *reply);

private:
    void setupUI();
    void setupConnections();

    QLineEdit *cityInput;
    QPushButton *searchButton;
    QLabel *weatherIconLabel;
    QLabel *weatherLabel;

    WeatherAPI* api;
};
