#pragma once

#include <QMainWindow>
#include "WeatherView.hpp"
#include <QToolButton>
#include <QLabel>
#include <QMouseEvent> 

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget *parent = nullptr);
    
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    
private:
    void setupTitleBar();
    
    QPoint dragPosition;
    QWidget *titleBar;
    QLabel *titleIcon;
    QLabel *titleLabel;
    QToolButton *minimizeButton;
    QToolButton *maximizeButton;
    QToolButton *closeButton;
    WeatherView *view;
};