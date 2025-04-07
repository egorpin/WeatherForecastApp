#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QComboBox>

#include <fstream>

class WeatherApp : public QMainWindow {
public:
    WeatherApp() : QMainWindow() {
        setupUI();
        setupConnections();

        manager = new QNetworkAccessManager(this);
        connect(manager, &QNetworkAccessManager::finished, this, &WeatherApp::onWeatherDataReceived);

            searchWeather();
    }


private slots:
    void searchWeather() {
        QString city = cityInput->text().trimmed();
        if (city.isEmpty()) {
            city = cityComboBox->currentText();
        }

        if (city.isEmpty()) return;

        std::ifstream file(".env");

        std::string strapi;
        file >> strapi;

        QString apiKey("eedcd5f107823e8b8e30ef4e3cce6e04");

        QString url = QString("https://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2&units=metric&lang=ru")
                         .arg(city).arg(apiKey);

        manager->get(QNetworkRequest(QUrl(url)));

        weatherLabel->setText("Загрузка данных...");
        weatherIconLabel->clear();
    }

    void onWeatherDataReceived(QNetworkReply *reply) {
        reply->deleteLater();

        if (reply->error()) {
            weatherLabel->setText("Ошибка: " + reply->errorString());
            return;
        }

        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());

        if (jsonResponse.isNull()) {
            weatherLabel->setText("Ошибка: неверный формат данных");
            return;
        }

        QJsonObject jsonObject = jsonResponse.object();
        updateWeatherUI(jsonObject);
    }

    void onCitySelected(int index) {
        if (index >= 0) {
        }
    }


private:
    void setupUI() {
        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

        QHBoxLayout *cityLayout = new QHBoxLayout();

        cityComboBox = new QComboBox(this);
        cityComboBox->addItems({"Москва", "Санкт-Петербург", "Новосибирск", "Екатеринбург", "Казань"});
        cityComboBox->setEditable(true);

        cityInput = new QLineEdit(this);
        cityInput->setPlaceholderText("Введите город");

        searchButton = new QPushButton(this);
        searchButton->setIcon(QIcon(":icons/search.png"));
        searchButton->setIconSize(QSize(24, 24));
        searchButton->setText("");
        searchButton->setFixedSize(32, 32);
        searchButton->setStyleSheet(
            "QPushButton { border: none; background-color: transparent; }"
            "QPushButton:hover { background-color: #e0e0e0; border-radius: 16px; }"
        );
        searchButton->setToolTip("Search weather");

        cityLayout->addWidget(cityComboBox);
        cityLayout->addWidget(cityInput);
        cityLayout->addWidget(searchButton);

        weatherIconLabel = new QLabel(this);
        weatherIconLabel->setAlignment(Qt::AlignCenter);

        weatherLabel = new QLabel(this);
        weatherLabel->setAlignment(Qt::AlignCenter);
        weatherLabel->setStyleSheet("font-size: 16px;");

        mainLayout->addLayout(cityLayout);
        mainLayout->addWidget(weatherIconLabel);
        mainLayout->addWidget(weatherLabel);
        mainLayout->addStretch();

        centralWidget->setLayout(mainLayout);
        setCentralWidget(centralWidget);

        setWindowIcon(QIcon(":icons/app_icon.png"));

        setWindowTitle("Прогноз погоды");
        resize(400, 400);
    }


    void setupConnections() {
        connect(searchButton, &QPushButton::clicked, this, &WeatherApp::searchWeather);
        connect(cityComboBox, QOverload<int>::of(&QComboBox::activated), this, &WeatherApp::onCitySelected);
    }

    void updateWeatherUI(const QJsonObject &jsonObject) {
        QString city = jsonObject["name"].toString();
        double temp = jsonObject["main"].toObject()["temp"].toDouble();
        int humidity = jsonObject["main"].toObject()["humidity"].toInt();
        double windSpeed = jsonObject["wind"].toObject()["speed"].toDouble();
        QString description = jsonObject["weather"].toArray()[0].toObject()["description"].toString();
        QString iconCode = jsonObject["weather"].toArray()[0].toObject()["icon"].toString();

        QString iconUrl = QString("https://openweathermap.org/img/wn/%1@2x.png").arg(iconCode);
        QNetworkRequest iconRequest(iconUrl);
        QString weatherText = QString(
            "<h2>%1</h2>"
            "<p><b>Температура:</b> %2°C</p>"
            "<p><b>Влажность:</b> %3%</p>"
            "<p><b>Ветер:</b> %4 м/с</p>"
            "<p><b>Состояние:</b> %5</p>"
        ).arg(city).arg(temp).arg(humidity).arg(windSpeed).arg(description);

        weatherLabel->setText(weatherText);
    }

    QComboBox *cityComboBox;
    QLineEdit *cityInput;
    QPushButton *searchButton;
    QLabel *weatherIconLabel;
    QLabel *weatherLabel;
    QNetworkAccessManager *manager;

};
