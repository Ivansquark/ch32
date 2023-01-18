#ifndef W25Q_H
#define W25Q_H

#include "ch32v30x_spi.h"
#include "main.h"
#include <string.h>

class W25q {
  public:
    W25q();
    static W25q* pThis;

    // Http server info
    //
    // TODO: make ffs system

    // static constexpr const char* headIndexHtml =
    //    "HTTP/1.1 200 OK\r\nServer: nginx\r\nContent-Type:
    //    text/html\r\nConnection: " "keep-alive\r\n\r\n";
    static constexpr const char* headIndexHtml =
        "HTTP/1.1 200 OK\n"
        "Content-Type: text/html; charset=UTF-8\n"
        "Content-Length: 733\r\n\r\n";
    static constexpr const char* indexHtml =
        "<!DOCTYPE html>"
        "<html lang=\"en\">"
        "   <head>"
        "       <meta charset=\"UTF-8\" />"
        "       <meta name=\"viewport\" content=\"width=device-width, "
        "       initial-scale=1.0\" />"
        "       <link rel=\"stylesheet\" href=\"style.css\" />"
        "       <title>Progress Steps</title>"
        "   </head>"
        "   <body>"
        "       <div class=\"container\">"
        "       <div class=\"progress-container\">"
        "       <div class=\"progress\" id=\"progress\"></div>"
        "       <div class=\"circle active\">1</div>"
        "       <div class=\"circle\">2</div>"
        "       <div class=\"circle\">3</div>"
        "       </div>"
        "           <button class=\"btn\" id=\"prev\" disabled>Prev</button>"
        "           <button class=\"btn\" id=\"next\">Next</button>"
        "           <button class=\"btn\" id=\"start\">START</button>"
        "       </div>"
        "       <div style=\"background-color:#8ebf42\">"
        "           <!-- Paragraph -->"
        "           <p>"
        "               <span style=\"color:#1c87c9\">"
        "                   OPA"
        "               </span>"
        "           </p>"
        "       </div>"
        "       <p></p>"
        "       <p></p>"
        "       <canvas id=\"myChart\" width=\"700\" height=\"400\"></canvas>"
        "       <p></p>"
        "       <p></p>"
        "       <script src=\"script.js\"></script>"
        "   </body>"
        "   <body onload=\"onload()\">"
        "</html>";
    static constexpr const char* headIco =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: image/png; charset=UTF-8\r\n"
        "Content-Length: 539\r\n\r\n";

