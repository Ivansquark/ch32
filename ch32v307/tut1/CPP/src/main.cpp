#include "main.h"

#include "basic_timer.h"
#include "eeprom.h"
#include "eth.h"
#include "gpio.h"
#include "my_tcp.h"
#include "my_udp.h"
#include "rcc.h"
#include "systim.h"
#include "w25q.h"

Rcc rcc(8);
Eeprom eeprom;
W25q flash;
Eth eth;
Udp udp;
Tcp tcp;

void timeout();
bool IsTimeout;

constexpr uint16_t SIZE = 256 * 12;
uint8_t tempArr[SIZE];

void delay(volatile uint32_t val);

static const unsigned char data_index_html[] = {
    /* /index.html */
    0x2f, 0x69, 0x6e, 0x64, 0x65, 0x78, 0x2e, 0x68, 0x74, 0x6d, 0x6c, 0,
    0x48, 0x54, 0x54, 0x50, 0x2f, 0x31, 0x2e, 0x30, 0x20, 0x32, 0x30, 0x30,
    0x20, 0x4f, 0x4b, 0xd,  0xa,  0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x3a,
    0x20, 0x6c, 0x77, 0x49, 0x50, 0x2f, 0x70, 0x72, 0x65, 0x2d, 0x30, 0x2e,
    0x36, 0x20, 0x28, 0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x77, 0x77,
    0x77, 0x2e, 0x73, 0x69, 0x63, 0x73, 0x2e, 0x73, 0x65, 0x2f, 0x7e, 0x61,
    0x64, 0x61, 0x6d, 0x2f, 0x6c, 0x77, 0x69, 0x70, 0x2f, 0x29, 0xd,  0xa,
    0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2d, 0x74, 0x79, 0x70, 0x65,
    0x3a, 0x20, 0x74, 0x65, 0x78, 0x74, 0x2f, 0x68, 0x74, 0x6d, 0x6c, 0xd,
    0xa,  0xd,  0xa,  0x3c, 0x68, 0x74, 0x6d, 0x6c, 0x3e, 0x3c, 0x62, 0x6f,
    0x64, 0x79, 0x3e, 0xa,  0x3c, 0x68, 0x74, 0x6d, 0x6c, 0x3e, 0xa,  0x20,
    0x20, 0x20, 0x20, 0x3c, 0x62, 0x6f, 0x64, 0x79, 0x20, 0x73, 0x74, 0x79,
    0x6c, 0x65, 0x3d, 0x22, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x3a, 0x20, 0x62,
    0x6c, 0x61, 0x63, 0x6b, 0x3b, 0x20, 0x62, 0x61, 0x63, 0x6b, 0x67, 0x72,
    0x6f, 0x75, 0x6e, 0x64, 0x2d, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x3a, 0x20,
    0x23, 0x41, 0x41, 0x46, 0x22, 0x3e, 0xa,  0x20, 0x20, 0x20, 0x20, 0x3c,
    0x62, 0x6f, 0x64, 0x79, 0x3e, 0xa,  0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x3c, 0x68, 0x31, 0x20, 0x73, 0x74, 0x79, 0x6c, 0x65, 0x3d,
    0x22, 0x74, 0x65, 0x78, 0x74, 0x2d, 0x61, 0x6c, 0x69, 0x67, 0x6e, 0x3a,
    0x20, 0x63, 0x65, 0x6e, 0x74, 0x65, 0x72, 0x3b, 0x22, 0x3e, 0xa,  0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x53,
    0x54, 0x4d, 0x33, 0x32, 0x46, 0x34, 0x30, 0x37, 0x78, 0x38, 0x3c, 0x62,
    0x72, 0x3e, 0x3c, 0x62, 0x72, 0x3e, 0xa,  0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x57, 0x45, 0x42, 0x20, 0x53,
    0x65, 0x72, 0x76, 0x65, 0x72, 0xa,  0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x3c, 0x2f, 0x68, 0x31, 0x3e, 0xa,  0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x3c, 0x70, 0x20, 0x73, 0x74, 0x79, 0x6c, 0x65,
    0x3d, 0x22, 0x74, 0x65, 0x78, 0x74, 0x2d, 0x61, 0x6c, 0x69, 0x67, 0x6e,
    0x3a, 0x20, 0x63, 0x65, 0x6e, 0x74, 0x65, 0x72, 0x3b, 0x22, 0x3e, 0xa,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3c, 0x2f, 0x70, 0x3e,
    0xa,  0x20, 0x20, 0x20, 0x20, 0x3c, 0x2f, 0x62, 0x6f, 0x64, 0x79, 0x3e,
    0xa,  0x3c, 0x2f, 0x68, 0x74, 0x6d, 0x6c, 0x3e, 0xa,
};

