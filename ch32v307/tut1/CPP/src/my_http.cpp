#include "my_http.h"

Http* Http::pThis = nullptr;

Http::Http() {
    pThis = this;
    init();
}

Http::ParseState Http::parse(const uint8_t* data, uint16_t len) {
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
        }
    } else {
        return ParseState::NOT;
    }
    return ParseState::NOT;
}

void Http::httpHandler() {
    if (!tcp.getIsDataIn()) {
        return;
    } else {
        if (tcp.current_tcp_pcb) {
            tcp.IsDataIn = false;
            pcb = tcp.current_tcp_pcb;
            currentParseState = parse(Eth::RxBuff, Eth::currentRxBuffLen);
            switch (currentParseState) {
            case Http::GET_HTML: {
                // TODO: send answer
                uint16_t sizeHeadIndexHtml = W25q::pThis->SizeHeadIndexHtml;
                uint16_t sizeIndexHtml = W25q::pThis->SizeIndexHtml;
                uint8_t arr[sizeHeadIndexHtml + sizeIndexHtml];
                W25q::pThis->read((uint8_t*)arr,
                                  W25q::pThis->AddressHeadIndexHtml,
                                  sizeHeadIndexHtml);
                W25q::pThis->read(((uint8_t*)arr + sizeHeadIndexHtml),
                                  W25q::pThis->AddressIndexHtml, sizeIndexHtml);
                // tcp.server_send(arr, sizeHeadIndexHtml + sizeIndexHtml);
                tcp_write(pcb, (const void*)(arr),
                          sizeHeadIndexHtml + sizeIndexHtml,
                          TCP_WRITE_FLAG_COPY);
                // tcp_output(pcb); // send data now
                tcp.current_es->state = Tcp::ES_CLOSE;
                pbuf_free(tcp.current_es->p);
                tcp.server_connection_close(pcb, tcp.current_es);
            } break;
            case Http::GET_ICO: {
                uint16_t sizeHeadIco = W25q::pThis->SizeHeadIco;
                uint16_t sizeIco = W25q::pThis->SizeIco;
                uint8_t arr[sizeHeadIco + sizeIco];
                W25q::pThis->read((uint8_t*)arr, W25q::pThis->AddressHeadIco,
                                  sizeHeadIco);
                W25q::pThis->read(((uint8_t*)arr + sizeHeadIco),
                                  W25q::pThis->AddressIco, sizeIco);
                tcp.server_send(arr, sizeHeadIco + sizeIco);
                tcp_write(pcb, (const void*)(arr), sizeHeadIco + sizeIco,
                          TCP_WRITE_FLAG_COPY);
                // tcp_output(pcb); // send data now
                tcp.current_es->state = Tcp::ES_CLOSE;
                pbuf_free(tcp.current_es->p);
                tcp.server_connection_close(pcb, tcp.current_es);
                // tcp.server_connection_close(pcb, nullptr);
            } break;
            case Http::GET_CSS: {
                uint16_t sizeHeadCss = W25q::pThis->SizeHeadCss;
                uint16_t sizeCss = W25q::pThis->SizeCss;
                uint8_t tempArr[sizeHeadCss + sizeCss];
                memcpy(tempArr, W25q::pThis->headCss, sizeHeadCss);
                memcpy(tempArr + sizeHeadCss, W25q::pThis->css, sizeCss);
                tcp_write(pcb, (const void*)(tempArr), sizeHeadCss + sizeCss,
                          TCP_WRITE_FLAG_COPY);
                tcp.current_es->state = Tcp::ES_CLOSE;
                pbuf_free(tcp.current_es->p);
                tcp.server_connection_close(pcb, tcp.current_es);
                // tcp.server_connection_close(pcb, nullptr);
            } break;
            case Http::GET_JS: {
                uint16_t sizeHeadJs = W25q::pThis->SizeHeadJs;
                uint16_t sizeJs = W25q::pThis->SizeJs;
                uint8_t tempArr[sizeHeadJs + sizeJs];
                memcpy(tempArr, W25q::pThis->headJs, sizeHeadJs);
                memcpy(tempArr + sizeHeadJs, W25q::pThis->js, sizeJs);
                tcp_write(pcb, (const void*)(tempArr), sizeHeadJs + sizeJs,
                          TCP_WRITE_FLAG_COPY);
                tcp.current_es->state = Tcp::ES_CLOSE;
                pbuf_free(tcp.current_es->p);
                tcp.server_connection_close(pcb, tcp.current_es);
                // tcp.server_connection_close(pcb, nullptr);
            } break;
            case Http::GET_CONTENT: {
                // TODO: send answer
                uint16_t sizeHeadContent = W25q::pThis->SizeHeadContentStream;
                uint16_t sizeContent = 2;
                uint8_t tempArr[sizeHeadContent + sizeContent];
                memcpy(tempArr, W25q::pThis->headContentStream,
                       sizeHeadContent);
                memcpy(tempArr + sizeHeadContent, &debugVal, sizeContent);
                tcp_write(pcb, (const void*)(tempArr),
                          sizeHeadContent + sizeContent, TCP_WRITE_FLAG_COPY);
                // tcp.current_es->state = Tcp::ES_CLOSE;
                pbuf_free(tcp.current_es->p);
                // tcp.server_connection_close(pcb, tcp.current_es);
                // TODO: try not to close conection
            } break;
            default:
                tcp.current_es->state = Tcp::ES_CLOSE;
                pbuf_free(tcp.current_es->p);
                // tcp.server_connection_close(pcb, tcp.current_es);
                break;
            }
        }
    }
}

void Http::init() {}
