#include "DeviceStore.h"
#include <QDateTime>
#include <QUuid>

DeviceStore::DeviceStore(QObject *parent) : QObject(parent) {}

QVector<QJsonObject> DeviceStore::all() const
{
    QVector<QJsonObject> result;
    result.reserve(m_devices.size());
    for (auto it = m_devices.cbegin(); it != m_devices.cend(); ++it)
        result.push_back(it.value());
    return result;
}

QJsonObject DeviceStore::makeEvent(const QString &name, const QJsonObject &data) const
{
    return {
        {"eventId", QUuid::createUuid().toString(QUuid::WithoutBraces)},
        {"event", name},
        {"timestamp", QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs)},
        {"data", data}
    };
}

QJsonObject DeviceStore::create(const QJsonObject &input)
{
    const QString name = input.value("name").toString().trimmed();
    if (name.isEmpty()) return {};
    const QString id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    const QString now = QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs);
    QJsonObject device {
        {"id", id}, {"name", name},
        {"type", input.value("type").toString("generic")},
        {"source", input.value("source").toString("virtual")},
        {"status", "offline"}, {"createdAt", now}, {"updatedAt", now}
    };
    m_devices.insert(id, device);
    emit eventPublished(makeEvent("device.created", {{"device", device}}));
    return device;
}

QJsonObject DeviceStore::update(const QString &id, const QJsonObject &input)
{
    if (!m_devices.contains(id)) return {};
    QJsonObject device = m_devices.value(id);
    if (input.contains("name")) {
        const QString name = input.value("name").toString().trimmed();
        if (name.isEmpty()) return {};
        device.insert("name", name);
    }
    if (input.contains("type")) device.insert("type", input.value("type"));
    device.insert("updatedAt", QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs));
    m_devices.insert(id, device);
    emit eventPublished(makeEvent("device.updated", {{"device", device}}));
    return device;
}

bool DeviceStore::remove(const QString &id)
{
    if (!m_devices.contains(id)) return false;
    const QJsonObject removed = m_devices.take(id);
    emit eventPublished(makeEvent("device.deleted", {{"deviceId", id}, {"device", removed}}));
    return true;
}
