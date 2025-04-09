#include "WeatherAPI/WeatherAPI.hpp"
#include "MainWindow/MainWindow.hpp"
#include "Utilities/filedownloader.hpp"
#include <QApplication>

int main(int argc, char *argv[]) {
    FileDownloader* fd = new FileDownloader(QUrl("Asd"));

    QApplication app(argc, argv);
    app.setStyle("Fusion");

    MainWindow window;
    window.show();

    return app.exec();
}
