#include "WeatherAPI/WeatherAPI.hpp"
#include "MainWindow/MainWindow.hpp"
#include "Utilities/Geolocation.hpp"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QDir>

int main(int argc, char *argv[]) {
    #ifdef Q_OS_LINUX
    qputenv("QT_QPA_PLATFORM", "xcb");
    #endif

    QApplication app(argc, argv);
    app.setApplicationName("WeatherApp");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("YourCompany");

    qDebug() << "Available resource paths:" << QDir(":/").entryList();
    qDebug() << "Styles available:" << QDir(":/styles/").entryList();

    QString stylePath = "MainWindow/resources/styles/backgrounds.qss";
    QFile styleFile(stylePath);

    if (!styleFile.exists()) {
        qCritical() << "Stylesheet file not found at:" << stylePath;
        qDebug() << "Current working directory:" << QDir::currentPath();
    } else if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        app.setStyleSheet(styleSheet);
        styleFile.close();
        qDebug() << "Stylesheet loaded successfully from:" << stylePath;
    } else {
        qCritical() << "Failed to open stylesheet:" << styleFile.errorString();
    }
    try {
        MainWindow window;
        window.show();



        #ifdef QT_DEBUG
        qDebug() << "Checking API connectivity...";
        WeatherAPI api;
        api.request("London");
        #endif

        return app.exec();
    } catch (const std::exception& e) {
        qCritical() << "Application error:" << e.what();
        return EXIT_FAILURE;
    }
}
