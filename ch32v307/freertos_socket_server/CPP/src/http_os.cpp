#include "http_os.h"

HttpOS::HttpOS(const char* recv_buf, uint16_t stackSize, BaseType_t priority,
               void* pvParameters)
    : FR_OS(stackSize, priority, pvParameters) {

        FR_OS::init_FR_OS();
    this->recv_buf = (char*)recv_buf;
}

HttpOS::~HttpOS() { 
    //vTaskDelete(NULL);
}

void HttpOS::runTask([[maybe_unused]] void* pvParameters) {
    int con_sock = *(int*)pvParameters;
    while (1) {
        int len = lwip_recv(con_sock, recv_buf, RECV_BUF_SIZE, 0);
        if (len > 0) {
            switch (parse(recv_buf, 7)) {
            case NOT:
                break;
            case GET_HTML: {
                uint16_t sizeHeadIndexHtml = W25q::pThis->SizeHeadIndexHtml;
                uint16_t sizeIndexHtml = W25q::pThis->SizeIndexHtml;
                uint8_t tempArr[sizeHeadIndexHtml + sizeIndexHtml];
                memcpy(tempArr, W25q::pThis->headIndexHtml, sizeHeadIndexHtml);
                memcpy(tempArr + sizeHeadIndexHtml, W25q::pThis->indexHtml,
                       sizeIndexHtml);
                write(con_sock, tempArr, sizeof(tempArr));
            } break;
            case GET_ICO: {
                uint16_t sizeHeadIco = W25q::pThis->SizeHeadIco;
                uint16_t sizeIco = W25q::pThis->SizeIco;

                uint8_t tempArr[sizeHeadIco + sizeIco];
                memcpy(tempArr, W25q::pThis->headIco, sizeHeadIco);
                memcpy(tempArr + sizeHeadIco, W25q::pThis->ico, sizeIco);
                write(con_sock, tempArr, sizeof(tempArr));
            } break;
            case GET_CSS: {
                uint16_t sizeHeadCss = W25q::pThis->SizeHeadCss;
                uint16_t sizeCss = W25q::pThis->SizeCss;
                uint8_t tempArr[sizeHeadCss + sizeCss];
                memcpy(tempArr, W25q::pThis->headCss, sizeHeadCss);
                memcpy(tempArr + sizeHeadCss, W25q::pThis->css, sizeCss);
                write(con_sock, tempArr, sizeof(tempArr));
            } break;
            case GET_JS: {
                uint16_t sizeHeadJs = W25q::pThis->SizeHeadJs;
                uint16_t sizeJs = W25q::pThis->SizeJs;
                uint8_t tempArr[sizeHeadJs + sizeJs];
                memcpy(tempArr, W25q::pThis->headJs, sizeHeadJs);
                memcpy(tempArr + sizeHeadJs, W25q::pThis->js, sizeJs);
                write(con_sock, tempArr, sizeof(tempArr));
            } break;
            case GET_CONTENT: {
                // send answer
                uint16_t sizeHeadContent = W25q::pThis->SizeHeadContentStream;
                uint16_t sizeContent = 2;
                uint8_t tempArr[sizeHeadContent + sizeContent];
                memcpy(tempArr, W25q::pThis->headContentStream,
                       sizeHeadContent);
                int debugVal = Adc::pThis->getAdc();
                memcpy(tempArr + sizeHeadContent, &debugVal, sizeContent);
                write(con_sock, tempArr, sizeof(tempArr));
            } break;
            case GET_LED_ON: {
                Gpio::Out::setBlue();
                uint16_t sizeHeadContent = W25q::pThis->SizeHeadContentStream;
                uint16_t sizeContent = 1;
                uint8_t tempArr[sizeHeadContent + sizeContent];
                memcpy(tempArr, W25q::pThis->headContentStream,
                       sizeHeadContent);
                tempArr[sizeHeadContent - 1 - 4] = '1';
                bool state = true;
                memcpy(tempArr + sizeHeadContent, &state, sizeContent);
                write(con_sock, tempArr, sizeof(tempArr));
            } break;
            case GET_LED_OFF: {
                Gpio::Out::resetBlue();
                uint16_t sizeHeadContent = W25q::pThis->SizeHeadContentStream;
                uint16_t sizeContent = 1;
                uint8_t tempArr[sizeHeadContent + sizeContent];
                memcpy(tempArr, W25q::pThis->headContentStream,
                       sizeHeadContent);
                tempArr[sizeHeadContent - 1 - 4] = '1';
                bool state = false;
                memcpy(tempArr + sizeHeadContent, &state, sizeContent);
                write(con_sock, tempArr, sizeof(tempArr));
            } break;
            }
            // close connection
            close(con_sock);
            vTaskDelete(NULL);

            // recv_buf[len - 1] = ' ';
            // recv_buf[len] = 'o';
            // recv_buf[len + 1] = 'k';
            // recv_buf[len + 2] = '\r';
            // recv_buf[len + 3] = '\n';
            // write(con_sock, recv_buf, len + 4);
        } else if (len < 0) {
            // close connection
            close(con_sock);
            vTaskDelete(NULL);
        }
        Gpio::Out::toggleBlue();
        vTaskDelay(1);
    }
}

HttpOS::ParseState HttpOS::parse(const char* data, uint16_t len) {
    if (!len) { return ParseState::NOT; }
    if (data[0] == 'G' && data[1] == 'E' && data[2] == 'T') {
        if (data[4] == '/' && data[5] == 0x20) {
            // GET /
            return ParseState::GET_HTML;
        } else if (data[4] == '/' && data[5] == 'f') {
            // GET /facicon.ico
            return ParseState::GET_ICO;
        } else if (data[4] == '/' && data[5] == 's' && data[6] == 't') {
            // GET /style.css
            return ParseState::GET_CSS;
        } else if (data[4] == '/' && data[5] == 's' && data[6] == 'c') {
            // GET /script.js
            return ParseState::GET_JS;
        } else if (data[4] == '/' && data[5] == 'c' && data[6] == 'o') {
            // GET /content.bin
            return ParseState::GET_CONTENT;
        } else if (data[4] == '/' && data[5] == 'o' && data[6] == 'n') {
            // GET /on
            return ParseState::GET_LED_ON;
        } else if (data[4] == '/' && data[5] == 'o' && data[6] == 'f') {
            // GET /off
            return ParseState::GET_LED_OFF;
        }
    } else {
        return ParseState::NOT;
    }
    return ParseState::NOT;
}
