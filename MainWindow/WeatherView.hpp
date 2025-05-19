#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTabWidget>
#include <QScrollArea>
#include <QDateTime>

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
    void applyStyles();
    QWidget* createDetailWidget(const QString &iconPath, const QString &value, const QString &title);
    QWidget* createForecastDayWidget(const QString &day, const QString &iconPath,
                                   const QString &temp, const QString &desc);
    QWidget* createHourlyWidget(const QString &time, const QString &iconPath,
                              const QString &temp, const QString &desc);

    WeatherAPI *api;

    // Search
    QLineEdit *cityInput;
    QPushButton *searchButton;

    // Current weather
    QLabel *cityLabel;
    QLabel *dateLabel;
    QLabel *weatherIcon;
    QLabel *tempLabel;
    QLabel *descLabel;
    QHBoxLayout *detailsLayout;

    // Forecast
    QTabWidget *forecastTabs;
    QWidget *dailyForecast;
    QScrollArea *hourlyForecast;

    // Constants
    const QString humidityIconPath = ":/icons/humidity.png";
    const QString windIconPath = ":/icons/wind.png";
    const QString pressureIconPath = ":/icons/pressure.png";
    const QString searchIconPath = ":/MainWindow/icons/search.png";
};
