#include "WeatherView.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QJsonObject>  
#include <QJsonArray>  

WeatherView::WeatherView(QWidget *parent) : QWidget(parent) {
    setupUI();
   // setupConnections();
}

void WeatherView::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // City selection
    QHBoxLayout *cityLayout = new QHBoxLayout();
    
    cityInput = new QLineEdit(this);
    cityInput->setPlaceholderText("Введите город");
    
    searchButton = new QPushButton(this);
    searchButton->setIcon(QIcon(":MainWindow/icons/search.png"));
    searchButton->setIconSize(QSize(24, 24));
    searchButton->setText("");
    searchButton->setFixedSize(32, 32);
    searchButton->setStyleSheet(
        "QPushButton { border: none; background-color: transparent; }"
        "QPushButton:hover { background-color: #e0e0e0; border-radius: 16px; }"
    );
    
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

/* void WeatherView::setupConnections() {
    connect(searchButton, &QPushButton::clicked, [this]() {
        QString city = cityInput->text().trimmed();
        if (city.isEmpty()) {
            city = cityComboBox->currentText();
        }
        emit citySearchRequested(city);  
    });
} */

std::string WeatherView::getSelectedCity() const {
    QString city = cityInput->text().trimmed();
    return city.toStdString();
}

void WeatherView::displayWeather(const QJsonObject &data) {
    QString city = data.value("name").toString();
    double temp = data.value("main").toObject().value("temp").toDouble();
    int humidity = data.value("main").toObject().value("humidity").toInt();
    double windSpeed = data.value("wind").toObject().value("speed").toDouble();
    QString description = data.value("weather").toArray()[0].toObject().value("description").toString();
    
    QString weatherText = QString(
        "<h2>%1</h2>"
        "<p><b>Температура:</b> %2°C</p>"
        "<p><b>Влажность:</b> %3%</p>"
        "<p><b>Ветер:</b> %4 м/с</p>"
        "<p><b>Состояние:</b> %5</p>"
    ).arg(city).arg(temp).arg(humidity).arg(windSpeed).arg(description);
    
    weatherLabel->setText(weatherText);
}

void WeatherView::showErrorMessage(const QString &message) {
    weatherLabel->setText(message);
}

void WeatherView::showLoadingIndicator() {
    weatherLabel->setText("Загрузка данных...");
}