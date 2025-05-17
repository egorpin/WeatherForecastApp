#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include "../WeatherAPI/WeatherAPI.hpp"
#include "../WeatherAPI/WeatherObject.hpp"

class WeatherView : public QWidget {
    Q_OBJECT
public:
    explicit WeatherView(QWidget *parent = nullptr);

    std::string getSelectedCity() const;

    void citySearchRequested(const QString &city);

public slots:
    void displayWeather(const WeatherObject &data);
    void displayForecast(const QVector<WeatherObject*>& forecast);
    void showErrorMessage(const QString &message);
    void showLoadingIndicator();

private slots:
    void onWeatherDataReceived(WeatherObject *wobj);
    void onForecastDataReceived(QVector<WeatherObject*>* wobj);

private:
    void setupUI();
    void setupConnections();


    WeatherAPI *api;
    QLabel *cityTitleLabel;
    QLineEdit *cityInput;
    QPushButton *searchButton;
    QLabel *weatherIconLabel;
    QLabel *currentWeatherLabel;
    QWidget *forecastContainer;
    QGridLayout *forecastLayout;
};
