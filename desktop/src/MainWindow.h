#pragma once
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QWebSocket>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow final : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private slots:
    void refreshDevices();
    void addDevice();
    void renameDevice();
    void deleteDevice();
    void handleWebSocketMessage(const QString &message);
private:
    void sendRequest(const QByteArray &method, const QUrl &url, const QByteArray &body = {});
    QString selectedDeviceId() const;
    Ui::MainWindow *ui;
    QNetworkAccessManager m_network;
    QWebSocket m_socket;
};
