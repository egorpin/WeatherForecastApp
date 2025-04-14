#include "WeatherView.hpp"

#include <QHBoxLayout>
#include <QIcon>
#include <QJsonArray>
#include <QJsonObject>
#include <QVBoxLayout>


WeatherView::WeatherView(QWidget* parent) : QWidget(parent) {
    api = new WeatherAPI(this);

    setupUI();
    setupConnections();

    connect(api, &WeatherAPI::weatherDataReady, this, &WeatherView::onWeatherDataReceived);
    api->request("Москва");

    //QTimer::singleShot(0, this, [this]() { searchWeather(); });

    // нужно делать обновление погоды раз в какое-то время
}


void WeatherView::onWeatherDataReceived(WeatherObject* wobj) {
    if (!wobj->IsValid()){
        // иконку погоды надо очистить
        weatherLabel->setText("Ошибка: неверный формат данных");
        return;
    }
    displayWeather(*wobj);
}

void WeatherView::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // City selection
    QHBoxLayout* cityLayout = new QHBoxLayout();

    cityInput = new QLineEdit(this);
    cityInput->setPlaceholderText("Введите город");

    searchButton = new QPushButton(this);
    searchButton->setIcon(QIcon(":MainWindow/icons/search.png"));
    searchButton->setIconSize(QSize(24, 24));
    searchButton->setText("");
    searchButton->setFixedSize(32, 32);
    searchButton->setStyleSheet(
        "QPushButton { border: none; background-color: transparent; }"
        "QPushButton:hover { background-color: #e0e0e0; border-radius: 16px; }");

    cityLayout->addWidget(cityInput);
    cityLayout->addWidget(searchButton);

    // Weather display
    weatherIconLabel = new QLabel(this);
    weatherIconLabel->setAlignment(Qt::AlignCenter);

    weatherLabel = new QLabel(this);
    weatherLabel->setAlignment(Qt::AlignCenter);
    weatherLabel->setStyleSheet("font-size: 16px;");

    mainLayout->addLayout(cityLayout);
    mainLayout->addWidget(weatherIconLabel);
    mainLayout->addWidget(weatherLabel);

    mainLayout->addStretch();
}


void WeatherView::setupConnections() {
    connect(searchButton, &QPushButton::clicked, [this]() {
        // нужно показать индикатор загрузки
        QString city = cityInput->text().trimmed();
        if (city.isEmpty()) {
            city = QString("");
        }
        emit citySearchRequested(city);
    });
}

std::string WeatherView::getSelectedCity() const {
    QString city = cityInput->text().trimmed();
    return city.toStdString();
}

void WeatherView::citySearchRequested(const QString& city) {
    api->request(city);
}

void WeatherView::displayWeather(const WeatherObject& data) {
    QString weatherText = QString(
                              "<h2>%1</h2>"
                              "<p><b>Температура:</b> %2°C</p>"
                              "<p><b>Влажность:</b> %3%</p>"
                              "<p><b>Ветер:</b> %4 м/с</p>"
                              "<p><b>Состояние:</b> %5</p>")
                              .arg(data.city)
                              .arg(data.temp)
                              .arg(data.humidity)
                              .arg(data.windSpeed)
                              .arg(data.description);

    weatherLabel->setText(weatherText);

    QPixmap image;
    image.loadFromData(data.iconImgdata);
    weatherIconLabel->setPixmap(image);
}

void WeatherView::showErrorMessage(const QString& message) {
    weatherLabel->setText(message);
}

void WeatherView::showLoadingIndicator() {
    weatherLabel->setText("Загрузка данных...");
}
