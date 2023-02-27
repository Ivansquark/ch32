#include "screen.h"

#include "ui_screen.h"

Screen::Screen(QWidget* parent) : QDialog(parent), ui(new Ui::Screen) {
    ui->setupUi(this);
    init();
}

Screen::~Screen() {
    delete fileName;
    delete fileArr;
    delete ui;
}

void Screen::init() {

    Str = new QString;
    labLocalIP = new QLabel("Local IP:", this);

    lineIP = new QLineEdit("192.168.3.200", this);
    butReboot = new QPushButton("Reboot", this);
    connect(butReboot, &QPushButton::clicked, this, &Screen::onReboot);
    butFirmwareUpdate = new QPushButton("FirmwereUpdate", this);
    connect(butFirmwareUpdate, &QPushButton::clicked, this,
            &Screen::onFirmwareUpdate);
    butExit = new QPushButton("Exit", this);
    connect(butExit, &QPushButton::clicked, [&] { close(); });
    ui->verticalLayout_28->addWidget(labLocalIP);
    ui->verticalLayout_28->addWidget(lineIP);
    ui->verticalLayout_28->addWidget(butReboot);
    barUpdate = new QProgressBar(this);
    ui->verticalLayout_28->addWidget(butFirmwareUpdate);
    ui->verticalLayout_28->addWidget(barUpdate);
    ui->verticalLayout_28->addWidget(butExit);

    //_______________ UDP ________________________________
    udpSocketSend = new QUdpSocket(this);
    udpSocketGet = new QUdpSocket(this);
    udpSocketGetBroadcast = new QUdpSocket(this);
    destAddr = new QHostAddress(QHostAddress::Broadcast);
    qDebug() << "localAddress" << localAddr;
    udpSocketGet->bind(QHostAddress::Any, BOARD_PORT_OUT);
    udpSocketGetBroadcast->bind(QHostAddress::Any, BROADCAST_PORT);
    qDebug() << "udpSocketGet->localPort()" << udpSocketGet->localPort();
    connect(udpSocketGet, &QUdpSocket::readyRead, this,
            &Screen::readDatagrammGet);
    connect(udpSocketGetBroadcast, &QUdpSocket::readyRead, this,
            &Screen::readDatagrammGetBroadcast);
    // connect(udpSocketSend, &QUdpSocket::readyRead, this,
    //        &Screen::readDatagrammSet);

    //_______________ TCP ________________________________
    client = new QTcpSocket(this);
    // connect(client,&QTcpSocket::connected,this,&Backend::isConnected);
    connect(client, &QTcpSocket::readyRead, this, &Screen::readFromServer);
    connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(slotError(QAbstractSocket::SocketError)));
    // connect(client,&QTcpSocket::disconnected,this, &Backend::isDisconnected);

    //-----------------------------------------------------
    fileName = new QString;
    fileArr = new QByteArray;
}

void Screen::timeOut() {}

void Screen::readDatagrammGet() {
    QHostAddress senderIP = QHostAddress::Any;
    char arr[512];
    QByteArray bArr;
    uint16_t sizeDatagram = udpSocketGet->pendingDatagramSize();
    udpSocketGet->readDatagram(arr, sizeDatagram, &senderIP);
    QString data;
    for (int i = 0; i < sizeDatagram; i++) {
        data.append(QString::number((uint8_t)(arr[i]), 16) + " ");
    }
    parseFrame((uint8_t*)arr, sizeDatagram);
}

void Screen::readDatagrammSet() {}

void Screen::readDatagrammGetBroadcast() {}

void Screen::onReboot() {
    uint8_t stateBuf[] = {'<', 6, SetState_type::SetReboot, 0, 0};
    uint8_t len = sizeof(stateBuf);
    uint16_t stateBufCrc = Crc16(stateBuf, len - 2);
    stateBuf[len - 2] = stateBufCrc >> 8;
    stateBuf[len - 1] = stateBufCrc;
    sendData(stateBuf, len);
}

void Screen::onFirmwareUpdate() {
    // set Udp:: SetUpdateFirmware
    uint8_t stateBuf[] = {'<', 6, SetState_type::SetUpdateFirmware, 0, 0};
    uint8_t len = sizeof(stateBuf);
    uint16_t stateBufCrc = Crc16(stateBuf, len - 2);
    stateBuf[len - 2] = stateBufCrc >> 8;
    stateBuf[len - 1] = stateBufCrc;
    sendData(stateBuf, len);
    delay(100);
    // connect to server
    if (client->state() == QAbstractSocket::ConnectedState) {
        // disconnectFromServer();
    }
    client->connectToHost("192.168.1.5", TCP_PORT);

    *fileName = fileDialog->getOpenFileName(
        this, "Select source file", ".", "Text files (*.bin); All files (*.*)");
    QFile file;
    file.setFileName(*fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "unable open file";
        this->setEnabled(true);
    } else {
        // start exchange
        // QByteArray aa = file.readAll();
        *fileArr = file.readAll();
        file.close();
        // MRTProto::pl_updateFirmware mode =
        // MRTProto::pl_updateFirmware::START; frmComposer.getHead()->type =
        // MRTProto::updateFirmware;
        // frmComposer.setPayLoad((MRTProto::BYTE_t*)&mode, 1);
        // frmComposer.composeFrame();
        // emit sendToServer(QByteArray((const char*)frmComposer.getFrame(),
        //                             frmComposer.getFrameLength()));
        // check CS
        firmwareCS = Crc16((uint8_t*)fileArr->data(), fileArr->size());
        QByteArray arr;
        arr.append(pl_updateFirmware::START);
        arr.append((char)(firmwareCS >> 8));
        arr.append((char)firmwareCS);
        sendToServer(arr);

        this->setEnabled(false);
        barUpdate->setEnabled(true);
        // TODO: pause or wait for cmdAnsMode
        // QThread::msleep(5000);
        auto pause = [](int ms) {
            QEventLoop loop;
            QTimer t;
            t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
            t.start(ms);
            loop.exec();
        };
        pause(100);
    }
}

