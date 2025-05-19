#include "WeatherView.hpp"
#include "ErrorDialog.hpp"
#include "../Utilities/Translator.hpp"
#include "style.qcss"

#include <QDate>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include <QDebug>
#include <QTimer>
#include <QRegularExpression>
#include <QTabBar>
WeatherView::WeatherView(QWidget *parent) : QWidget(parent) {
    api = new WeatherAPI(this);

    setupUI();
    setupConnections();
    applyStyles();

    api->request("Москва");
    api->requestForecast("Москва");
}

void WeatherView::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    // Search bar
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->setSpacing(10);

    cityInput = new QLineEdit(this);
    cityInput->setPlaceholderText("Введите город");

    searchButton = new QPushButton(this);
    QPixmap searchPixmap(":/MainWindow/icons/search.png");
    if(!searchPixmap.isNull()) {
        searchButton->setIcon(QIcon(searchPixmap));
    } else {
        searchButton->setText("Поиск");
    }
    searchButton->setIconSize(QSize(24, 24));

    searchLayout->addWidget(cityInput);
    searchLayout->addWidget(searchButton);
    mainLayout->addLayout(searchLayout);

    // Current weather
    QWidget *currentWeatherWidget = new QWidget(this);
    QVBoxLayout *currentLayout = new QVBoxLayout(currentWeatherWidget);
    currentLayout->setContentsMargins(20, 20, 20, 20);
    currentLayout->setSpacing(15);

    cityLabel = new QLabel("Москва", this);
    cityLabel->setAlignment(Qt::AlignCenter);

    weatherIcon = new QLabel(this);
    weatherIcon->setAlignment(Qt::AlignCenter);
    weatherIcon->setFixedSize(100, 100);

    tempLabel = new QLabel("--°C", this);
    tempLabel->setAlignment(Qt::AlignCenter);

    descLabel = new QLabel("--", this);
    descLabel->setAlignment(Qt::AlignCenter);

    detailsLayout = new QHBoxLayout();
    detailsLayout->setSpacing(20);

    currentLayout->addWidget(cityLabel);
    currentLayout->addWidget(weatherIcon);
    currentLayout->addWidget(tempLabel);
    currentLayout->addWidget(descLabel);
    currentLayout->addLayout(detailsLayout);

    mainLayout->addWidget(currentWeatherWidget);

    // Forecast tabs
    forecastTabs = new QTabWidget(this);

    // Daily forecast tab
    dailyForecast = new QWidget();
    QVBoxLayout *dailyLayout = new QVBoxLayout(dailyForecast);
    dailyLayout->setContentsMargins(5, 5, 5, 5);
    dailyLayout->setSpacing(10);

    for(int i = 0; i < 5; i++) {
        dailyLayout->addWidget(createForecastDayWidget(
            QDate::currentDate().addDays(i).toString("ddd, d MMM"),
            "",
            "--°C",
            "--"
        ));
    }


    forecastTabs->addTab(dailyForecast, "На 5 дней");

    forecastTabs->tabBar()->setHidden(true); 
    mainLayout->addWidget(forecastTabs, 1000, Qt::AlignCenter);
}

void WeatherView::setupConnections() {
    connect(api, &WeatherAPI::weatherDataReady, this, &WeatherView::onWeatherDataReceived);
    connect(api, &WeatherAPI::forecastDataReady, this, &WeatherView::onForecastDataReceived);

    connect(searchButton, &QPushButton::clicked, [this]() {
        QString city = cityInput->text().trimmed();
        if (city.isEmpty()) {
            showErrorMessage("Введите название города");
            return;
        }
        showLoadingIndicator();
        emit citySearchRequested(city);
    });

    connect(cityInput, &QLineEdit::returnPressed, [this]() {
        QString city = cityInput->text().trimmed();
        if (city.isEmpty()) {
            showErrorMessage("Введите название города");
            return;
        }
        showLoadingIndicator();
        emit citySearchRequested(city);
    });
}

