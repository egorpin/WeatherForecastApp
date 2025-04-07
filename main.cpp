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

class WeatherApp : public QMainWindow {
public:
    WeatherApp() : QMainWindow() {
        setupUI();
        setupConnections();

        manager = new QNetworkAccessManager(this);
        connect(manager, &QNetworkAccessManager::finished, this, &WeatherApp::onWeatherDataReceived);

         //Загрузка погоды по умолчанию
        //QTimer::singleShot(0, this, [this]() {
            searchWeather();
        //});
    }


private slots:
    void searchWeather() {
        QString city = cityInput->text().trimmed();
        if (city.isEmpty()) {
            city = cityComboBox->currentText();
        }

        if (city.isEmpty()) return;

        QString apiKey = "eedcd5f107823e8b8e30ef4e3cce6e04"; // Замените на ваш API ключ
        QString url = QString("https://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2&units=metric&lang=ru")
                         .arg(city).arg(apiKey);

        manager->get(QNetworkRequest(QUrl(url)));

        // Показываем индикатор загрузки
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
            //searchWeather();
        }
    }


private:
    void setupUI() {
        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

        // Верхняя панель с выбором города
        QHBoxLayout *cityLayout = new QHBoxLayout();

        cityComboBox = new QComboBox(this);
        cityComboBox->addItems({"Москва", "Санкт-Петербург", "Новосибирск", "Екатеринбург", "Казань"});
        cityComboBox->setEditable(true);

        cityInput = new QLineEdit(this);
        cityInput->setPlaceholderText("Введите город");

        searchButton = new QPushButton(QIcon(":/icons/search.png"), "", this);
        searchButton->setToolTip("Поиск");
        searchButton->setFixedSize(30, 30);

        cityLayout->addWidget(cityComboBox);
        cityLayout->addWidget(cityInput);
        cityLayout->addWidget(searchButton);

        // Основная область с информацией о погоде
        weatherIconLabel = new QLabel(this);
        weatherIconLabel->setAlignment(Qt::AlignCenter);

        weatherLabel = new QLabel(this);
        weatherLabel->setAlignment(Qt::AlignCenter);
        weatherLabel->setStyleSheet("font-size: 16px;");

        // Добавление элементов в основной layout
        mainLayout->addLayout(cityLayout);
        mainLayout->addWidget(weatherIconLabel);
        mainLayout->addWidget(weatherLabel);
        mainLayout->addStretch();

        centralWidget->setLayout(mainLayout);
        setCentralWidget(centralWidget);

        setWindowTitle("Прогноз погоды - Qt6");
        resize(400, 400);
    }

    void setupConnections() {
        connect(searchButton, &QPushButton::clicked, this, &WeatherApp::searchWeather);
        connect(cityComboBox, QOverload<int>::of(&QComboBox::activated), this, &WeatherApp::onCitySelected);
        //connect(cityInput, &QLineEdit::returnPressed, this, &WeatherApp::searchWeather);
    }

    void updateWeatherUI(const QJsonObject &jsonObject) {
        QString city = jsonObject["name"].toString();
        double temp = jsonObject["main"].toObject()["temp"].toDouble();
        int humidity = jsonObject["main"].toObject()["humidity"].toInt();
        double windSpeed = jsonObject["wind"].toObject()["speed"].toDouble();
        QString description = jsonObject["weather"].toArray()[0].toObject()["description"].toString();
        QString iconCode = jsonObject["weather"].toArray()[0].toObject()["icon"].toString();

        // Установка иконки погоды
        QString iconUrl = QString("https://openweathermap.org/img/wn/%1@2x.png").arg(iconCode);
        QNetworkRequest iconRequest(iconUrl);
        //QNetworkReply *iconReply = manager->get(iconRequest);
        /*connect(iconReply, &QNetworkReply::finished, this, [this, iconReply]() {
            iconReply->deleteLater();
            if (!iconReply->error()) {
                QPixmap pixmap;
                pixmap.loadFromData(iconReply->readAll());
                weatherIconLabel->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatio));
            }
        });*/

        // Форматирование текста с информацией о погоде
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

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Установка стиля приложения
    app.setStyle("Fusion");

    WeatherApp window;
    window.show();

    return app.exec();
}
