#include "main.h"

#include "basic_timer.h"
#include "buttons.h"
#include "gpio.h"
#include "lcdpar.h"
#include "rcc.h"

#include "frwrapper.h"
/* Global define */
#define TASK_RECEIVE_TASK_PRIO 5
#define TASK_RECEIVE_STK_SIZE 512
#define RECV_BUF_SIZE 2048
char recv_buf[RECV_BUF_SIZE] = {0};

/* Global Variable */
uint32_t SystemCoreClock = 144000000;
xQueueHandle queue1;
char* queue_buf;
//------------- objects in static memory (.data section) ----------------------
Rcc rcc(8);
// LcdParIni parDisp;
Figure fig;
// ---------------- OS classes ------------------------------------------------
Buttons but;
//-----------------------------------------------------------------------------

void setRamSize(uint32_t size);

int main(void) {
    /* add queues, ... */
    queue1 = xQueueCreate(10, sizeof(uint32_t));
    //--------  Interrupt region ----------------------------------------------
    InterruptManagerForObjects<TIM6_IRQn> timer_ms;
    timer_ms.addClass(&but);
    timer_ms.setVector(); // subscribe all classes to TIM3_IRQ
    BasicTimer6::Instance().start();
    __enable_irq();

    // setRamSize(0x20000);

    // fig.drawRect(50, 100, 100, 130, Figure::RED);
    uint16_t j = 0;
    for (int i = 0; i < Figure::HALF_DISPLAY_MEMORY; i++) {
        fig.buff[i] = Figure::BLACK;
    }
    fig.fillScreen(Figure::BLACK);
    while (1) {
        if (but.isAnyButtonPressed()) {
            if (but.currentBut == Buttons::B1) {
                if (!but.B1_once) {
                    but.B1_once = true;
                    fig.drawRect(5, 55, 5, 55, Figure::BLUE);
                }
            } else {
                if (but.B1_once) {
                    but.B1_once = false;
                    fig.drawRect(5, 55, 5, 55, Figure::BLACK);
                }
            }
            if (but.currentBut == Buttons::B2) {
                if (!but.B2_once) {
                    but.B2_once = true;
                    fig.drawRect(60, 110, 5, 55, Figure::BLUE);
                }
            } else {
                if (but.B2_once) {
                    but.B2_once = false;
                    fig.drawRect(60, 110, 5, 55, Figure::BLACK);
                }
            }
            if (but.currentBut == Buttons::B3) {
                if (!but.B3_once) {
                    but.B3_once = true;
                    fig.drawRect(115, 165, 5, 55, Figure::BLUE);
                }
            } else {
                if (but.B3_once) {
                    but.B3_once = false;
                    fig.drawRect(115, 165, 5, 55, Figure::BLACK);
                }
            }
            if (but.currentBut == Buttons::B4) {
                if (!but.B4_once) {
                    but.B4_once = true;
                    fig.drawRect(5, 55, 60, 110, Figure::BLUE);
                }
            } else {
                if (but.B4_once) {
                    but.B4_once = false;
                    fig.drawRect(5, 55, 60, 110, Figure::BLACK);
                }
            }
            if (but.currentBut == Buttons::B5) {
                if (!but.B5_once) {
                    but.B5_once = true;
                    fig.drawRect(60, 110, 60, 110, Figure::BLUE);
                }
            } else {
                if (but.B5_once) {
                    but.B5_once = false;
                    fig.drawRect(60, 110, 60, 110, Figure::BLACK);
                }
            }
            if (but.currentBut == Buttons::B6) {
                if (!but.B6_once) {
                    but.B6_once = true;
                    fig.drawRect(115, 165, 60, 110, Figure::BLUE);
                }
            } else {
                if (but.B6_once) {
                    but.B6_once = false;
                    fig.drawRect(115, 165, 60, 110, Figure::BLACK);
                }
            }
            if (but.currentBut == Buttons::B7) {
                if (!but.B7_once) {
                    but.B7_once = true;
                    fig.drawRect(5, 55, 115, 165, Figure::BLUE);
                }
            } else {
                if (but.B7_once) {
                    but.B7_once = false;
                    fig.drawRect(5, 55, 115, 165, Figure::BLACK);
                }
            }
            if (but.currentBut == Buttons::B8) {
                if (!but.B8_once) {
                    but.B8_once = true;
                    fig.drawRect(60, 110, 115, 165, Figure::BLUE);
                }
            } else {
                if (but.B8_once) {
                    but.B8_once = false;
                    fig.drawRect(60, 110, 115, 165, Figure::BLACK);
                }
            }
            if (but.currentBut == Buttons::B9) {
                if (!but.B9_once) {
                    but.B9_once = true;
                    fig.drawRect(115, 165, 115, 165, Figure::BLUE);
                }
            } else {
                if (but.B9_once) {
                    but.B9_once = false;
                    fig.drawRect(115, 165, 115, 165, Figure::BLACK);
                }
            }
            if (but.currentBut == Buttons::B10) {
                if (!but.B10_once) {
                    but.B10_once = true;
                    fig.drawRect(5, 55, 170, 220, Figure::BLUE);
                }
            } else {
                if (but.B10_once) {
                    but.B10_once = false;
                    fig.drawRect(5, 55, 170, 220, Figure::BLACK);
                }
            }
            if (but.currentBut == Buttons::B0) {
                if (!but.B0_once) {
                    but.B0_once = true;
                    fig.drawRect(60, 110, 170, 220, Figure::BLUE);
                }
            } else {
                if (but.B0_once) {
                    but.B0_once = false;
                    fig.drawRect(60, 110, 170, 220, Figure::BLACK);
                }
            }
            if (but.currentBut == Buttons::B11) {
                if (!but.B11_once) {
                    but.B11_once = true;
                    fig.drawRect(115, 165, 170, 220, Figure::BLUE);
                }
            } else {
                if (but.B11_once) {
                    but.B11_once = false;
                    fig.drawRect(115, 165, 170, 220, Figure::BLACK);
                }
            }
            if (but.currentBut == Buttons::B12) {
                if (!but.B12_once) {
                    but.B12_once = true;
                    fig.drawRect(230, 270, 50, 90, Figure::BLUE);
                }
            } else {
                if (but.B12_once) {
                    but.B12_once = false;
                    fig.drawRect(230, 270, 50, 90, Figure::BLACK);
                }
            }
            if (but.currentBut == Buttons::B13) {
                if (!but.B13_once) {
                    but.B13_once = true;
                    fig.drawRect(180, 220, 100, 140, Figure::BLUE);
                }
            } else {
                if (but.B13_once) {
                    but.B13_once = false;
                    fig.drawRect(180, 220, 100, 140, Figure::BLACK);
                }
            }
            if (but.currentBut == Buttons::Enter) {
                if (!but.Enter_once) {
                    but.Enter_once = true;
                    fig.drawRect(230, 270, 100, 140, Figure::BLUE);
                }
            } else {
                if (but.Enter_once) {
                    but.Enter_once = false;
                    fig.drawRect(230, 270, 100, 140, Figure::BLACK);
                }
            }
            if (but.currentBut == Buttons::B14) {
                if (!but.B14_once) {
                    but.B14_once = true;
                    fig.drawRect(280, 320, 100, 140, Figure::BLUE);
                }
            } else {
                if (but.B14_once) {
                    but.B14_once = false;
                    fig.drawRect(280, 320, 100, 140, Figure::BLACK);
                }
            }
            if (but.currentBut == Buttons::B15) {
                if (!but.B15_once) {
                    but.B15_once = true;
                    fig.drawRect(230, 270, 150, 190, Figure::BLUE);
                }
            } else {
                if (but.B15_once) {
                    but.B15_once = false;
                    fig.drawRect(230, 270, 150, 190, Figure::BLACK);
                }
            }
        } else {
            fig.fillScreen(Figure::BLACK);
        }
        // for (int i = 0; i < Figure::HALF_DISPLAY_MEMORY; i++) {
        //     fig.buff[i] = Figure::BLACK + i + j;
        // }
        // fig.fillHalfScreenHigh(fig.buff);
        // fig.fillHalfScreenLow(fig.buff);
        // j += 1;
    }
    FR_OS::startOS();
    //__enable_irq();
    // BasicTimer6::Instance().setCallback(timeout);
    // BasicTimer6::Instance().start(500);
    // Gpio::Out::init();
    while (1) {}
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
