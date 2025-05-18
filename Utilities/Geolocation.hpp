#pragma once

#include <QObject>
#include <QGeoPositionInfoSource>
#include <QGeoPositionInfo>
#include <QDebug>
#include <QTimer>
#include <QCoreApplication>

class Geolocation : public QObject
{
    Q_OBJECT

public:
    explicit Geolocation(QObject *parent = nullptr);
    ~Geolocation() = default;

private slots:
    void positionUpdated(const QGeoPositionInfo &info);
    void positionTimeout();

private:
    QGeoPositionInfoSource *m_source;
};
