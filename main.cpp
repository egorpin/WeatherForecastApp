#include "MainWindow/MainWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Установка стиля приложения
    app.setStyle("Fusion");
    app.setWindowIcon(QIcon(":icons/app_icon.png"));

    WeatherApp window;
    window.show();

    return app.exec();
}
