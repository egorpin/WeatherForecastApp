#include "MainWindow.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QToolButton>
#include <QLabel>
#include <QIcon>
#include <QPixmap>
#include <QStyle>
#include <QSettings>
#include <QActionGroup>
#include <QMenu>
#include <QApplication>
#include <QCursor>

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
    setStyleSheet("background-color: rgba(172, 172, 172, 0.78);");

    QSettings settings;
    int bgIndex = settings.value("background", 0).toInt();
    if (bgIndex > 0) {
        QAction* action = bgActionGroup->actions().at(bgIndex);
        action->setChecked(true);
        changeBackground(action);
    }

    m_isResizing = false;
}

void MainWindow::setupTitleBar() {
    titleBar = new QWidget(this);
    titleBar->setFixedHeight(30);
    titleBar->setStyleSheet("background-color: rgb(28, 41, 80);");

    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(5, 0, 5, 0);
    titleLayout->setSpacing(5);

    titleIcon = new QLabel(titleBar);
    QPixmap appPixmap(":/MainWindow/icons/app_icon.png");
    if(!appPixmap.isNull()) {
        titleIcon->setPixmap(appPixmap.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        qDebug() << "Юра епта ну где значок приложения";
        titleIcon->setText("App");
    }
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
            maximizeButton->setIcon(QIcon(":/MainWindow/icons/maximize.png"));
            maximizeButton->setToolTip("Развернуть");
        } else {
            // Сохраняем текущее положение перед максимизацией
            m_normalGeometry = geometry();
            showMaximized();
            maximizeButton->setIcon(QIcon(":/MainWindow/icons/restore.png"));
            maximizeButton->setToolTip("Восстановить");
        }
        update(); // Принудительная перерисовка
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

    backgroundMenu = new QMenu("Фон", this);
    bgActionGroup = new QActionGroup(this);

    QStringList backgrounds = {"default", "bg1", "bg2", "bg3"};
    for (int i = 0; i < backgrounds.size(); ++i) {
        QAction* action = new QAction(backgrounds[i], this);
        action->setCheckable(true);
        action->setChecked(i == 0);
        action->setData(i);
        bgActionGroup->addAction(action);
        backgroundMenu->addAction(action);
    }

    connect(bgActionGroup, &QActionGroup::triggered, this, &MainWindow::changeBackground);

    QToolButton* menuButton = new QToolButton(titleBar);
    menuButton->setIcon(QIcon(":MainWindow/icons/menu.png"));
    menuButton->setMenu(backgroundMenu);
    menuButton->setPopupMode(QToolButton::InstantPopup);
    titleLayout->insertWidget(2, menuButton);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    if (bgActionGroup->checkedAction()->data().toInt() != 0) {
        QPalette palette = this->palette();
        QString currentBgIndex = QString::number(bgActionGroup->checkedAction()->data().toInt());
        QString imagePath = QString(":/MainWindow/resources/backgrounds/bg%1.jpg").arg(currentBgIndex);
        QPixmap bg(imagePath);
        if (!bg.isNull()) {
            palette.setBrush(backgroundRole(), QBrush(bg.scaled(size(), Qt::KeepAspectRatioByExpanding)));
            setPalette(palette);
        } else {
            qWarning() << "Background image not found or failed to load:" << imagePath;
            setStyleSheet("background-color: rgba(0, 168, 219, 0.78);");
        }
    } else {
        setStyleSheet("background-color: rgba(0, 168, 219, 0.78);");
    }
}

void MainWindow::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);

    if (event->spontaneous()) {
        QScreen *screen = QGuiApplication::primaryScreen();
        if (screen) {
            QRect screenGeometry = screen->availableGeometry();
            move((screenGeometry.width() - width()) / 2, (screenGeometry.height() - height()) / 2);
        }
    }
}

