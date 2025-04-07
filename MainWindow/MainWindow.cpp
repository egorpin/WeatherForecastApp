#include "MainWindow.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStyle>
#include <QIcon>
#include <QPixmap>
#include <QToolButton>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QMouseEvent>

// Стили для кнопок
const QString windowButtonStyle = R"(
    QToolButton {
        border: none;
        background: transparent;
        padding: 4px;
        border-radius: 8px;
        opacity: 0.9;
    }
    QToolButton:hover {
        background: rgba(255, 255, 255, 0.2);
        opacity: 1.0;
    }
    QToolButton:pressed {
        background: rgba(255, 255, 255, 0.3);
    }
)";

const QString closeButtonStyle = R"(
    QToolButton {
        border: none;
        background: transparent;
        padding: 4px;
        border-radius: 8px;
        opacity: 0.9;
    }
    QToolButton:hover {
        background: #e81123;
        opacity: 1.0;
    }
    QToolButton:pressed {
        background: #f1707a;
    }
)";

MainWindow::MainWindow(QWidget *parent) 
    : QMainWindow(parent)
{
    // Убираем стандартную рамку окна
    setWindowFlags(Qt::FramelessWindowHint);
    
    // Создаем кастомную панель заголовка
    setupTitleBar();
    
    // Основное содержимое
    view = new WeatherView(this);
    
    // Компоновка
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    mainLayout->addWidget(titleBar);
    mainLayout->addWidget(view, 1);
    
    setCentralWidget(centralWidget);
    setWindowIcon(QIcon(":MainWindow/icons/app_icon.png"));
    resize(400, 500);
    
    view->showLoadingIndicator();
}

void MainWindow::setupTitleBar()
{
    titleBar = new QWidget(this);
    titleBar->setFixedHeight(30);
    titleBar->setStyleSheet("background-color: #2b2b2b;");
    
    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(5, 0, 5, 0);
    titleLayout->setSpacing(5);
    
    // Иконка приложения
    titleIcon = new QLabel(titleBar);
    titleIcon->setPixmap(QPixmap(":MainWindow/icons/app_icon.png").scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    titleLayout->addWidget(titleIcon);
    
    // Название приложения
    titleLabel = new QLabel("Прогноз погоды", titleBar);
    titleLabel->setStyleSheet("color: white; font-weight: bold;");
    titleLayout->addWidget(titleLabel);
    
    titleLayout->addStretch();
    
    // Кнопка сворачивания
    minimizeButton = new QToolButton(titleBar);
    minimizeButton->setIcon(QIcon(":MainWindow/icons/minimize.svg"));
    minimizeButton->setIconSize(QSize(16, 16));
    minimizeButton->setStyleSheet(windowButtonStyle);
    minimizeButton->setToolTip("Свернуть");
    connect(minimizeButton, &QToolButton::clicked, this, &QMainWindow::showMinimized);
    
    // Кнопка разворачивания/восстановления
    maximizeButton = new QToolButton(titleBar);
    maximizeButton->setIcon(QIcon(":MainWindow/icons/maximize.svg"));
    maximizeButton->setIconSize(QSize(16, 16));
    maximizeButton->setStyleSheet(windowButtonStyle);
    maximizeButton->setToolTip("Развернуть");
    connect(maximizeButton, &QToolButton::clicked, [this]() {
        if (isMaximized()) {
            showNormal();
            maximizeButton->setIcon(QIcon(":MainWindow/icons/maximize.svg"));
            maximizeButton->setToolTip("Развернуть");
        } else {
            showMaximized();
            maximizeButton->setIcon(QIcon(":MainWindow/icons/restore.svg"));
            maximizeButton->setToolTip("Восстановить");
        }
    });
    
    // Кнопка закрытия
    closeButton = new QToolButton(titleBar);
    closeButton->setIcon(QIcon(":MainWindow/icons/close.svg"));
    closeButton->setIconSize(QSize(16, 16));
    closeButton->setStyleSheet(closeButtonStyle);
    closeButton->setToolTip("Закрыть");
    connect(closeButton, &QToolButton::clicked, this, &QMainWindow::close);
    
    // Добавляем эффекты анимации
    auto addHoverAnimation = [](QToolButton* button) {
        QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(button);
        effect->setOpacity(0.9);
        button->setGraphicsEffect(effect);
        
        QPropertyAnimation* anim = new QPropertyAnimation(effect, "opacity", button);
        anim->setDuration(150);
        anim->setStartValue(0.7);
        anim->setEndValue(1.0);
    
    };
    
    addHoverAnimation(minimizeButton);
    addHoverAnimation(maximizeButton);
    addHoverAnimation(closeButton);
    
    titleLayout->addWidget(minimizeButton);
    titleLayout->addWidget(maximizeButton);
    titleLayout->addWidget(closeButton);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && titleBar->geometry().contains(event->position().toPoint())) {
        dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - dragPosition);
        event->accept();
    }
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && 
        titleBar->geometry().contains(event->position().toPoint())) {
        maximizeButton->click();
        event->accept();
    }
}