#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkRequest>
#include <QUrl>

static const QUrl kApiBase(QStringLiteral("http://127.0.0.1:8080/api/v1/"));
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("DevLink - Device Manager");
    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::refreshDevices);
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::addDevice);
    connect(ui->renameButton, &QPushButton::clicked, this, &MainWindow::renameDevice);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::deleteDevice);
    connect(&m_socket, &QWebSocket::textMessageReceived, this, &MainWindow::handleWebSocketMessage);
    connect(&m_socket, &QWebSocket::connected, this, [this]() {
        ui->connectionLabel->setText("实时同步：已连接");
    });
    connect(&m_socket, &QWebSocket::disconnected, this, [this]() {
        ui->connectionLabel->setText("实时同步：未连接");
    });
    refreshDevices();
    m_socket.open(QUrl(QStringLiteral("ws://127.0.0.1:8081/ws")));
}
MainWindow::~MainWindow() { delete ui; }
QString MainWindow::selectedDeviceId() const
{
    const QListWidgetItem *item = ui->deviceList->currentItem();
    return item ? item->data(Qt::UserRole).toString() : QString{};
}
void MainWindow::sendRequest(const QByteArray &method, const QUrl &url, const QByteArray &body)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = nullptr;
    if (method == "POST") reply = m_network.post(request, body);
    else if (method == "PUT") reply = m_network.put(request, body);
    else if (method == "DELETE") reply = m_network.deleteResource(request);
    else reply = m_network.get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError)
            ui->connectionLabel->setText("API 错误：" + reply->errorString());
        reply->deleteLater();
        refreshDevices();
    });
}
void MainWindow::refreshDevices()
{
    QNetworkReply *reply = m_network.get(QNetworkRequest(kApiBase.resolved(QUrl("devices"))));
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        const QByteArray bytes = reply->readAll();
        if (reply->error() == QNetworkReply::NoError) {
            const QJsonArray devices = QJsonDocument::fromJson(bytes).object().value("devices").toArray();
            ui->deviceList->clear();
            for (const QJsonValue &value : devices) {
                const QJsonObject device = value.toObject();
                auto *item = new QListWidgetItem(device.value("name").toString(), ui->deviceList);
                item->setData(Qt::UserRole, device.value("id").toString());
                item->setToolTip(device.value("id").toString());
            }
            ui->connectionLabel->setText("API：已连接");
        } else {
            ui->connectionLabel->setText("API：连接失败，请先启动后端");
        }
        reply->deleteLater();
    });
}
void MainWindow::addDevice()
{
    const QString name = ui->nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::information(this, "提示", "请先输入设备名称。");
        return;
    }
    const QByteArray body = QJsonDocument(QJsonObject{
        {"name", name}, {"type", "generic"}, {"source", "virtual"}
    }).toJson(QJsonDocument::Compact);
    sendRequest("POST", kApiBase.resolved(QUrl("devices")), body);
    ui->nameEdit->clear();
}
void MainWindow::renameDevice()
{
    const QString id = selectedDeviceId();
    const QString name = ui->nameEdit->text().trimmed();
    if (id.isEmpty() || name.isEmpty()) {
        QMessageBox::information(this, "提示", "请选择设备并输入新名称。");
        return;
    }
    const QByteArray body = QJsonDocument(QJsonObject{{"name", name}}).toJson(QJsonDocument::Compact);
    sendRequest("PUT", kApiBase.resolved(QUrl("devices/" + id)), body);
}
void MainWindow::deleteDevice()
{
    const QString id = selectedDeviceId();
    if (id.isEmpty()) {
        QMessageBox::information(this, "提示", "请先选择设备。");
        return;
    }
    sendRequest("DELETE", kApiBase.resolved(QUrl("devices/" + id)));
}
void MainWindow::handleWebSocketMessage(const QString &message)
{
    Q_UNUSED(message);
    refreshDevices();
}