void MainWindow::changeBackground(QAction* action) {
    int index = action->data().toInt();

    if (index == 0) {
        setStyleSheet("background-color: rgba(0, 168, 219, 0.78);");
        setPalette(QPalette()); // Сбрасываем палитру
    } else {
        QString imagePath = QString(":/MainWindow/resources/backgrounds/bg%1.jpg").arg(index);
        QPixmap bg(imagePath);
        if (!bg.isNull()) {
            QPalette palette;
            palette.setBrush(backgroundRole(), QBrush(bg.scaled(size(), Qt::KeepAspectRatioByExpanding)));
            setPalette(palette);
            setStyleSheet(""); // Сбрасываем стиль
        } else {
            qWarning() << "Failed to load background:" << imagePath;
            setStyleSheet("background-color: rgba(0, 168, 219, 0.78);");
        }
    }

    QSettings settings;
    settings.setValue("background", index);
    update();
    repaint();
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        const int borderWidth = 10;
        bool isNearLeftEdge = event->position().x() <= borderWidth;
        bool isNearRightEdge = event->position().x() >= width() - borderWidth;
        bool isNearTopEdge = event->position().y() <= borderWidth;
        bool isNearBottomEdge = event->position().y() >= height() - borderWidth;

        if ((isNearLeftEdge || isNearRightEdge) && (isNearTopEdge || isNearBottomEdge)) {
            if (isNearLeftEdge && isNearTopEdge) {
                m_resizeEdge = Qt::TopEdge | Qt::LeftEdge;
            } else if (isNearRightEdge && isNearTopEdge) {
                m_resizeEdge = Qt::TopEdge | Qt::RightEdge;
            } else if (isNearLeftEdge && isNearBottomEdge) {
                m_resizeEdge = Qt::BottomEdge | Qt::LeftEdge;
            } else if (isNearRightEdge && isNearBottomEdge) {
                m_resizeEdge = Qt::BottomEdge | Qt::RightEdge;
            }
            m_isResizing = true;
            m_resizeStartPosition = event->globalPos();
            event->accept();
            return;
        } else if (isNearLeftEdge || isNearRightEdge || isNearTopEdge || isNearBottomEdge) {
            if (isNearLeftEdge) {
                m_resizeEdge = Qt::LeftEdge;
            } else if (isNearRightEdge) {
                m_resizeEdge = Qt::RightEdge;
            } else if (isNearTopEdge) {
                m_resizeEdge = Qt::TopEdge;
            } else if (isNearBottomEdge) {
                m_resizeEdge = Qt::BottomEdge;
            }
            m_isResizing = true;
            m_resizeStartPosition = event->globalPos();
            event->accept();
            return;
        }

        if (event->position().y() < titleBar->height()) {
            m_isDragging = true;
            m_dragStartPosition = event->globalPos();
            m_windowStartPosition = this->pos();
            event->accept();
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (m_isDragging) {
        QPoint delta = event->globalPos() - m_dragStartPosition;
        QPoint newPos = m_windowStartPosition + delta;
        move(newPos);
        event->accept();
    } else if (m_isResizing) {
        QPoint delta = event->globalPos() - m_resizeStartPosition;
        QRect newGeometry = geometry();

        if (m_resizeEdge.testFlag(Qt::LeftEdge)) {
            newGeometry.adjust(delta.x(), 0, 0, 0);
        }
        if (m_resizeEdge.testFlag(Qt::RightEdge)) {
            newGeometry.adjust(0, 0, delta.x(), 0);
        }
        if (m_resizeEdge.testFlag(Qt::TopEdge)) {
            newGeometry.adjust(0, delta.y(), 0, 0);
        }
        if (m_resizeEdge.testFlag(Qt::BottomEdge)) {
            newGeometry.adjust(0, 0, 0, delta.y());
        }

        setGeometry(newGeometry);
        m_resizeStartPosition = event->globalPos();
        event->accept();
    } else {
        const int borderWidth = 10;
        bool isNearLeftEdge = event->position().x() <= borderWidth;
        bool isNearRightEdge = event->position().x() >= width() - borderWidth;
        bool isNearTopEdge = event->position().y() <= borderWidth;
        bool isNearBottomEdge = event->position().y() >= height() - borderWidth;

        if (isNearLeftEdge && isNearTopEdge) {
            setCursor(Qt::SizeFDiagCursor);
        } else if (isNearRightEdge && isNearTopEdge) {
            setCursor(Qt::SizeBDiagCursor);
        } else if (isNearLeftEdge && isNearBottomEdge) {
            setCursor(Qt::SizeBDiagCursor);
        } else if (isNearRightEdge && isNearBottomEdge) {
            setCursor(Qt::SizeFDiagCursor);
        } else if (isNearLeftEdge || isNearRightEdge) {
            setCursor(Qt::SizeHorCursor);
        } else if (isNearTopEdge || isNearBottomEdge) {
            setCursor(Qt::SizeVerCursor);
        } else {
            unsetCursor();
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
        m_isResizing = false;
        unsetCursor();
        event->accept();
    }
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && titleBar->geometry().contains(event->position().toPoint())) {
        maximizeButton->click();
        event->accept();
    }
}
