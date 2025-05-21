#include "WeatherAPI/WeatherAPI.hpp"
#include "MainWindow/MainWindow.hpp"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QDir>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("WeatherApp");

    QString stylePath = "MainWindow/resources/styles/backgrounds.qss";
    QFile styleFile(stylePath);

    QString styleSheet = QLatin1String(styleFile.readAll());
    app.setStyleSheet(styleSheet);
    styleFile.close();

    MainWindow window;
    window.show();

    return app.exec();
}
