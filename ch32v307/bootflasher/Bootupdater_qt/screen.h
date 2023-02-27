#ifndef SCREEN_H
#define SCREEN_H

#include "protocol.h"

#include <QDebug>
#include <QDialog>
#include <QEventLoop>
#include <QFile>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QNetworkInterface>
#include <QProgressBar>
#include <QPushButton>
#include <QTcpSocket>
#include <QTimer>
#include <QUdpSocket>

namespace Ui {
class Screen;
}

using namespace Protocol;

class Screen : public QDialog {
    Q_OBJECT

  public:
    explicit Screen(QWidget* parent = 0);
    ~Screen();
    void writeToPort(QByteArray&);

  private slots:
    void timeOut();

    void readDatagrammGet();
    void readDatagrammSet();
    void readDatagrammGetBroadcast();
    void onReboot();
    void onFirmwareUpdate();

    //--------- TCP ------------------------
    void readFromServer();

  private:
    Ui::Screen* ui;

    void init();
    QString* Str{nullptr};

    //__________ TCP ________________
    QTcpSocket* client{nullptr};

    void delay(uint32_t ms);

    void connectToServerControl();
    void disconnectFromServer();
    void parseFrame(const QByteArray& f);
    void sendToServer(QByteArray data);

    QString* fileName;
    QFileDialog* fileDialog;
    QByteArray* fileArr;
    uint16_t firmwareCS = 0;
    QProgressBar* barUpdate;
    uint32_t bytes_counter = 0;
    uint16_t packet_counter = 1;
    //__________ UDP ________________
    QUdpSocket* udpSocketSend{nullptr};
    QUdpSocket* udpSocketGet{nullptr};
    QUdpSocket* udpSocketGetBroadcast{nullptr};
    QHostAddress* localAddr{nullptr};
    QHostAddress* destAddr{nullptr};
    const QHostAddress localhost = QHostAddress::LocalHost;
    void sendDatagram(uint8_t* data, uint8_t len);
    void sendDatagramToBoard(uint8_t* data, uint8_t len, const QString& ip);

    static constexpr uint16_t TCP_PORT = 55555;
    static constexpr uint16_t LOCAL_PORT = 33333;
    static constexpr uint16_t UDP_PORT = 60000;
    static constexpr uint16_t BROADCAST_PORT = 0xFFFF;
    static constexpr uint16_t BOARD_PORT_IN = 40000;
    static constexpr uint16_t BOARD_PORT_OUT = 40001;
    static constexpr const char* IP_BOARD = "192.168.1.5";

    QLabel* labLocalIP;
    QLineEdit* lineIP;
    QPushButton* butReboot;
    QPushButton* butFirmwareUpdate;
    QPushButton* butExit;
    void sendData(uint8_t* data, uint8_t len);

    void parseFrame(uint8_t* data, uint8_t len);
};

#endif // SCREEN_H
