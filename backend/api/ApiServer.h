#pragma once
#include "core/DeviceStore.h"
#include <QHttpServer>
#include <QObject>
#include <QTcpServer>
#include <QWebSocketServer>
#include <QWebSocket>

class ApiServer final : public QObject
{
    Q_OBJECT
public:
    explicit ApiServer(QObject *parent = nullptr);
    bool start(quint16 httpPort = 8080, quint16 webSocketPort = 8081);
private:
    QHttpServer m_httpServer;
    QTcpServer m_tcpServer;
    QWebSocketServer m_webSocketServer;
    DeviceStore m_store;
    QJsonObject parseObject(const QByteArray &body) const;
    QHttpServerResponse jsonResponse(const QJsonObject &object) const;
    QHttpServerResponse errorResponse(const QString &message) const;
    void publishToClients(const QJsonObject &event);
    void sendSnapshot(QWebSocket *socket);
};
