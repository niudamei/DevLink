#include "ApiServer.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QHostAddress>

ApiServer::ApiServer(QObject *parent)
    : QObject(parent),
      m_webSocketServer(QStringLiteral("DevLink WebSocket"), QWebSocketServer::NonSecureMode, this),
      m_store(this)
{
    connect(&m_store, &DeviceStore::eventPublished, this, &ApiServer::publishToClients);

    m_httpServer.route("/api/v1/health", [this]() {
        return jsonResponse({{"status", "ok"}, {"service", "devlink-server"}});
    });
    m_httpServer.route("/api/v1/devices", QHttpServerRequest::Method::Get, [this]() {
        QJsonArray devices;
        for (const auto &device : m_store.all()) devices.append(device);
        return jsonResponse({{"devices", devices}});
    });
    m_httpServer.route("/api/v1/devices", QHttpServerRequest::Method::Post,
                       [this](const QHttpServerRequest &request) {
        const QJsonObject device = m_store.create(parseObject(request.body()));
        if (device.isEmpty()) return errorResponse("A non-empty device name is required.");
        return jsonResponse({{"device", device}});
    });
    m_httpServer.route("/api/v1/devices/<arg>", QHttpServerRequest::Method::Put,
                       [this](const QString &id, const QHttpServerRequest &request) {
        const QJsonObject device = m_store.update(id, parseObject(request.body()));
        if (device.isEmpty()) return errorResponse("Device not found or invalid update.");
        return jsonResponse({{"device", device}});
    });
    m_httpServer.route("/api/v1/devices/<arg>", QHttpServerRequest::Method::Delete,
                       [this](const QString &id) {
        if (!m_store.remove(id)) return errorResponse("Device not found.");
        return jsonResponse({{"deleted", true}, {"id", id}});
    });

    connect(&m_webSocketServer, &QWebSocketServer::newConnection, this, [this]() {
        QWebSocket *socket = m_webSocketServer.nextPendingConnection();
        connect(socket, &QWebSocket::disconnected, socket, &QObject::deleteLater);
        socket->sendTextMessage(QString::fromUtf8(QJsonDocument(QJsonObject{
            {"eventId", "connection"}, {"event", "connection.ready"},
            {"data", QJsonObject{{"service", "devlink-server"}}}
        }).toJson(QJsonDocument::Compact)));
        sendSnapshot(socket);
    });
}

QJsonObject ApiServer::parseObject(const QByteArray &body) const
{
    QJsonParseError error;
    const QJsonDocument doc = QJsonDocument::fromJson(body, &error);
    return error.error == QJsonParseError::NoError && doc.isObject() ? doc.object() : QJsonObject{};
}
QHttpServerResponse ApiServer::jsonResponse(const QJsonObject &object) const
{
    return QHttpServerResponse("application/json", QJsonDocument(object).toJson(QJsonDocument::Compact));
}
QHttpServerResponse ApiServer::errorResponse(const QString &message) const
{
    return jsonResponse({{"error", message}});
}
void ApiServer::publishToClients(const QJsonObject &event)
{
    const QString message = QString::fromUtf8(QJsonDocument(event).toJson(QJsonDocument::Compact));
    for (QWebSocket *socket : m_webSocketServer.findChildren<QWebSocket *>())
        if (socket->state() == QAbstractSocket::ConnectedState) socket->sendTextMessage(message);
}
void ApiServer::sendSnapshot(QWebSocket *socket)
{
    QJsonArray devices;
    for (const auto &device : m_store.all()) devices.append(device);
    const QJsonObject event{{"eventId", "snapshot"}, {"event", "devices.snapshot"},
                             {"data", QJsonObject{{"devices", devices}}}};
    socket->sendTextMessage(QString::fromUtf8(QJsonDocument(event).toJson(QJsonDocument::Compact)));
}
bool ApiServer::start(quint16 httpPort, quint16 webSocketPort)
{
    if (!m_tcpServer.listen(QHostAddress::AnyIPv4, httpPort)) return false;
    if (!m_httpServer.bind(&m_tcpServer)) return false;
    return m_webSocketServer.listen(QHostAddress::AnyIPv4, webSocketPort);
}
