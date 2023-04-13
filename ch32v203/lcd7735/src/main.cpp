#include "main.h"

#include "basic_timer.h"
#include "display.h"
#include "gpio.h"
#include "rcc.h"
#include "systim.h"

#include "opel_160_128.h"

Rcc rcc();

int main(void) {
    __enable_irq();
    Gpio::Out::init();
    Display display;
    display.ST7735_DrawImage(0, 0, 160, 128, image_data_OPEL160_128);
    display.writeString(0, 0, "opa", Font_16x26, RED, WHITE);

    while (1) {
        display.writeString(0, 0, "1", Font_16x26, RED, WHITE);
        display.writeString(0, 0, "12", Font_16x26, RED, WHITE);
        display.writeString(0, 0, "123", Font_16x26, RED, WHITE);
        display.writeString(0, 0, "1234", Font_16x26, RED, WHITE);
        display.writeString(0, 0, "123", Font_16x26, BLUE, WHITE);
        display.writeString(0, 0, "12", Font_16x26, BLUE, WHITE);
        display.writeString(0, 0, "1", Font_16x26, BLUE, WHITE);
    }
}