int main(void) {
    // flash.reset();
    // delay(1000000);
    // flash.WAKEUP();
    // delay(10000);
    // uint16_t id = 0;
    // do {
    //    id = 0;
    //    //id = flash.readSR();
    //    id = flash.readID();
    //}
    // while (id != flash.W25Q32);
    // const char* str = "Hello wq25 flash";
    // uint8_t tempArr[20] = {0};
    // flash.write((uint8_t*)str, 0, 15);
    // flash.erase_Sector(0);
    // flash.write(tempArr, 0, sizeof(tempArr));
    flash.read(tempArr, 0x0, sizeof(tempArr));
    // SysTim::init(144000000); // 1 s
    __enable_irq();
    BasicTimer6::Instance().setCallback(timeout);
    BasicTimer6::Instance().start(500);
    IsTimeout = false;
    bool timeoutSate = false;
    Gpio::Out::init();

    uint8_t counter = 0;
    while (1) {
        eth.rx_handler();
        // TCP handler
        // TODO: make Rx Tx buff
        if (tcp.getDataForSend()) {
            counter++;
            continue;
            if (counter < 10) continue;
            tcp.setDataForSend(false);
            switch (tcp.currentHttpState) {
            case Http::GET_HTML: {
                // TODO: send answer
                uint16_t sizeHeadIndexHtml = W25q::pThis->SizeHeadIndexHtml;
                uint16_t sizeIndexHtml = W25q::pThis->SizeIndexHtml;
                uint8_t arr[sizeHeadIndexHtml + sizeIndexHtml];
                W25q::pThis->read((uint8_t*)arr,
                                  W25q::pThis->AddressHeadIndexHtml,
                                  sizeHeadIndexHtml);
                W25q::pThis->read(((uint8_t*)arr + sizeHeadIndexHtml),
                                  W25q::pThis->AddressIndexHtml, sizeIndexHtml);
                tcp.server_send(arr, sizeHeadIndexHtml + sizeIndexHtml);
                // tcp.server_send(data_index_html, sizeof(data_index_html));
                // tcp.server_connection_close(tcp.tpcbPtr, nullptr);
            } break;
            case Http::GET_ICO: {
                uint16_t sizeHeadIco = W25q::pThis->SizeHeadIco;
                uint16_t sizeIco = W25q::pThis->SizeIco;
                uint8_t arr[sizeHeadIco + sizeIco];
                W25q::pThis->read((uint8_t*)arr, W25q::pThis->AddressHeadIco,
                                  sizeHeadIco);
                W25q::pThis->read(((uint8_t*)arr + sizeHeadIco),
                                  W25q::pThis->AddressIco, sizeIco);
                tcp.server_send(arr, sizeHeadIco + sizeIco);
                // server_connection_close(tpcb, es);
            } break;
            case Http::GET_CSS: {
                tcp.server_send(NULL, 0);
                // server_connection_close(tpcb, es);
            } break;
            case Http::GET_JS: {
                tcp.server_send(NULL, 0);
                // server_connection_close(tpcb, es);
            } break;
            default:
                break;
            }
        }

        if (IsTimeout) {
            IsTimeout = false;
            if (!timeoutSate) {
                timeoutSate = true;
                Gpio::Out::setRed();
                Gpio::Out::resetBlue();
            } else {
                timeoutSate = false;
                Gpio::Out::resetRed();
                Gpio::Out::setBlue();
            }
        }
        if (udp.SendFlag) {
            static uint8_t add;
            add += 1;
            static uint16_t lineCounter;

            for (int i = 0; i < sizeof(tempArr); i++) { tempArr[i] = i + add; }
            Udp::Header head;
            head.PackFlag = 0xFFFF;
            head.pixel.dataLen = 0x100;
            head.pixel.dataNum = 0x0;
            head.line.dataLen = 0x000;
            head.line.dataNum = 0x0;

            uint16_t MAX_SIZE = 0x5ba;
            uint8_t count = sizeof(tempArr) / MAX_SIZE; // packet nums
            uint16_t l = 0;
            head.pixel.dataNum = count;
            for (int i = 0; i < count + 1; i++) {
                if (lineCounter >= 0xFFF) {
                    lineCounter = 0;
                } else {
                    lineCounter++;
                }
                l = sizeof(tempArr) - (MAX_SIZE * i);
                if (l >= MAX_SIZE) {
                    head.pixel.dataLen = MAX_SIZE;
                    head.pixel.dataNum = count;
                    head.line.dataLen = lineCounter;
                    head.line.dataNum = i;
                    uint8_t temp = *(uint8_t*)&head.pixel;
                    *(uint8_t*)&head.pixel = *((uint8_t*)(&head.pixel) + 1);
                    *((uint8_t*)(&head.pixel) + 1) = temp;
                    temp = *(uint8_t*)&head.line;
                    *(uint8_t*)&head.line = *((uint8_t*)(&head.line) + 1);
                    *((uint8_t*)(&head.line) + 1) = temp;
                    memcpy(Udp::pThis->udp_msg_send, &head,
                           sizeof(Udp::Header));
                    memcpy((uint8_t*)Udp::pThis->udp_msg_send +
                               sizeof(Udp::Header),
                           (uint8_t*)tempArr + i * (MAX_SIZE), MAX_SIZE);
                    uint16_t len = sizeof(Udp::Header) + MAX_SIZE;
                    udp.sendToPC(udp.udp_msg_send, len);
                } else {
                    // last packet
                    head.pixel.dataLen = l;
                    head.pixel.dataNum = count;
                    head.line.dataLen = lineCounter;
                    head.line.dataNum = i;
                    uint8_t temp = *(uint8_t*)&head.pixel;
                    *(uint8_t*)&head.pixel = *((uint8_t*)(&head.pixel) + 1);
                    *((uint8_t*)(&head.pixel) + 1) = temp;
                    temp = *(uint8_t*)&head.line;
                    *(uint8_t*)&head.line = *((uint8_t*)(&head.line) + 1);
                    *((uint8_t*)(&head.line) + 1) = temp;
                    memcpy(Udp::pThis->udp_msg_send, &head,
                           sizeof(Udp::Header));
                    memcpy((uint8_t*)Udp::pThis->udp_msg_send +
                               sizeof(Udp::Header),
                           (uint8_t*)tempArr + i * (MAX_SIZE), l);
                    uint16_t len = sizeof(Udp::Header) + l;
                    udp.sendToPC(udp.udp_msg_send, len);
                }
            }
        }
        // if (SysTim::getIsTimeout()) {
        //    SysTim::setIsTimeout(false);
        //    if (!timeoutSate) {
        //        timeoutSate = true;
        //        Gpio::Out::setRed();
        //        Gpio::Out::resetBlue();
        //    } else {
        //        timeoutSate = false;
        //        Gpio::Out::resetRed();
        //        Gpio::Out::setBlue();
        //    }
        //}
    }
}

void timeout() { IsTimeout = true; }

void delay(volatile uint32_t val) {
    while (val--) {}
}
