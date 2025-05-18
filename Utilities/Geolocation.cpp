#include "Geolocation.hpp"

Geolocation::Geolocation(QObject *parent) : QObject(parent)
{
    // Создаем источник информации о местоположении
    m_source = QGeoPositionInfoSource::createDefaultSource(this);

    if (!m_source) {
        qWarning() << "Не удалось создать источник геолокации. Возможно, на устройстве нет поддержки GPS.";
        QCoreApplication::exit(1);
        return;
    }

    // Подключаем сигналы
    connect(m_source, &QGeoPositionInfoSource::positionUpdated, this, &Geolocation::positionUpdated);
    connect(m_source, &QGeoPositionInfoSource::errorOccurred, this, &Geolocation::positionTimeout);

    // Устанавливаем интервал обновления (в миллисекундах)
    m_source->setUpdateInterval(1000);

    // Запускаем обновление
    m_source->startUpdates();

    // Таймер для завершения приложения, если не получим данные
    QTimer::singleShot(30000, this, []() {
        qWarning() << "Тайм-аут при получении геолокации.";
        QCoreApplication::exit(1);
    });
}

void Geolocation::positionUpdated(const QGeoPositionInfo &info)
{
    if (info.isValid()) {
        qDebug() << "Геолокация получена:";
        qDebug() << "Широта:" << info.coordinate().latitude();
        qDebug() << "Долгота:" << info.coordinate().longitude();
        qDebug() << "Высота:" << info.coordinate().altitude() << "метров";
        qDebug() << "Точность:" << info.attribute(QGeoPositionInfo::HorizontalAccuracy) << "метров";
        qDebug() << "Время:" << info.timestamp().toString(Qt::ISODate);

        QCoreApplication::exit(0);
    } else {
        qWarning() << "Получены невалидные данные геолокации";
    }
}

void Geolocation::positionTimeout()
{
    qWarning() << "Тайм-аут при попытке получить геолокацию";
}
