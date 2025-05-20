#pragma once

#include <QMainWindow>
#include "WeatherView.hpp"
#include "button.qcss"

#include "style.qcss"

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
    void handleWeatherUpdate(WeatherObject *wobj);
    void applyWeatherBackground(int weatherId);

private:
    void setBlurEffectEnabled(bool enabled);
    void setupTitleBar();
    void setBackgroundImage(const QString &imagePath);
    void updateBlurBackground();
    void cleanBlurResources();

    const QMap<int, QString> weatherBackgrounds = {
    // Дождь
    {300, ":/MainWindow/resources/backgrounds/drizzle.jpg"},
    {500, ":/MainWindow/resources/backgrounds/rain.jpg"},
    {511, ":/MainWindow/resources/backgrounds/rain_snow.jpg"},
    // Снег
    {600, ":/MainWindow/resources/backgrounds/snow.jpg"},
    // Туман
    {700, ":/MainWindow/resources/backgrounds/mist.jpg"},
    {711, ":/MainWindow/resources/backgrounds/mist.jpg"},
    {721, ":/MainWindow/resources/backgrounds/mist.jpg"},
    {731, ":/MainWindow/resources/backgrounds/mist.jpg"},
    {741, ":/MainWindow/resources/backgrounds/mist.jpg"},
    // Ясная погода
    {800, ":/MainWindow/resources/backgrounds/clear.jpg"},
    // Облака
    {801, ":/MainWindow/resources/backgrounds/few_clouds.jpg"},
    {802, ":/MainWindow/resources/backgrounds/scattered_clouds.jpg"},
    {803, ":/MainWindow/resources/backgrounds/broken_clouds.jpg"},
    {804, ":/MainWindow/resources/backgrounds/overcast.jpg"},
    {200, ":/MainWindow/resources/backgrounds/thunderstorm.jpg"},
    {201, ":/MainWindow/resources/backgrounds/thunderstorm.jpg"},
    {202, ":/MainWindow/resources/backgrounds/thunderstorm.jpg"}
    };


    bool m_isDragging = false;
    bool m_isResizing = false;

    QPoint m_dragStartPosition;
    QPoint m_resizeStartPosition;
    QPoint m_windowStartPosition;
    QFlags<Qt::Edge> m_resizeEdge;
    QRect m_normalGeometry;
    // QMenu* backgroundMenu;
    // QActionGroup* bgActionGroup;
    QString windowButtonStyle;
    QString closeButtonStyle;
    QWidget *titleBar;
    QLabel *titleIcon;
    QLabel *titleLabel;
    QLabel *backgroundLabel;
    QToolButton *minimizeButton;
    QToolButton *maximizeButton;
    QToolButton *closeButton;
    QGraphicsBlurEffect* m_blurEffect;
    WeatherView *view;
};
