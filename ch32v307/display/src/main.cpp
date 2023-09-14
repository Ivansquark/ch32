#include "main.h"

#include "basic_timer.h"
#include "buttons.h"
#include "ch32v30x_usbhs_device.h"
#include "dac.h"
#include "gpio.h"
#include "lcdpar.h"
#include "rcc.h"
#include "uart.h"

#include "frwrapper.h"

/* Global Variable */
uint32_t SystemCoreClock = 144000000;
xQueueHandle queue1;
char* queue_buf;
//------------- objects in static memory (.data section) ----------------------
Rcc rcc(8);
// Uart5 uart5;
// Uart1 uart1;
// Uart3 uart3;
LcdParIni parDisp;
Figure fig;
Dac dac;
// ---------------- OS classes ------------------------------------------------
Buttons but;
//-----------------------------------------------------------------------------
extern uint8_t TxCDC_flag;
extern uint8_t RxCDC_flag;
extern uint16_t RxCDC_len;
extern uint8_t RxCDC_buf[];
extern uint8_t TxBULK_flag;
extern volatile uint8_t RxBULK_flag;
extern volatile uint16_t RxBULK_len;
extern uint8_t RxBULK_buf[];

void setRamSize(uint32_t size);
void checkButtonsDisp();

typedef struct {
    // Report ID = 0x01 (1) Collection: CA:Keyboard
    uint8_t reportId;
    // Usage 0x000700E0: Keyboard Left Control, Value = 0 to 1
    uint8_t KB_KeyboardKeyboardLeftControl : 1;
    // Usage 0x000700E1: Keyboard Left Shift, Value = 0 to 1
    uint8_t KB_KeyboardKeyboardLeftShift : 1;
    // Usage 0x000700E2: Keyboard Left Alt, Value = 0 to 1
    uint8_t KB_KeyboardKeyboardLeftAlt : 1;
    // Usage 0x000700E3: Keyboard Left GUI, Value = 0 to 1
    uint8_t KB_KeyboardKeyboardLeftGui : 1;
    // Usage 0x000700E4: Keyboard Right Control, Value = 0 to 1
    uint8_t KB_KeyboardKeyboardRightControl : 1;
    // Usage 0x000700E5: Keyboard Right Shift, Value = 0 to 1
    uint8_t KB_KeyboardKeyboardRightShift : 1;
    // Usage 0x000700E6: Keyboard Right Alt, Value = 0 to 1
    uint8_t KB_KeyboardKeyboardRightAlt : 1;
    // Usage 0x000700E7: Keyboard Right GUI, Value = 0 to 1
    uint8_t KB_KeyboardKeyboardRightGui : 1;
    uint8_t pad_2;       // Pad
    uint8_t Keyboard[2]; // Value = 0 to 101
} KeyboardReport_t;
typedef struct {
    // Report ID = 0x02 (2) Collection: CA:Mouse CP:Pointer
    uint8_t reportId;
    // Usage 0x00090001: Button 1 Primary/trigger, Value = 0 to 1
    uint8_t BTN_MousePointerButton1 : 1;
    // Usage 0x00090002: Button 2 Secondary, Value = 0 to 1
    uint8_t BTN_MousePointerButton2 : 1;
    // Usage 0x00090003: Button 3 Tertiary, Value = 0 to 1
    uint8_t BTN_MousePointerButton3 : 1;
    uint8_t : 5;             // Pad
    int8_t GD_MousePointerX; // Usage 0x00010030: X, Value = -127 to 127
    int8_t GD_MousePointerY; // Usage 0x00010031: Y, Value = -127 to 127
    // Usage 0x00010038: Wheel, Value = -127 to 127
    int8_t GD_MousePointerWheel;
} MouseReport_t;

KeyboardReport_t key;
MouseReport_t mouse;

