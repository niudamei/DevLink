#pragma once
#include <QHash>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QVector>

class DeviceStore final : public QObject
{
    Q_OBJECT
public:
    explicit DeviceStore(QObject *parent = nullptr);
    QVector<QJsonObject> all() const;
    QJsonObject create(const QJsonObject &input);
    QJsonObject update(const QString &id, const QJsonObject &input);
    bool remove(const QString &id);
signals:
    void eventPublished(const QJsonObject &event);
private:
    QJsonObject makeEvent(const QString &name, const QJsonObject &data) const;
    QHash<QString, QJsonObject> m_devices;
};
