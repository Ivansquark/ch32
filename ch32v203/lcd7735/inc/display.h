#ifndef DISPLAY_H
#define DISPLAY_H

#include "fonts.h"
#include "st7735.h"

extern FontDef Font_7x10;
extern FontDef Font_11x18;
extern FontDef Font_16x26;

class Display : public ST7735 {
  public:
    Display();
    void writeString(uint16_t x, uint16_t y, const char* str,
                            FontDef font, uint16_t color, uint16_t bgcolor);

  private:
    void writeChar(uint16_t x, uint16_t y, char ch, FontDef font,
                          uint16_t color, uint16_t bgcolor);
};

#endif // DISPLAY_H
