#include "WeatherView.hpp"
#include "../Utilities/Translator.hpp"

#include <QDate>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include <QDebug>

WeatherView::WeatherView(QWidget *parent) : QWidget(parent) {
    api = new WeatherAPI(this);

    setupUI();
    setupConnections();

    api->request("Москва");
    api->requestForecast("Москва");

    //QTimer::singleShot(0, this, [this]() { searchWeather(); });

    // нужно делать обновление погоды раз в какое-то время
}

void WeatherView::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    // Header
    QHBoxLayout *headerLayout = new QHBoxLayout();

    cityTitleLabel = new QLabel("Москва", this);
    cityTitleLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: white;");
    cityTitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    headerLayout->addWidget(cityTitleLabel, 1);

    cityInput = new QLineEdit(this);
    cityInput->setPlaceholderText("Введите город");
    cityInput->setFixedWidth(200);
    cityInput->setStyleSheet(
        "color: black;"
        "background-color: rgba(255, 255, 255, 177);"
        "border: 1px solid rgba(255, 255, 255, 50);"
        "border-radius: 4px;"
        "padding: 8px;"
        "font-size: 14px;"
    );

    searchButton = new QPushButton(this);
    QPixmap searchPixmap(":/MainWindow/icons/search.png");
    if(!searchPixmap.isNull()) {
        searchButton->setIcon(QIcon(searchPixmap));
    } else {
        qDebug() << "Валера блять где значок поиска";
        searchButton->setText("Search");
    }
    searchButton->setIconSize(QSize(24, 24));
    searchButton->setFixedSize(40, 40);
    searchButton->setStyleSheet(
        "QPushButton {"
        "   border: none;"
        "   background-color: rgba(255, 255, 255, 100);"
        "   border-radius: 20px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(255, 255, 255, 150);"
        "}"
    );

    headerLayout->addWidget(cityInput);
    headerLayout->addWidget(searchButton);
    mainLayout->addLayout(headerLayout);

    // Current weather
    QHBoxLayout *currentWeatherLayout = new QHBoxLayout();

    weatherIconLabel = new QLabel(this);
    weatherIconLabel->setAlignment(Qt::AlignCenter);
    weatherIconLabel->setFixedSize(150, 150);

    currentWeatherLabel = new QLabel(this);
    currentWeatherLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    currentWeatherLabel->setStyleSheet("font-size: 18px; color: white;");

    currentWeatherLayout->addWidget(weatherIconLabel);
    currentWeatherLayout->addWidget(currentWeatherLabel, 1);
    mainLayout->addLayout(currentWeatherLayout);

    // 5-day forecast
    QLabel *forecastTitle = new QLabel("Прогноз на 5 дней", this);
    forecastTitle->setStyleSheet("font-size: 24px; font-weight: bold; color: white;");
    mainLayout->addWidget(forecastTitle);

    forecastContainer = new QWidget(this);
    forecastLayout = new QGridLayout(forecastContainer);
    forecastLayout->setHorizontalSpacing(15);
    forecastLayout->setVerticalSpacing(10);

    for(int i = 0; i < 5; i++) {
        QWidget *dayWidget = new QWidget(forecastContainer);
        dayWidget->setStyleSheet(
            "background-color: rgba(255, 255, 255, 50);"
            "border-radius: 10px;"
            "padding: 10px;"
        );

        QVBoxLayout *dayLayout = new QVBoxLayout(dayWidget);
        QLabel *dayLabel = new QLabel("День", dayWidget);
        dayLabel->setObjectName(QString("daylabel%1").arg(i));
        QLabel *tempLabel = new QLabel("--°C", dayWidget);

        tempLabel->setObjectName(QString("templabel%1").arg(i));

        dayLabel->setStyleSheet("color: white; font-size: 14px;");
        tempLabel->setStyleSheet("color: white; font-size: 16px; font-weight: bold;");

        dayLayout->addWidget(dayLabel);
        dayLayout->addWidget(tempLabel);
        forecastLayout->addWidget(dayWidget, 0, i);
    }

    mainLayout->addWidget(forecastContainer);
}

void WeatherView::setupConnections() {
    connect(api, &WeatherAPI::weatherDataReady, this, &WeatherView::onWeatherDataReceived);
    connect(searchButton, &QPushButton::clicked, [this]() {
        // нужно показать индикатор загрузки
        QString city = cityInput->text().trimmed();
        if (city.isEmpty()) {
            showErrorMessage("Введите название города");
            return;
        }
        emit citySearchRequested(city);
    });
    connect(api, &WeatherAPI::forecastDataReady, this, &WeatherView::onForecastDataReceived);
}

void WeatherView::citySearchRequested(const QString& city) {
    api->request(city);
    api->requestForecast(city);
}

void WeatherView::onWeatherDataReceived(WeatherObject *wobj) {
    if (!wobj) {
        showErrorMessage("Неверные данные о погоде");
        return;
    }

    displayWeather(*wobj);
    delete wobj;
}

void WeatherView::onForecastDataReceived(QVector<WeatherObject*>* wobj) {
    if (!wobj || wobj->isEmpty()) {
        showErrorMessage("Неверные данные о погоде");
        return;
    }

    displayForecast(*wobj);
}

void WeatherView::displayWeather(const WeatherObject &data) {
    if (!cityTitleLabel || !weatherIconLabel || !currentWeatherLabel) return;

    cityTitleLabel->setText(data.city);

    if (!data.iconImgdata.isEmpty()) {
        QPixmap pixmap;
        if (pixmap.loadFromData(data.iconImgdata)) {
            weatherIconLabel->setPixmap(pixmap.scaled(150, 150, Qt::KeepAspectRatio));
        }
    }

    QString weatherText = QString(
        "<p><b>Температура:</b> %1°C</p>"
        "<p><b>Влажность:</b> %3%</p>"
        "<p><b>Ветер:</b> %4 м/с</p>"
        "<p><b>Описание:</b> %6</p>"
    ).arg(data.temp)
     .arg(data.humidity)
     .arg(data.windSpeed)
     .arg(data.description);

    currentWeatherLabel->setText(weatherText);
}

void WeatherView::displayForecast(const QVector<WeatherObject*>& forecast) {
    QList<QLabel*> dayLabels = forecastContainer->findChildren<QLabel*>(QRegularExpression("daylabel\\d"));
    QList<QLabel*> tempLabels = forecastContainer->findChildren<QLabel*>(QRegularExpression("templabel\\d"));

    for (int i = 0; i < forecast.size() && i < dayLabels.size(); i++) {
        QString dayName = forecast[i]->getDayOfWeek();
        //QString translatedDay = dayTranslations.value(dayName, dayName); // Если перевода нет, оставляем оригинал

        dayLabels[i]->setText(Translate(dayName));
        tempLabels[i]->setText(QString("%1°C").arg(std::ceil(forecast[i]->temp)));
    }
}

void WeatherView::showErrorMessage(const QString &message) {
    currentWeatherLabel->setText("<span style='color:red;'>" + message + "</span>");
}

void WeatherView::showLoadingIndicator() {
    currentWeatherLabel->setText("Загрузка данных...");
}

std::string WeatherView::getSelectedCity() const {
    return cityInput->text().trimmed().toStdString();
}
