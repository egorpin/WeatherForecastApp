#pragma once

#include <QMainWindow>
#include "WeatherView.hpp"
#include "style.qcss"

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    void setupTitleBar();
    
    bool m_isDragging = false;
    QPoint m_dragStartPosition;
    QPoint m_windowStartPosition;
    
    QWidget *titleBar;
    QLabel *titleIcon;
    QLabel *titleLabel;
    QToolButton *minimizeButton;
    QToolButton *maximizeButton;
    QToolButton *closeButton;
    WeatherView *view;
};