int main(void) {
    // setRamSize(0x20000);
    /* add queues, ... */
    queue1 = xQueueCreate(10, sizeof(uint32_t));
    //--------  Interrupt region ----------------------------------------------
    InterruptManagerForObjects<TIM6_IRQn> timer_ms;
    timer_ms.addClass(&but);
    timer_ms.setVector(); // subscribe all classes to TIM3_IRQ
    BasicTimer6::Instance().start();
    __enable_irq();
    /* USB20 device init */
    USBHS_RCC_Init();
    USBHS_Device_Init(ENABLE);

    // TODO: draw screen saver
    fig.drawRect(50, 100, 100, 130, Figure::RED);
    // uint16_t j = 0;
    for (int i = 0; i < Figure::HALF_DISPLAY_MEMORY; i++) {
        fig.buff[i] = Figure::BLACK;
    }
    fig.fillScreen(Figure::BLACK);
    // checkButtonsDisp();
    // for (int i = 0; i < Figure::HALF_DISPLAY_MEMORY; i++) {
    //    fig.buff[i] = Figure::YELLOW;
    //}
    // fig.fillHalfScreenHigh(fig.buff);
    // fig.fillHalfScreenLow(fig.buff);
    // j += 1;
    //
    uint16_t X;
    uint16_t Y;
    uint32_t counterBulk = 0;
    bool isHighOrLow = false;
    RxBULK_flag = 0;
    bool mustSend0 = false;
    volatile uint32_t counter = 0;
    while (1) {
        X = 1000 * but.averageH / 4095;
        Y = 1000 * but.averageV / 4095;
        /*
        if (RxCDC_flag) {
            RxCDC_flag = 0;
            if (RxCDC_buf[0] == 0x30) { TxCDC_flag = 1; }
        }
        if (TxCDC_flag) {
            TxCDC_flag = 0;
            uint8_t testBuf[4] = {0};
            auto& [a, b, c, d] = testBuf;
            // auto& [a, b, c, d] = *(uint8_t(*)[4])testBuf;
            USBHS_Endp_DataUp(DEF_UEP2, testBuf, sizeof(testBuf),
                              DEF_UEP_CPY_LOAD);
            TxCDC_flag = 0;
        }
        */
        if (RxBULK_flag) {
            // receive from usb bulk4 (videomemory)
            RxBULK_flag = 0;
            memcpy((uint8_t*)(fig.buff) + counterBulk, USBHSD_UEP_RXBUF(4),
                   RxBULK_len);
            counterBulk += RxBULK_len;
            if (counterBulk >= LcdParIni::HALF_DISPLAY_MEMORY * 2) {
                counterBulk = 0;
                if (isHighOrLow) {
                    fig.fillHalfScreenLow(fig.buff);
                    isHighOrLow = false;
                } else {
                    fig.fillHalfScreenHigh(fig.buff);
                    isHighOrLow = true;
                }
            }
            NVIC_EnableIRQ(USBHS_IRQn);
            USBHSD->UEP4_RX_CTRL &= ~USBHS_UEP_R_RES_MASK;
            USBHSD->UEP4_RX_CTRL |= USBHS_UEP_R_RES_ACK;
        }
        /*
        if (TxBULK_flag) {
            TxBULK_flag = 0;
            uint8_t testBuf[4] = {0};
            auto& [a, b, c, d] = testBuf;
            a = 0x34;
            USBHS_Endp_DataUp(DEF_UEP4, testBuf, sizeof(testBuf),
                              DEF_UEP_CPY_LOAD);
            TxBULK_flag = 0;
        }
        */
        // TODO: set next to rtos hid
        //
        //----------------- buttons handler -----------------------------------
        if (but.isJoyB) {
            if (!but.currentModeOnceTime) {
                but.currentModeOnceTime = true;
                if (but.currentMode == Buttons::KEYBOARD) {
                    but.currentMode = Buttons::Mode::MOUSE;
                } else {
                    but.currentMode = Buttons::Mode::KEYBOARD;
                }
            }
        } else {
            but.currentModeOnceTime = false;
        }
        if (but.currentMode == Buttons::KEYBOARD) {
            if (counter >= 200000) {
                memset(&key, 0, sizeof(KeyboardReport_t));
                key.reportId = 0x01;
                counter = 0;
                if (but.isAnyButtonPressed()) {
                    key.Keyboard[1] = but.whichNumber();
                    if (key.Keyboard[1]) {
                        USBHS_Endp_DataUp(DEF_UEP3, (uint8_t*)&key,
                                          sizeof(KeyboardReport_t),
                                          DEF_UEP_CPY_LOAD);
                    }
                } else {
                    mustSend0 = true;
                }
            } else {
                counter++;
            }
            if (mustSend0) {
                counter = 0;
                memset(&key, 0, sizeof(KeyboardReport_t));
                key.reportId = 0x01;
                USBHS_Endp_DataUp(DEF_UEP3, (uint8_t*)&key,
                                  sizeof(KeyboardReport_t), DEF_UEP_CPY_LOAD);
                mustSend0 = false;
            }
        } else {
            // MOUSE
            if (counter >= 200000) {
                memset(&key, 0, sizeof(MouseReport_t));
                counter = 0;
                mouse.reportId = 2;
                // 4096 127; 0 -127 //2048 0
                if ((but.averageH > 1800) && (but.averageH < 2200)) {
                    mouse.GD_MousePointerX = 0;
                } else if (but.averageH <= 1800) {
                    mouse.GD_MousePointerX = 32 * (but.averageH - 2048) / 2048;
                } else if (but.averageH >= 2200) {
                    mouse.GD_MousePointerX = 32 * (but.averageH - 2048) / 2048;
                }
                if ((but.averageV > 1800) && (but.averageV < 2200)) {
                    mouse.GD_MousePointerY = 0;
                } else if (but.averageV <= 1900) {
                    mouse.GD_MousePointerY = 32 * (but.averageV - 2048) / 2048;
                } else if (but.averageV >= 2100) {
                    mouse.GD_MousePointerY = 32 * (but.averageV - 2048) / 2048;
                }
                if(but.isB13) {
                    mouse.BTN_MousePointerButton1 = 1;
                } else {
                    mouse.BTN_MousePointerButton1 = 0;
                }
                if(but.isB14) {
                    mouse.BTN_MousePointerButton2 = 1;
                } else {
                    mouse.BTN_MousePointerButton2 = 0;
                }
                if(but.isEnter) {
                    mouse.BTN_MousePointerButton3 = 1;
                } else {
                    mouse.BTN_MousePointerButton3 = 0;
                }
                if(but.isB12) {
                    mouse.GD_MousePointerWheel = 1;
                } else {
                    mouse.GD_MousePointerWheel = 0;
                }
                if(but.isB15) {
                    mouse.GD_MousePointerWheel = -1;
                } else {
                    if(!but.isB12)
                    mouse.GD_MousePointerWheel = 0;
                }
                USBHS_Endp_DataUp(DEF_UEP3, (uint8_t*)&mouse,
                                  sizeof(MouseReport_t), DEF_UEP_CPY_LOAD);
            } else {
                counter++;
            }
        }
    }
    // FR_OS::startOS();
    // while (1) {}
}

