
#include "main.h"

#include "basic_timer.h"
#include "gpio.h"
#include "lcdspi.h"
#include "led.h"
#include "rcc.h"
#include "tirq.h"

#include <array>
#include <stdlib.h>

/* Global Variable */
uint32_t SystemCoreClock = 144000000;
//------------- objects in static memory (.data section) ----------------------
Rcc rcc(8);
//-----------------------------------------------------------------------------
#include "rad.cpp"
extern const uint16_t image_data_rad[];
int main(void) {
    Figure lcd;
    Font_interface fonts;
    lcd.fillScreen(LcdSpiInterface::BLUE);
    volatile uint16_t i = 0;
    while (1) {
        if (i < 100) {
            if (i == 1) { lcd.fillScreen(LcdSpiInterface::BLUE); }
            fonts.lcdprop.TextColor = Font_interface::GREEN;
            fonts.lcdprop.BackColor = Font_interface::BLUE;
            //fonts.drawString(0, 30, "Setting  145 kV");
            fonts.drawString(0, 30, "Setting");
            int x = 144;
            fonts.intToChar(x);
            if (x / 10 == 0) {
                fonts.arr[4] = '\0';
                fonts.arr[3] = 'V';
                fonts.arr[2] = 'k';
                fonts.arr[1] = ' ';
            } else if (x / 100 == 0) {
                fonts.arr[5] = '\0';
                fonts.arr[4] = 'V';
                fonts.arr[3] = 'k';
                fonts.arr[2] = ' ';
            } else {
                fonts.arr[3] = ' ';
                fonts.arr[4] = 'k';
                fonts.arr[5] = 'V';
                fonts.arr[6] = '\0';
            }
            fonts.drawString(163, 30, fonts.arr);
            fonts.drawString(0, 80, "Measure");
            fonts.lcdprop.TextColor = Font_interface::BLACK;
            fonts.lcdprop.BackColor = Font_interface::WHITE;
            fonts.intToChar(i);
            if (i / 10 == 0) {
                fonts.arr[4] = '\0';
                fonts.arr[3] = 'V';
                fonts.arr[2] = 'k';
                fonts.arr[1] = ' ';
            } else if (i / 100 == 0) {
                fonts.arr[5] = '\0';
                fonts.arr[4] = 'V';
                fonts.arr[3] = 'k';
                fonts.arr[2] = ' ';
            } else {
                fonts.arr[3] = ' ';
                fonts.arr[4] = 'k';
                fonts.arr[5] = 'V';
                fonts.arr[6] = '\0';
            }
            fonts.drawString(163, 80, fonts.arr);
            fonts.lcdprop.TextColor = Font_interface::GREEN;
            fonts.lcdprop.BackColor = Font_interface::BLUE;
            //fonts.drawString(0, 130, "Setting  0.50 mA");
            fonts.drawString(0, 130, "Setting");
            x = 60;
            fonts.intToChar(x);
            if (x / 10 == 0) {
                fonts.arr[3] = fonts.arr[0];
                fonts.arr[2] = '0';
                fonts.arr[1] = '.';
                fonts.arr[0] = '0';
            } else if (x / 100 == 0) {
                fonts.arr[3] = fonts.arr[1];
                fonts.arr[2] = fonts.arr[0];
                fonts.arr[1] = '.';
                fonts.arr[0] = '0';
            } else {
                fonts.arr[3] = fonts.arr[2];
                fonts.arr[2] = fonts.arr[1];
                fonts.arr[1] = '.';
            }
            fonts.arr[4] = ' ';
            fonts.arr[5] = 'm';
            fonts.arr[6] = 'A';
            fonts.arr[7] = '\0';
            fonts.drawString(163, 130, fonts.arr);

            fonts.drawString(0, 180, "Measure");
            fonts.lcdprop.TextColor = Font_interface::BLACK;
            fonts.lcdprop.BackColor = Font_interface::WHITE;
            fonts.intToChar(i);
            if (i / 10 == 0) {
                fonts.arr[3] = fonts.arr[0];
                fonts.arr[2] = '0';
                fonts.arr[1] = '.';
                fonts.arr[0] = '0';
            } else if (i / 100 == 0) {
                fonts.arr[3] = fonts.arr[1];
                fonts.arr[2] = fonts.arr[0];
                fonts.arr[1] = '.';
                fonts.arr[0] = '0';
            } else {
                fonts.arr[3] = fonts.arr[2];
                fonts.arr[2] = fonts.arr[1];
                fonts.arr[1] = '.';
            }
            fonts.arr[4] = ' ';
            fonts.arr[5] = 'm';
            fonts.arr[6] = 'A';
            fonts.arr[7] = '\0';
            fonts.drawString(163, 180, fonts.arr);
        } else {
            if (i > 200) i = 0;
            if (i == 100) {
                lcd.fillScreen(LcdSpiInterface::RED);
                lcd.drawPicture(270, 320, 0, 50, (uint16_t*)image_data_rad);
            }
            fonts.lcdprop.TextColor = Font_interface::GREEN;
            fonts.lcdprop.BackColor = Font_interface::RED;
            //fonts.drawString(0, 30, "Setting  145 kV");
            fonts.drawString(0, 30, "Setting");
            int x = 144;
            fonts.intToChar(x);
            if (x / 10 == 0) {
                fonts.arr[4] = '\0';
                fonts.arr[3] = 'V';
                fonts.arr[2] = 'k';
                fonts.arr[1] = ' ';
            } else if (x / 100 == 0) {
                fonts.arr[5] = '\0';
                fonts.arr[4] = 'V';
                fonts.arr[3] = 'k';
                fonts.arr[2] = ' ';
            } else {
                fonts.arr[3] = ' ';
                fonts.arr[4] = 'k';
                fonts.arr[5] = 'V';
                fonts.arr[6] = '\0';
            }
            fonts.drawString(163, 30, fonts.arr);
            fonts.drawString(0, 80, "Measure");
            fonts.lcdprop.TextColor = Font_interface::BLACK;
            fonts.lcdprop.BackColor = Font_interface::WHITE;
            fonts.intToChar(i);
            if (i / 10 == 0) {
                fonts.arr[4] = '\0';
                fonts.arr[3] = 'V';
                fonts.arr[2] = 'k';
                fonts.arr[1] = ' ';
            } else if (i / 100 == 0) {
                fonts.arr[5] = '\0';
                fonts.arr[4] = 'V';
                fonts.arr[3] = 'k';
                fonts.arr[2] = ' ';
            } else {
                fonts.arr[3] = ' ';
                fonts.arr[4] = 'k';
                fonts.arr[5] = 'V';
                fonts.arr[6] = '\0';
            }
            fonts.drawString(163, 80, fonts.arr);
            fonts.lcdprop.TextColor = Font_interface::GREEN;
            fonts.lcdprop.BackColor = Font_interface::RED;
            //fonts.drawString(0, 130, "Setting  0.50 mA");
            fonts.drawString(0, 130, "Setting");
            x = 60;
            fonts.intToChar(x);
            if (x / 10 == 0) {
                fonts.arr[3] = fonts.arr[0];
                fonts.arr[2] = '0';
                fonts.arr[1] = '.';
                fonts.arr[0] = '0';
            } else if (x / 100 == 0) {
                fonts.arr[3] = fonts.arr[1];
                fonts.arr[2] = fonts.arr[0];
                fonts.arr[1] = '.';
                fonts.arr[0] = '0';
            } else {
                fonts.arr[3] = fonts.arr[2];
                fonts.arr[2] = fonts.arr[1];
                fonts.arr[1] = '.';
            }
            fonts.arr[4] = ' ';
            fonts.arr[5] = 'm';
            fonts.arr[6] = 'A';
            fonts.arr[7] = '\0';
            fonts.drawString(163, 130, fonts.arr);
            fonts.drawString(0, 180, "Measure");
            fonts.lcdprop.TextColor = Font_interface::BLACK;
            fonts.lcdprop.BackColor = Font_interface::WHITE;
            fonts.intToChar(i);
            if (i / 10 == 0) {
                fonts.arr[3] = fonts.arr[0];
                fonts.arr[2] = '0';
                fonts.arr[1] = '.';
                fonts.arr[0] = '0';
            } else if (i / 100 == 0) {
                fonts.arr[3] = fonts.arr[1];
                fonts.arr[2] = fonts.arr[0];
                fonts.arr[1] = '.';
                fonts.arr[0] = '0';
            } else {
                fonts.arr[3] = fonts.arr[2];
                fonts.arr[2] = fonts.arr[1];
                fonts.arr[1] = '.';
            }
            fonts.arr[4] = ' ';
            fonts.arr[5] = 'm';
            fonts.arr[6] = 'A';
            fonts.arr[7] = '\0';
            fonts.drawString(163, 180, fonts.arr);
        }
        i++;
    }
}