    static constexpr unsigned char ico[539] = {
        0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x32, 0x2a, 0x02, 0x00, 0x01, 0x00,
        0x01, 0x00, 0x05, 0x02, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x89, 0x50,
        0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d, 0x49, 0x48,
        0x44, 0x52, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x2a, 0x08, 0x06,
        0x00, 0x00, 0x00, 0xf1, 0xba, 0x08, 0x47, 0x00, 0x00, 0x01, 0xcc, 0x49,
        0x44, 0x41, 0x54, 0x68, 0x81, 0xed, 0x59, 0x5b, 0x6e, 0xc4, 0x20, 0x0c,
        0x34, 0x55, 0x2f, 0xd1, 0xdb, 0xf6, 0xa8, 0xbd, 0x49, 0xf6, 0xa7, 0x91,
        0x5c, 0xc7, 0x8f, 0xf1, 0x03, 0x65, 0xbb, 0xda, 0x91, 0x22, 0x04, 0x21,
        0xc0, 0x30, 0x60, 0x6c, 0xb2, 0xbe, 0xbe, 0x7f, 0xe8, 0x15, 0xf0, 0x71,
        0xf7, 0x00, 0xa6, 0xf0, 0x26, 0xf2, 0x6c, 0xf8, 0xdc, 0xd0, 0xe6, 0x41,
        0x44, 0xeb, 0x37, 0x95, 0x58, 0xec, 0xfd, 0x28, 0xa6, 0x15, 0x39, 0x44,
        0xca, 0xb1, 0x94, 0xf7, 0x5a, 0xbd, 0x12, 0x26, 0x88, 0x1c, 0xec, 0x91,
        0x90, 0x33, 0xcf, 0xf3, 0xa3, 0x64, 0xba, 0x44, 0xf8, 0x20, 0x16, 0x4b,
        0x97, 0xc8, 0x93, 0x52, 0x8f, 0xd7, 0x6d, 0x93, 0xe9, 0x10, 0x91, 0x24,
        0xac, 0x19, 0x46, 0x95, 0xf2, 0x94, 0x0d, 0x51, 0x25, 0x72, 0x6e, 0x58,
        0x3e, 0xf3, 0x7c, 0x50, 0x5e, 0x9e, 0x97, 0x6b, 0x8a, 0x96, 0x50, 0x21,
        0xe2, 0xcd, 0x98, 0x36, 0x98, 0x68, 0x86, 0x35, 0x32, 0x69, 0x55, 0xb2,
        0x44, 0xac, 0x0e, 0x3c, 0x93, 0x9a, 0x9d, 0xe9, 0x92, 0x32, 0x99, 0x73,
        0x04, 0x59, 0xfb, 0xc8, 0x77, 0x1c, 0x7c, 0x6f, 0x68, 0xe5, 0x30, 0xa9,
        0x8c, 0x22, 0x96, 0x55, 0xe2, 0x40, 0x36, 0xb6, 0x55, 0x5f, 0xb3, 0x76,
        0xf0, 0x12, 0xcb, 0x2a, 0x62, 0x59, 0x1b, 0xcf, 0x6a, 0x51, 0x50, 0xee,
        0xf5, 0x67, 0xf5, 0x7b, 0x01, 0x4a, 0x04, 0x69, 0xcc, 0x52, 0x48, 0x92,
        0xcd, 0x28, 0x09, 0x2f, 0x2f, 0x64, 0x69, 0x75, 0x4e, 0xe0, 0x8c, 0x89,
        0x8d, 0xdc, 0x1a, 0x17, 0x08, 0x11, 0xeb, 0x84, 0xb6, 0x3a, 0xb7, 0xbe,
        0xef, 0xd4, 0x0b, 0xfb, 0xc9, 0x28, 0x52, 0x05, 0x4a, 0x36, 0x7b, 0x3e,
        0xfd, 0x41, 0x46, 0x91, 0x5d, 0xef, 0x89, 0x30, 0x13, 0xed, 0xa2, 0xa2,
        0xc8, 0xdd, 0x79, 0x15, 0x88, 0xd5, 0xd2, 0x0e, 0xad, 0x4a, 0xe7, 0xde,
        0xf7, 0xb2, 0x7c, 0x8b, 0x8b, 0x62, 0xb9, 0xea, 0xe4, 0x94, 0x67, 0xeb,
        0x65, 0xdb, 0xb9, 0xa0, 0x62, 0xb5, 0xee, 0x48, 0x47, 0xf7, 0x48, 0xd5,
        0xce, 0x77, 0x7d, 0x32, 0xe8, 0x50, 0xfc, 0x2f, 0xe7, 0x48, 0x88, 0x8c,
        0xd3, 0x58, 0x75, 0x08, 0xd1, 0x0d, 0x1c, 0x19, 0x08, 0x17, 0xa8, 0xaf,
        0x15, 0x2d, 0x21, 0xeb, 0x0a, 0x28, 0x13, 0xf6, 0xb6, 0xe2, 0x99, 0xa9,
        0x98, 0xfd, 0xcc, 0x7b, 0xae, 0xbe, 0x75, 0x21, 0x91, 0x89, 0xf5, 0x4d,
        0x64, 0xe3, 0x11, 0x6d, 0xb9, 0xa0, 0x26, 0x55, 0xe6, 0x35, 0xa2, 0x5b,
        0xcf, 0x11, 0x09, 0xcb, 0x9b, 0xed, 0xc4, 0x21, 0x96, 0xa9, 0xdd, 0x12,
        0x21, 0x9e, 0x0d, 0xa3, 0x33, 0x8f, 0x0c, 0xc6, 0x0a, 0xd6, 0xa2, 0xef,
        0x2e, 0xe8, 0x28, 0x62, 0xdd, 0x6b, 0x59, 0xf5, 0x91, 0x76, 0x4e, 0x57,
        0x28, 0x6d, 0x8a, 0x2b, 0x44, 0xb8, 0x2a, 0x08, 0x99, 0x48, 0x29, 0x54,
        0x59, 0x17, 0x13, 0xb7, 0xf1, 0x5a, 0xcc, 0x1e, 0xed, 0x9d, 0xb3, 0x6c,
        0xec, 0x92, 0xae, 0x63, 0x7e, 0xb9, 0x89, 0xf5, 0x4c, 0xa7, 0xa7, 0xc8,
        0xd8, 0x4d, 0xe3, 0xd4, 0xff, 0x91, 0xca, 0x61, 0x18, 0x5d, 0x48, 0xa4,
        0x30, 0xf9, 0x7f, 0x44, 0xbb, 0x97, 0x92, 0xef, 0xbd, 0xb4, 0x85, 0x1d,
        0x7f, 0xac, 0x88, 0x70, 0x32, 0x63, 0x78, 0x99, 0x7f, 0x88, 0x6f, 0x22,
        0xcf, 0x86, 0x07, 0xcc, 0x3c, 0x8e, 0xfc, 0xbe, 0xd8, 0x35, 0x28, 0x00,
        0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82};

    static constexpr const char* headCss =
        "HTTP/1.1 200 OK"
        "Content-Type: text/css; charset=UTF-8"
        "Content-Length: 1610\r\n\r\n";

