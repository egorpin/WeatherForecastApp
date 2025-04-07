#include "WeatherAPI/WeatherAPI.hpp"
#include "MainWindow/MainWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    WeatherAPI api;

    api.request();

    return app.exec();
}
