#include "MainWindow.hpp"
#include "style.qcss"

#include <QApplication>
#include <QGraphicsOpacityEffect>
#include <QHBoxLayout>
#include <QIcon>
#include <QMouseEvent>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QStyle>
#include <QToolButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // Убираем стандартную рамку окна
    setWindowFlags(Qt::FramelessWindowHint);

    // Создаем кастомную панель заголовка
    setupTitleBar();

    // Основное содержимое
    view = new WeatherView(this);

    // Компоновка
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    mainLayout->addWidget(titleBar);
    mainLayout->addWidget(view, 1);

    setCentralWidget(centralWidget);
    setWindowIcon(QIcon(":MainWindow/icons/app_icon.png"));
    resize(900, 600);

    view->showLoadingIndicator();

    this->setStyleSheet("background-color: rgba(0, 168, 219, 0.78);");

    setWindowFlags(Qt::FramelessWindowHint);
}

void MainWindow::setupTitleBar() {
    titleBar = new QWidget(this);
    titleBar->setFixedHeight(30);
    titleBar->setStyleSheet("background-color:rgb(28, 41, 80);");

    QHBoxLayout* titleLayout = new QHBoxLayout(titleBar);
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
    minimizeButton->setIcon(QIcon(":MainWindow/icons/minimize.png"));
    minimizeButton->setIconSize(QSize(15, 15));
    minimizeButton->setStyleSheet(windowButtonStyle);
    minimizeButton->setToolTip("Свернуть");
    connect(minimizeButton, &QToolButton::clicked, this, &QMainWindow::showMinimized);

    // Кнопка разворачивания/восстановления
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

    // Кнопка закрытия
    closeButton = new QToolButton(titleBar);
    closeButton->setIcon(QIcon(":MainWindow/icons/close.png"));
    closeButton->setIconSize(QSize(15, 15));
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

void MainWindow::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && event->position().toPoint().y() < 40) {  // Только верхние 40 пикселей
        m_isDragging = true;
        m_dragStartPosition = event->globalPosition().toPoint();
        m_windowStartPosition = this->pos();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    if (m_isDragging) {
        QPoint delta = event->globalPosition().toPoint() - m_dragStartPosition;
        QPoint newPos = m_windowStartPosition + delta;

        // Полностью убираем ограничения по границам экрана
        move(newPos);

        // Опционально: частичное скрытие за границами
        // allowPartialHiding(newPos);

        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
        unsetCursor();  // Восстанавливаем курсор
        event->accept();

        // Опционально: притягивание к границам при отпускании
        // snapToScreenEdges();
    }
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && titleBar->geometry().contains(event->position().toPoint())) {
        maximizeButton->click();
        event->accept();
    }
}

void MainWindow::allowPartialHiding(const QPoint& newPos) {
    // Разрешаем окну частично скрываться за границами
    QRect screenGeometry = QApplication::primaryScreen()->geometry();
    QRect windowGeometry = this->geometry();

    // Проверяем, чтобы хотя бы часть окна оставалась видимой
    if (!screenGeometry.intersects(windowGeometry)) {
        // Если окно полностью за границами, немного возвращаем
        int x = qBound(screenGeometry.left() - width() + 50, newPos.x(), screenGeometry.right() - 50);
        int y = qBound(screenGeometry.top() - height() + 50, newPos.y(), screenGeometry.bottom() - 50);
        move(x, y);
    }
}