void setRamSize(uint32_t size) {
#define FLASH_KEY1 ((uint32_t)0x45670123)
#define FLASH_KEY2 ((uint32_t)0xCDEF89AB)
    //----- OPTION BYTE PROGRAMMING FOR CHANGE RAM SIZE -----------------------
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
    while (FLASH->STATR & FLASH_STATR_BSY) {}
    FLASH->OBKEYR = FLASH_KEY1;
    FLASH->OBKEYR = FLASH_KEY2;
    while (FLASH->STATR & FLASH_STATR_BSY) {}
    uint16_t pbuf[8] = {0};
    // write 0:0 RAM=128Kb CODE=192Kb
    uint8_t Data = 0;
    switch (size) {
    case 0x8000:
        // write 1:1 RAM=32Kb CODE=288Kb
        Data = 0xDF; // 1:1:0:1:1:1:1:1
        break;
    case 0x10000:
        // write 1:0 RAM=64Kb CODE=192Kb
        Data = 0x9F; // 1:0:0:1:1:1:1:1
        break;
    case 0x18000:
        // write 0:1 RAM=96Kb CODE=224Kb
        Data = 0x5F; // 0:1:0:1:1:1:1:1
        break;
    case 0x20000:
        // write 0:0 RAM=128Kb CODE=192Kb
        Data = 0x1F; // 0:0:0:1:1:1:1:1
        break;
    default:
        Data = 0x9F;
        break;
    }
    /* Read optionbytes */
    for (int i = 0; i < 8; i++) { pbuf[i] = *(uint16_t*)(0x1FFFF800 + 2 * i); }
    /* Erase optionbytes */
    FLASH->CTLR |= FLASH_CTLR_OPTER;
    FLASH->CTLR |= FLASH_CTLR_STRT;
    while (FLASH->STATR & FLASH_STATR_BSY) {}
    FLASH->CTLR &= ~FLASH_CTLR_OPTER;
    // Write optionbytes RAM
    pbuf[((0x1FFFF802 - 0x1FFFF800) / 2)] =
        ((((uint16_t) ~(Data)) << 8) | ((uint16_t)Data));

    FLASH->CTLR |= FLASH_CTLR_OPTPG;
    for (int i = 0; i < 8; i++) {
        *(uint16_t*)(0x1FFFF800 + 2 * i) = pbuf[i];
        while (FLASH->STATR & FLASH_STATR_BSY) {}
    }
    FLASH->CTLR &= ~FLASH_CTLR_OPTPG;
    // FLASH->CTLR |= FLASH_CTLR_LOCK;
}