    static constexpr const char* css =
        ":root {"
        "   --line-border-fill: #3498db;"
        "   --line-border-empty: #e0e0e0;"
        "}"
        "* {"
        "box-sizing: border-box;"
        "}"
        "body {"
        "   font-family: \"Muli\", sans-serif;"
        "   background-color: #f6f7fb;"
        "   display: flex;"
        "   align-items: center;"
        "   justify-content: center;"
        "   height: 100vh;"
        "   margin: 10px;"
        "}"
        ".container {"
        "   text-align: center;"
        "}"
        ".progress-container {"
        "   display: flex;"
        "   justify-content: space-between;"
        "   position: relative;"
        "   margin-bottom: 30px;"
        "   max-width: 100%;"
        "   width: 350px;"
        "}"
        ".progress-container::before {"
        "   content: \"\";"
        "   background-color: var(--line-border-empty);"
        "   position: absolute;"
        "   top: 50%;"
        "   left: 0;"
        "   transform: translateY(-50%);"
        "   height: 4px;"
        "   width: 100%;"
        "   z-index: -1;"
        "}"
        ".progress {"
        "   background-color: var(--line-border-fill);"
        "   position: absolute;"
        "   top: 50%;"
        "   left: 0;"
        "   transform: translateY(-50%);"
        "   height: 4px;"
        "   width: 0%;"
        "   z-index: -1;"
        "   transition: 0.4s ease;"
        "}"
        ".circle {"
        "   background-color: #fff;"
        "   color: #999;"
        "   border-radius: 50%;"
        "   height: 30px;"
        "   width: 30px;"
        "   display: flex;"
        "   align-items: center;"
        "   justify-content: center;"
        "   border: 3px solid var(--line-border-empty);"
        "   transition: 0.4s ease;"
        "}"
        ".circle.active {"
        "   border-color: var(--line-border-fill);"
        "}"
        ".btn {"
        "   background-color: var(--line-border-fill);"
        "   color: #fff;"
        "   border: 0;"
        "   border-radius: 6px;"
        "   cursor: pointer;"
        "   font-family: inherit;"
        "   padding: 8px 30px;"
        "   margin: 5px;"
        "   font-size: 14px;"
        "}"
        ".btn:active {"
        "   transform: scale(0.98);"
        "}"
        ".btn:focus {"
        "   outline: 0;"
        "}"
        ".btn:disabled {"
        "   background-color: var(--line-border-empty);"
        "   cursor: not-allowed;"
        "}";

    static constexpr const char* headJs =
        "HTTP/1.1 200 OK"
        "Content-Type: text/javascript; charset=UTF-8"
        "Content-Length: 994\r\n\r\n";

    static constexpr const char* js =
        "const progress = document.getElementById(\"progress\");"
        "const prev = document.getElementById(\"prev\");"
        "const next = document.getElementById(\"next\");"
        "const start = document.getElementById(\"start\");"
        "const circles = document.querySelectorAll(\".circle\");"
        "let currentActive = 1;"
        "next.addEventListener(\"click\", () => {"
        "   currentActive++;"
        "   if (currentActive > circles.length) currentActive = circles.length;"
        "   update();"
        "});"
        "prev.addEventListener(\"click\", () => {"
        "   currentActive--;"
        "   if (currentActive < 1) currentActive = 1;"
        "   update();"
        "});"
        "start.addEventListener(\"click\", startstring);"
        "const update = () => {"
        "   circles.forEach((circle, index) => {"
        "       if (index < currentActive) {"
        "           circle.classList.add(\"active\");"
        "       } else {"
        "           circle.classList.remove(\"active\");"
        "       }"
        "   });"
        "   const actives = document.querySelectorAll(\".active\");"
        "       progress.style.width ="
        "       ((actives.length - 1) / (circles.length - 1)) * 100 + \"%\";"
        "       if (currentActive === 1) {"
        "           prev.disabled = true;"
        "           document.getElementById(\"info\").textContent=\"START\";"
        "       } else if (currentActive === circles.length) {"
        "           next.disabled = true;"
        "           document.getElementById(\"info\").textContent=\"END\";"
        "       } else {"
        "           prev.disabled = false;"
        "           next.disabled = false;"
        "   }"
        "};"
        "var xhr;"
        "var idTimer1"
        "var temp = false;"
        "var ctx = document.getElementById(\"myChart\");"
        "function Timer1() {"
        "   xhr.open(\"GET\", \"content.bin?r=\" + Math.random(), true);"
        "   xhr.responseType = \"arraybuffer\";"
        "   xhr.onload = function (oEvent) {"
        "       var uint16 = new Uint16(this.response);"
        "       myChart.data = uint16;"
        "       myChart.update();"
        "       draw(uint16);"
        "   }"
        "   xhr.send(null);"
        "   idTimer1 = setTimeout(\"Timer1()\", 1000);"
        "}"
        "var IsStarted = false;"
        "function startstring() {"
        "   if(!IsStarted) {"
        "       IsStarted = true;"
        "       document.getElementById('start').innerHTML = 'STOP';"
        "       Timer1();"
        "   } else {"
        "       IsStarted = false;"
        "       clearTimeout(idTimer1);"
        "       document.getElementById('start').innerHTML = 'START';"
        "   }"
        "}"
        "function onload() {"
        "   xhr = new(XMLHttpRequest);"
        "}"
        "function draw(i) {"
        "   var V = 3*i/4096"
        "   var canvas = document.getElementById('myChart');"
        "   if (canvas.getContext) {"
        "       var ctx = canvas.getContext('2d');"
        "       ctx.clearRect(0, 0, canvas.width, canvas.height);"
        "       ctx.fillStyle = \"red\";"
        "       ctx.fillRect(0, 0, canvas.width*V/3, 50);"
        "       ctx.fillStyle = \"blue\";"
        "       ctx.font = \"italic 30pt Arial\";"
        "       ctx.fillText(V.toFixed(3) + \"V\", 35, 150);"
        "   }"
        "}";

