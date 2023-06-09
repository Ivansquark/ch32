#include "main.h"

#include "basic_timer.h"
#include "gpio.h"
#include "rcc.h"
#include "systim.h"

#include "ch32v30x_usbfs_device.h"
#include "w25q.h"

#include "ssd1306.h"
Rcc rcc(8);

W25q w25q;
//Display display;

constexpr uint16_t IMAGE_SIZE = 40960;
uint8_t imageArr[IMAGE_SIZE];

extern volatile uint8_t ReceivedLenEP0;
extern volatile uint8_t ReceivedLenEP2;
extern uint8_t Rx_Buf[];

uint8_t writeBuf[4096] = {0};

void ssd1306_TestFonts1();

int main(void) {
    /* USB20 device init */
    USBFS_RCC_Init();
    USBFS_Device_Init(ENABLE);
    Gpio::Out::init();

    // TODO read first byte in w25q flash = number of images of 40960 bytes
    volatile uint8_t imagesNum = 0;
    w25q.read((uint8_t*)&imagesNum, 0, 1);

    bool IsUpdateStarted = false;
    bool IsNeedSendReply = false;
    uint32_t updateCounter = 0;
    ReceivedLenEP2 = 0;
    uint8_t writeBufCounter = 0;
    ssd1306_init();
    ssd1306_Init();
    ssd1306_TestFonts1();
    while (1) {
        // TODO update image to flash handler
        // --------------------- USB -------------------------------
        if (ReceivedLenEP2 != 0) {
            memcpy(Rx_Buf, USBFS_EP2_Buf, ReceivedLenEP2);
            // TODO: parse data //64bytes
            if (!IsUpdateStarted) {
                // START
                if (ReceivedLenEP2 == 5) {
                    if (Rx_Buf[2] == 'A') {
                        IsUpdateStarted = true;
                        IsNeedSendReply = true;
                    }
                }
            } else {
                // STOP
                if (ReceivedLenEP2 == 4) {
                    if (Rx_Buf[2] == 'O') {
                        // write last buf
                        // w25q.write(Rx_Buf, (updateCounter * 4096),
                        //           writeBufCounter * 64);
                        IsUpdateStarted = false;
                        //______________ Software RESET _______________________
                        NVIC_SystemReset();
                    }
                } else if (ReceivedLenEP2 == 64) {
                    // receive by 64 bytes
                    //w25q.write(Rx_Buf, (updateCounter * 64), 64);
                    //updateCounter++;
                    // its write for 4096 bytes
                    //if (writeBufCounter < 64) {
                    //    // prepare buffer
                    //    memcpy(writeBuf + 64 * writeBufCounter, Rx_Buf, 64);
                    //    writeBufCounter++;
                    //} else {
                    //    writeBufCounter = 0;
                    //    w25q.write(writeBuf, (updateCounter * 4096), 4096);
                    //    updateCounter++;
                    //}
                    if (writeBufCounter < 63) {
                        // prepare buffer
                        memcpy((uint8_t*)writeBuf + 64 * writeBufCounter, Rx_Buf, 64);
                        writeBufCounter++;
                    } else {
                        memcpy((uint8_t*)writeBuf + 64 * writeBufCounter, Rx_Buf, 64);
                        writeBufCounter = 0;
                        w25q.write(writeBuf, (updateCounter * 4096), 4096);
                        updateCounter++;
                    }
                    //
                    IsNeedSendReply = true;
                } else {
                    // error
                    while (1) {}
                }
            }
            Rx_Buf[0] = 0;

            ReceivedLenEP2 = 0;
        } else if (IsNeedSendReply) {
            IsNeedSendReply = false;
            // Reply send Handler
            uint8_t arrReply[1] = {1};
            USBFS_Endp_DataUp(DEF_UEP3, (uint8_t*)&arrReply, sizeof(arrReply),
                              DEF_UEP_CPY_LOAD);
        } else if (ReceivedLenEP2 == 0) {
            if (IsUpdateStarted) continue;
            //-------------- Draw image main loop -----------------------------
            //

            for (int i = 0; i < imagesNum; i++) {
                w25q.read(imageArr, (IMAGE_SIZE * i), IMAGE_SIZE);
            }
        }
    }
}

void ssd1306_TestFonts1() {
    uint8_t y = 0;
    ssd1306_Fill(Black);

    #ifdef SSD1306_INCLUDE_FONT_16x26
    ssd1306_SetCursor(2, y);
    ssd1306_WriteString("Font 16x26", Font_16x26, White);
    y += 26;
    #endif

    #ifdef SSD1306_INCLUDE_FONT_11x18
    ssd1306_SetCursor(2, y);
    //ssd1306_WriteString("Font 11x18", Font_11x18, White);
    ssd1306_WriteString("Font_OPEL", Font_11x18, White);
    y += 18;
    #endif

    #ifdef SSD1306_INCLUDE_FONT_7x10
    ssd1306_SetCursor(2, y);
    ssd1306_WriteString("Font 7x10", Font_7x10, White);
    y += 10;
    #endif

    #ifdef SSD1306_INCLUDE_FONT_6x8
    ssd1306_SetCursor(2, y);
    //ssd1306_WriteString("Font 6x8", Font_6x8, White);
    #endif

    ssd1306_UpdateScreen();
}
