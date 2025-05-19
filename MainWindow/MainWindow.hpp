#pragma once

#include <QMainWindow>
#include "WeatherView.hpp"
#include "button.qcss"

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private slots:
    void changeBackground(QAction *action);

private:
    void setupTitleBar();
    void setupBackgroundMenu();

    bool m_isDragging = false;
    bool m_isResizing = false;

    QPoint m_dragStartPosition;
    QPoint m_resizeStartPosition;
    QPoint m_windowStartPosition;
    QFlags<Qt::Edge> m_resizeEdge;
    QRect m_normalGeometry;
    QMenu* backgroundMenu;
    QActionGroup* bgActionGroup;

    QWidget *titleBar;
    QLabel *titleIcon;
    QLabel *titleLabel;
    QToolButton *minimizeButton;
    QToolButton *maximizeButton;
    QToolButton *closeButton;
    WeatherView *view;
};
