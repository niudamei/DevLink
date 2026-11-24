#include "api/ApiServer.h"
#include <QCoreApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("DevLink Server");
    QCoreApplication::setApplicationVersion("0.1.0");
    ApiServer server;
    if (!server.start()) {
        qCritical() << "Failed to start. Check ports 8080 and 8081.";
        return 1;
    }
    qInfo() << "REST API: http://0.0.0.0:8080";
    qInfo() << "WebSocket: ws://0.0.0.0:8081";
    return app.exec();
}
