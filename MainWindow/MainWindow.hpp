#pragma once

#include <QMainWindow>
#include "WeatherView.hpp"


class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    //void setupConnections();

    WeatherView *view;
};
