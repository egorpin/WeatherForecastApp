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
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_blurEffect(new QGraphicsBlurEffect(this)),
      backgroundLabel(new QLabel(this))
{
    setWindowFlags(Qt::FramelessWindowHint);

    backgroundLabel->setScaledContents(true);
    backgroundLabel->setGraphicsEffect(m_blurEffect);
    m_blurEffect->setBlurRadius(15);
    backgroundLabel->lower();

    setupTitleBar();

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setAttribute(Qt::WA_TranslucentBackground);

    view = new WeatherView(centralWidget);
    view->setAttribute(Qt::WA_TranslucentBackground);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(titleBar);
    mainLayout->addWidget(view, 1);

    setCentralWidget(centralWidget);
    setWindowIcon(QIcon(":MainWindow/icons/app_icon.png"));

    setBackgroundImage(":/MainWindow/resources/backgrounds/clear.jpg");

    connect(view, &WeatherView::weatherUpdated,
            this, &MainWindow::handleWeatherUpdate);
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
}

void MainWindow::handleWeatherUpdate(WeatherObject *wobj) {
    if (wobj && wobj->IsValid()) {
        applyWeatherBackground(wobj->weatherId);
    }
    delete wobj;
}

void MainWindow::showEvent(QShowEvent* event) {
    QMainWindow::showEvent(event);
    if (event->spontaneous()) {
        QScreen* screen = QGuiApplication::primaryScreen();
        if (screen) {
            QRect screenGeometry = screen->availableGeometry();
            move((screenGeometry.width() - width()) / 2,
                 (screenGeometry.height() - height()) / 2);
        }
    }
}

void MainWindow::applyWeatherBackground(int weatherId) {
    int backgroundId = 800; // По умолчанию ясно

    if (weatherId >= 200 && weatherId < 300) backgroundId = 200; // Гроза
    else if (weatherId >= 300 && weatherId < 400) backgroundId = 300; // Морось
    else if (weatherId >= 500 && weatherId < 600) backgroundId = weatherId < 512 ? 500 : 511; // Дождь
    else if (weatherId >= 600 && weatherId < 700) backgroundId = 600; // Снег
    else if (weatherId >= 700 && weatherId < 800) backgroundId = 700; // Туман и т.д.
    else if (weatherId >= 800) backgroundId = qMin(weatherId, 804); // Облака

    setBackgroundImage(weatherBackgrounds.value(backgroundId,
                       ":/MainWindow/resources/backgrounds/clear.jpg"));
}

void MainWindow::setBackgroundImage(const QString &imagePath) {
    QPixmap bg(imagePath);
    if (!bg.isNull()) {
        QPalette palette;
        palette.setBrush(backgroundRole(), QBrush(bg.scaled(size(), Qt::KeepAspectRatioByExpanding)));
        setPalette(palette);
    } else {
        qWarning() << "Failed to load background image:" << imagePath;
        setStyleSheet("background-color: rgba(0, 168, 219, 0.78);");
    }
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    QPixmap currentBg = palette().brush(backgroundRole()).texture();
    if (!currentBg.isNull()) {
        QPalette palette;
        palette.setBrush(backgroundRole(),
                        QBrush(currentBg.scaled(size(), Qt::KeepAspectRatioByExpanding)));
        setPalette(palette);
    }
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
