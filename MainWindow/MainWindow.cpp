#include "MainWindow.hpp"
#include <QIcon>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    view = new WeatherView(this);
   // service = new WeatherService(this);
    
    setCentralWidget(view);
    setWindowIcon(QIcon(":MainWindow/icons/app_icon.png"));
    setWindowTitle("Прогноз погоды");
    resize(400, 400);
    
   // setupConnections();
    view->showLoadingIndicator();
   // service->fetchWeather(view->getSelectedCity());
}

/* void MainWindow::setupConnections() {
    connect(view, &WeatherView::citySearchRequested, 
            service, &WeatherService::fetchWeather);
    
    connect(service, &WeatherService::weatherDataReceived,
            view, &WeatherView::displayWeather);
            
    connect(service, &WeatherService::weatherError,
            view, &WeatherView::showErrorMessage);
}
            */