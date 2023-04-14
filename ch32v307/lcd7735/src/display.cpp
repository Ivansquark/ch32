#include "display.h"

Display::Display() {}

void Display::writeChar(uint16_t x, uint16_t y, char ch, FontDef font,
                        uint16_t color, uint16_t bgcolor) {
    uint32_t i, b, j;

    ST7735_SetAddressWindow(x, y, x + font.width - 1, y + font.height - 1);

    for (i = 0; i < font.height; i++) {
        b = font.data[(ch - 32) * font.height + i];
        for (j = 0; j < font.width; j++) {
            if ((b << j) & 0x8000) {
                uint8_t data[] = {(uint8_t)(color >> 8),
                                  (uint8_t)(color & 0xFF)};
                ST7735_WriteData(data, sizeof(data));
            } else {
                uint8_t data[] = {(uint8_t)(bgcolor >> 8),
                                  (uint8_t)(bgcolor & 0xFF)};
                ST7735_WriteData(data, sizeof(data));
            }
        }
    }
}

void Display::writeString(uint16_t x, uint16_t y, const char* str, FontDef font,
                          uint16_t color, uint16_t bgcolor) {
    ST7735_Select();

    while (*str) {
        if (x + font.width >= _width) {
            x = 0;
            y += font.height;
            if (y + font.height >= _height) { break; }

            if (*str == ' ') {
                // skip spaces in the beginning of the new line
                str++;
                continue;
            }
        }

        writeChar(x, y, *str, font, color, bgcolor);
        x += font.width;
        str++;
    }

    ST7735_Unselect();
}
