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
                tcp_output(pcb); // send data now
                tcp.current_es->state = Tcp::ES_CLOSE;
                //tcp.server_connection_close(pcb, tcp.current_es);
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
                tcp_output(pcb); // send data now
                tcp.current_es->state = Tcp::ES_CLOSE;
                //tcp.server_connection_close(pcb, tcp.current_es);
                //tcp.server_connection_close(pcb, nullptr);
            } break;
            case Http::GET_CSS: {
                //tcp.server_send(NULL, 0);
                tcp.current_es->state = Tcp::ES_CLOSE;
                //tcp.server_connection_close(pcb, tcp.current_es);
                //tcp.server_connection_close(pcb, nullptr);
            } break;
            case Http::GET_JS: {
                //tcp.server_send(NULL, 0);
                tcp.current_es->state = Tcp::ES_CLOSE;
                //tcp.server_connection_close(pcb, tcp.current_es);
                //tcp.server_connection_close(pcb, nullptr);
            } break;
            default:
                tcp.current_es->state = Tcp::ES_CLOSE;
                //tcp.server_connection_close(pcb, tcp.current_es);
                break;
            }
            //tcp.setIsDataIn(false, NULL, NULL);
        }
    }
}

void Http::init() {}