    static constexpr const char* headContentStream =
        "HTTP/1.1 200 OK"
        "Content-Type: application/octet-stream"
        "Content-Length: 2 \r\n\r\n";

    // int constexpr length(const char* str) {
    //    return *str ? 1 + length(str + 1) : 0;
    //}

    void writeHtml();
    static constexpr uint16_t AddressHeadIndexHtml = 0x00;
    const uint16_t AddressIndexHtml = strlen(headIndexHtml);
    const uint16_t AddressHeadIco = AddressIndexHtml + strlen(indexHtml);
    const uint16_t AddressIco = AddressHeadIco + strlen(headIco);

    const uint16_t SizeHeadIndexHtml = strlen(headIndexHtml);
    const uint16_t SizeIndexHtml = strlen(indexHtml);
    const uint16_t SizeHeadIco = strlen(headIco);
    const uint16_t SizeIco = sizeof(ico);

    //@brief Write data to flash.(no need Erase)
    void write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t size);
    void read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t size);
    void erase_Sector(uint32_t Dst_Addr);
    uint16_t readID(void);
    uint16_t readSR(void);
    void reset();
    void WAKEUP(void);

    /* Winbond SPIFalsh ID */
    static constexpr uint16_t W25Q80 = 0XEF13;
    static constexpr uint16_t W25Q16 = 0XEF14;
    static constexpr uint16_t W25Q32 = 0XEF15;
    static constexpr uint16_t W25Q64 = 0XEF16;
    static constexpr uint16_t W25Q128 = 0XEF17;

  private:
    void init();
    void delay(volatile uint32_t val);
    uint16_t readWriteByte(uint8_t TxData);
    void writeSR(uint8_t sr);
    void wait_Busy(void);
    void write_Enable(void);
    void write_Disable(void);
    void write_Page(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t size);
    void write_NoCheck(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t size);
    void erase_Chip(void);
    void powerDown(void);

    inline void setCS() { GPIOA->BSHR |= GPIO_BSHR_BR2; }
    inline void resetCS() { GPIOA->BSHR |= GPIO_BSHR_BS2; }

    /* Winbond SPIFalsh Instruction List */
    static constexpr uint16_t W25X_WriteEnable = 0x06;
    static constexpr uint16_t W25X_WriteDisable = 0x04;
    static constexpr uint16_t W25X_ReadStatusReg = 0x05;
    static constexpr uint16_t W25X_WriteStatusReg = 0x01;
    static constexpr uint16_t W25X_ReadData = 0x03;
    static constexpr uint16_t W25X_FastReadData = 0x0B;
    static constexpr uint16_t W25X_FastReadDual = 0x3B;
    static constexpr uint16_t W25X_PageProgram = 0x02;
    static constexpr uint16_t W25X_BlockErase = 0xD8;
    static constexpr uint16_t W25X_SectorErase = 0x20;
    static constexpr uint16_t W25X_ChipErase = 0xC7;
    static constexpr uint16_t W25X_PowerDown = 0xB9;
    static constexpr uint16_t W25X_ReleasePowerDown = 0xAB;
    static constexpr uint16_t W25X_EnableReset = 0x66;
    static constexpr uint16_t W25X_Reset = 0x99;
    static constexpr uint16_t W25X_DeviceID = 0xAB;
    static constexpr uint16_t W25X_ManufactDeviceID = 0x90;
    static constexpr uint16_t W25X_JedecDeviceID = 0x9F;

    uint8_t SPI_FLASH_BUF[4096] = {0};

  public:
};

#endif // W25Q_H