void Screen::readFromServer() {
    if ((client->state() == QAbstractSocket::ConnectedState)) {
        QByteArray tempArr;
        uint16_t bytesAvailable = client->bytesAvailable();
        if (bytesAvailable) {
            QByteArray rcvdData = client->readAll();
            parseFrame(rcvdData);
        }
    }
}

void Screen::sendDatagram(uint8_t* data, uint8_t len) {
    QByteArray bytes;
    for (int i = 0; i < len; i++) {
        bytes.append(data[i]);
    }
    udpSocketSend->writeDatagram(bytes, QHostAddress(IP_BOARD), BOARD_PORT_IN);
}

void Screen::sendDatagramToBoard(uint8_t* data, uint8_t len,
                                 const QString& ip) {
    QByteArray bytes;
    for (int i = 0; i < len; i++) {
        bytes.append(data[i]);
    }
    udpSocketSend->writeDatagram(bytes, QHostAddress(ip), 60001);
}

void Screen::sendData(uint8_t* data, uint8_t len) { sendDatagram(data, len); }

void Screen::parseFrame(uint8_t* data, uint8_t len) {
    // TODO: check CS
    if (data[0] != '>')
        return;
    uint16_t crc_received = data[len - 1] + (data[len - 2] << 8);
    uint16_t crc_counted = Crc16(data, len - 2);
    if (crc_received != crc_counted) {
        qDebug() << "WRONG CRC";
        return;
    }
    qDebug() << data;
}

void Screen::parseFrame(const QByteArray& f) {
    QString strList;

    //------------ updateFirmware ------------------------------

    if (f[0] == pl_updateFirmware::START) {
        barUpdate->setValue(0);
        barUpdate->setEnabled(true);
        barUpdate->setVisible(true);
        bytes_counter = 0;
        packet_counter = 0;
        QByteArray arr;
        arr.clear();
        uint32_t temp_size = (fileArr->size() - bytes_counter);
        if (temp_size) {
            arr.append(pl_updateFirmware::TO_BOARD);
            arr.append((char)(packet_counter >> 8));
            arr.append((char)(packet_counter));
            uint32_t singleSize = 0;
            if (temp_size >= 512) {
                singleSize = 512;
            } else {
                singleSize = temp_size; // remainder bytes
            }
            for (uint32_t i = 0; i < singleSize; i++) {
                // arr.append(temp_data[bytes_counter]);
                arr.append((fileArr->data()[bytes_counter++]));
            }
            // qDebug()<<"packet counter" << packet_counter;
            QByteArray sendArr(arr);

            sendToServer(sendArr);
            uint8_t percent = 100 * ((float)bytes_counter / fileArr->size());
            barUpdate->setValue(percent);
        } else {
            QByteArray arr;
            arr.append(pl_updateFirmware::END);
            arr.append((char)(firmwareCS >> 8));
            arr.append((char)firmwareCS);
            sendToServer(arr);
        }
    } else if (f[0] == pl_updateFirmware::TO_PC) {
        uint16_t receivePacketNumber = (f[1] << 8 | f[2]);
        if (receivePacketNumber != packet_counter) {
            qDebug() << "Error in update";
            return;
        }
        packet_counter++;
        QByteArray arr;
        arr.clear();
        uint32_t temp_size = (fileArr->size() - bytes_counter);
        if (temp_size) {
            arr.append(pl_updateFirmware::TO_BOARD);
            arr.append((char)(packet_counter >> 8));
            arr.append((char)(packet_counter));
            uint32_t singleSize = 0;
            if (temp_size >= 512) {
                singleSize = 512;
            } else {
                singleSize = temp_size; // remainder bytes
            }
            for (uint32_t i = 0; i < singleSize; i++) {
                // arr.append(temp_data[bytes_counter]);
                arr.append((fileArr->data()[bytes_counter++]));
            }
            sendToServer(arr);
            uint8_t percent = 100 * ((float)bytes_counter / fileArr->size());
            barUpdate->setValue(percent);
        } else {
            QByteArray arr;
            arr.append(pl_updateFirmware::END);
            arr.append((char)(firmwareCS >> 8));
            arr.append((char)firmwareCS);
            sendToServer(arr);
        }
    } else if (f[0] == pl_updateFirmware::CS_OK) {
        // todo: check for response of CS counted by Board
        QByteArray arr;
        arr.append(pl_updateFirmware::BOARD_REBOOT);
        sendToServer(arr);
        this->setEnabled(true);
        // TODO: Check if we updating commutation firmware then shutdown
        // (if not try to reconnect Control)
        qDebug() << "update Shutdown if Commutation";
    } else if (f[0] == pl_updateFirmware::CS_NOT_OK) {
        // todo: check for response of CS counted by Board
        qDebug() << "CS not OK need to restart";
        barUpdate->setVisible(true);
        this->setEnabled(true);
    }
}

void Screen::delay(uint32_t ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

void Screen::sendToServer(QByteArray data) {
    if (client->state() == QAbstractSocket::ConnectedState) {
        client->write(data);
        client->waitForBytesWritten();
    } else {
        qDebug() << "No connection to server";
    }
}
