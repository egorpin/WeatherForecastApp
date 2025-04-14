#include "MainWindow.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QToolButton>
#include <QLabel>
#include <QIcon>
#include <QPixmap>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowFlags(Qt::FramelessWindowHint);
    setupTitleBar();
    
    view = new WeatherView(this);
    
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    mainLayout->addWidget(titleBar);
    mainLayout->addWidget(view, 1);
    
    setCentralWidget(centralWidget);
    setWindowIcon(QIcon(":MainWindow/icons/app_icon.png"));
    setStyleSheet("background-color: rgba(0, 168, 219, 0.78);");
}

void MainWindow::setupTitleBar() {
    titleBar = new QWidget(this);
    titleBar->setFixedHeight(30);
    titleBar->setStyleSheet("background-color: rgb(28, 41, 80);");

    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(5, 0, 5, 0);
    titleLayout->setSpacing(5);

    titleIcon = new QLabel(titleBar);
    titleIcon->setPixmap(QPixmap(":MainWindow/icons/app_icon.png").scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    titleLayout->addWidget(titleIcon);

    titleLabel = new QLabel("Прогноз погоды", titleBar);
    titleLabel->setStyleSheet("color: white; font-weight: bold;");
    titleLayout->addWidget(titleLabel);

    titleLayout->addStretch();

    minimizeButton = new QToolButton(titleBar);
    minimizeButton->setIcon(QIcon(":MainWindow/icons/minimize.png"));
    minimizeButton->setIconSize(QSize(15, 15));
    minimizeButton->setStyleSheet(windowButtonStyle);
    minimizeButton->setToolTip("Свернуть");
    connect(minimizeButton, &QToolButton::clicked, this, &QMainWindow::showMinimized);

    maximizeButton = new QToolButton(titleBar);
    maximizeButton->setIcon(QIcon(":MainWindow/icons/maximize.png"));
    maximizeButton->setIconSize(QSize(15, 15));
    maximizeButton->setStyleSheet(windowButtonStyle);
    maximizeButton->setToolTip("Развернуть");
    connect(maximizeButton, &QToolButton::clicked, [this]() {
        if (isMaximized()) {
            showNormal();
            maximizeButton->setIcon(QIcon(":MainWindow/icons/maximize.png"));
            maximizeButton->setToolTip("Развернуть");
        } else {
            showMaximized();
            maximizeButton->setIcon(QIcon(":MainWindow/icons/restore.png"));
            maximizeButton->setToolTip("Восстановить");
        }
    });

    closeButton = new QToolButton(titleBar);
    closeButton->setIcon(QIcon(":MainWindow/icons/close.png"));
    closeButton->setIconSize(QSize(15, 15));
    closeButton->setStyleSheet(closeButtonStyle);
    closeButton->setToolTip("Закрыть");
    connect(closeButton, &QToolButton::clicked, this, &QMainWindow::close);

    titleLayout->addWidget(minimizeButton);
    titleLayout->addWidget(maximizeButton);
    titleLayout->addWidget(closeButton);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && event->position().toPoint().y() < 40) {
        m_isDragging = true;
        m_dragStartPosition = event->globalPosition().toPoint();
        m_windowStartPosition = this->pos();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (m_isDragging) {
        QPoint delta = event->globalPosition().toPoint() - m_dragStartPosition;
        QPoint newPos = m_windowStartPosition + delta;
        move(newPos);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
        event->accept();
    }
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && titleBar->geometry().contains(event->position().toPoint())) {
        maximizeButton->click();
        event->accept();
    }
}