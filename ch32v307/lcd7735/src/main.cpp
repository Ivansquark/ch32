#include "main.h"

#include "basic_timer.h"
#include "display.h"
#include "gpio.h"
#include "rcc.h"
#include "systim.h"

#include "ch32v30x_usbfs_device.h"
#include "w25q.h"

#include "opel_160_128.h"

Rcc rcc(8);

W25q w25q;
Display display;

constexpr uint16_t IMAGE_SIZE = 40960;
uint8_t imageArr[IMAGE_SIZE];

extern volatile uint8_t ReceivedLenEP0;
extern volatile uint8_t ReceivedLenEP2;
extern uint8_t Rx_Buf[];

int main(void) {
    /* USB20 device init */
    USBFS_RCC_Init();
    USBFS_Device_Init(ENABLE);
    bool IsNeedStartTransmit = false;
    Gpio::Out::init();
    display.ST7735_DrawImage(0, 0, 160, 128, image_data_OPEL160_128);
    display.writeString(0, 0, "opa", Font_16x26, RED, WHITE);

    // TODO read first byte in w25q flash = number of images of 40960 bytes
    volatile uint8_t imagesNum = 0;
    w25q.read((uint8_t*)&imagesNum, 0, 1);
    imagesNum = 2;
    // for (int i = 0; i < 1; i++) {
    //    w25q.read(imageArr, (IMAGE_SIZE * i + 256), IMAGE_SIZE);
    //    display.ST7735_DrawImage(0, 0, 160, 128, (const uint16_t*)imageArr);
    //}
    // for (int i = 0; i < 16; i++) {
    //    memset(imageArr, i * 1000, IMAGE_SIZE);
    //    w25q.write(imageArr, (IMAGE_SIZE * i + 256), IMAGE_SIZE);
    //    // display.ST7735_DrawImage(0, 0, 160, 128, (const
    //    uint16_t*)imageArr);
    //}
    bool updateSaterted = true;
    uint32_t updateCounter = 0;
    ReceivedLenEP2 = 0;
    while (1) {
        // TODO update image to flash handler
        // --------------------- USB -------------------------------
        if (ReceivedLenEP2 != 0) {
            memcpy(Rx_Buf, USBFS_EP2_Buf, ReceivedLenEP2);
            // TODO: parse data //64bytes
            // START STOP
            if (!updateSaterted) {
                if (ReceivedLenEP2 == 5) {
                    if (Rx_Buf[2] == 'A') { updateSaterted = true; }
                }
            } else {
                if (ReceivedLenEP2 == 4) {
                    if (Rx_Buf[2] == 'O') {
                        updateSaterted = false;
                        //______________ Software RESET _______________________
                        NVIC_SystemReset();
                    }
                } else if (ReceivedLenEP2 == 64) {
                    // copy to flash by 64 bytes
                    w25q.write(Rx_Buf, (updateCounter * 64), 64);
                    updateCounter++;
                } else {
                    // error
                    while (1) {}
                }
            }
            Rx_Buf[0] = 0;

            ReceivedLenEP2 = 0;
        } else if (ReceivedLenEP2 == 0) {
            //
            for (int i = 0; i < imagesNum; i++) {
                w25q.read(imageArr, (IMAGE_SIZE * i + 256), IMAGE_SIZE);
                display.ST7735_DrawImage(0, 0, 160, 128,
                                         (const uint16_t*)imageArr);
            }
        }

        /*
                display.writeString(0, 0, "1", Font_16x26, RED, BLACK);
                display.writeString(0, 0, "12", Font_16x26, RED, BLACK);
                display.writeString(0, 0, "123", Font_16x26, RED, BLACK);
                display.writeString(0, 0, "1234", Font_16x26, RED, BLACK);
                display.writeString(0, 0, "1234", Font_16x26, BLUE, WHITE);
                display.writeString(0, 0, "123", Font_16x26, BLUE, WHITE);
                display.writeString(0, 0, "12", Font_16x26, BLUE, WHITE);
                display.writeString(0, 0, "1", Font_16x26, BLUE, WHITE);

                display.writeString(0, 30, "1", Font_11x18, YELLOW, BLACK);
                display.writeString(0, 30, "12", Font_11x18, YELLOW, BLACK);
                display.writeString(0, 30, "123", Font_11x18, YELLOW, BLACK);
                display.writeString(0, 30, "1234", Font_11x18, YELLOW, BLACK);
                display.writeString(0, 30, "1234", Font_11x18, GREEN, BLACK);
                display.writeString(0, 30, "123", Font_11x18, GREEN, BLACK);
                display.writeString(0, 30, "12", Font_11x18, GREEN, BLACK);
                display.writeString(0, 30, "1", Font_11x18, GREEN, BLACK);

                display.writeString(0, 60, "1", Font_7x10, CYAN, BLACK);
                display.writeString(0, 60, "12", Font_7x10, CYAN, BLACK);
                display.writeString(0, 60, "123", Font_7x10, CYAN, BLACK);
                display.writeString(0, 60, "1234", Font_7x10, CYAN, BLACK);
                display.writeString(0, 60, "1234", Font_7x10, MAGENTA, BLACK);
                display.writeString(0, 60, "123", Font_7x10, MAGENTA, BLACK);
                display.writeString(0, 60, "12", Font_7x10, MAGENTA, BLACK);
                display.writeString(0, 60, "1", Font_7x10, MAGENTA, BLACK);
        */
    }
}