void WeatherView::applyStyles() {
    QString combinedStyle = appStyle;
    
    setStyleSheet(combinedStyle);                      
}

QWidget* WeatherView::createDetailWidget(const QString &iconPath, const QString &value, const QString &title) {
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->setSpacing(5);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *icon = new QLabel();
    if (!iconPath.isEmpty()) {
        icon->setPixmap(QPixmap(iconPath).scaled(30, 30, Qt::KeepAspectRatio));
    }
    icon->setAlignment(Qt::AlignCenter);

    QLabel *valueLabel = new QLabel(value);
    valueLabel->setAlignment(Qt::AlignCenter);

    QLabel *titleLabel = new QLabel(title);
    titleLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(icon);
    layout->addWidget(valueLabel);
    layout->addWidget(titleLabel);

    return widget;
}

QWidget* WeatherView::createForecastDayWidget(const QString &day, const QString &iconPath,
                                           const QString &temp, const QString &desc) {
    QWidget *widget = new QWidget();
    widget->setObjectName("forecastDayWidget");
    widget->setFixedHeight(80);

    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(15, 5, 15, 5);

    QLabel *dayLabel = new QLabel(day);
    dayLabel->setFixedWidth(120);

    QLabel *icon = new QLabel();
    if (!iconPath.isEmpty()) {
        icon->setPixmap(QPixmap(iconPath).scaled(50, 50, Qt::KeepAspectRatio));
    }

    QLabel *tempLabel = new QLabel(temp);
    tempLabel->setFixedWidth(80);

    QLabel *descLabel = new QLabel(desc);
    descLabel->setWordWrap(true);

    layout->addWidget(dayLabel);
    layout->addWidget(icon);
    layout->addWidget(tempLabel);
    layout->addWidget(descLabel);

    return widget;
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
    if (data.city.isEmpty()) return;

    cityLabel->setText(data.city);
    tempLabel->setText(QString("%1°C").arg(data.temp));
    descLabel->setText(data.description);

    if (!data.iconImgdata.isEmpty()) {
        QPixmap pixmap;
        if (pixmap.loadFromData(data.iconImgdata)) {
            weatherIcon->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatio));
        }
    }

    // Clear old details
    QLayoutItem *child;
    while (child = detailsLayout->takeAt(0)) {
        delete child->widget();
        delete child;
    }

    // Add new details
    detailsLayout->addWidget(createDetailWidget(":MainWindow/icons/humidity.png",
        QString("%5%").arg(data.humidity), "Влажность"));
        
    detailsLayout->addWidget(createDetailWidget(":MainWindow/icons/wind.png",
        QString("%1 м/с").arg(data.windSpeed), "Ветер"));
}

void WeatherView::displayForecast(const QVector<WeatherObject*>& forecast) {
    // Update daily forecast
    QVBoxLayout *dailyLayout = qobject_cast<QVBoxLayout*>(dailyForecast->layout());
    if (dailyLayout) {
        QLayoutItem *child;
        while (child = dailyLayout->takeAt(0)) {
            delete child->widget();
            delete child;
        }

        for (int i = 0; i < forecast.size() && i < 5; i++) {
            QString dayName = forecast[i]->getDayOfWeek();
            dailyLayout->addWidget(createForecastDayWidget(
                Translate(dayName),
                "",
                QString("%1°C").arg(std::ceil(forecast[i]->temp)),
                forecast[i]->description
            ));
        }
    }

}

void WeatherView::showErrorMessage(const QString &message) {
    ErrorDialog dialog(message, this);
    dialog.exec();
}

void WeatherView::showLoadingIndicator() {
    tempLabel->setText("Загрузка...");
    descLabel->setText("Получение данных о погоде");
}

std::string WeatherView::getSelectedCity() const {
    return cityInput->text().trimmed().toStdString();
}
