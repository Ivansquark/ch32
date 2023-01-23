#include "my_http.h"

Http* Http::pThis = nullptr;

Http::Http() {
    pThis = this;
    init();
}

void Http::httpHandler() {
    // int tempConnections = tcp.currentConnections;

    for (int i = 0; i < tcp.MAX_CONNECTIONS; i++) {
        if (!tcp.my_es[i].currentIndex && i > 0) { continue; }
        if (tcp.my_es[i].state == tcp.ES_ACCEPTED) {
            continue;
        } else if (tcp.my_es[i].state == tcp.ES_NONE) {
            continue;
        } else if (tcp.my_es[i].state == tcp.ES_RECEIVE) {
            continue;
        } else if (tcp.my_es[i].state == tcp.ES_CLOSE) {
            continue;
        } else if (tcp.my_es[i].state == tcp.ES_WRITE_ANSWER ||
                   tcp.my_es[i].retries > 0) {
            if (tcp.my_es[i].state != tcp.ES_WRITE_ANSWER) {
                // retryState
                if (retryCounter > MaxRetryCount) {
                    retryCounter = 0;
                } else {
                    retryCounter++;
                    continue;
                }
            }
            // TODO: check retries
            tcp.my_es[i].retries++;
            if (tcp.my_es[i].retries > 10) {
                // tcp.server_connection_close(pcb, &tcp.my_es[i]);
                continue;
            }
            // write answer
            err_t err = ERR_OK;
            if (!tcp.my_es[i].answer_pcb) {
                // tcp.server_connection_close(pcb, &tcp.my_es[i]);
                continue;
            }
            pcb = tcp.my_es[i].answer_pcb;
            switch (tcp.my_es[i].parseState) {
            case Tcp::GET_HTML: {
                uint16_t sizeHeadIndexHtml = W25q::pThis->SizeHeadIndexHtml;
                uint16_t sizeIndexHtml = W25q::pThis->SizeIndexHtml;
                uint8_t tempArr[sizeHeadIndexHtml + sizeIndexHtml];
                memcpy(tempArr, W25q::pThis->headIndexHtml, sizeHeadIndexHtml);
                memcpy(tempArr + sizeHeadIndexHtml, W25q::pThis->indexHtml,
                       sizeIndexHtml);
                err = tcp_write(pcb, (const void*)(tempArr),
                                sizeHeadIndexHtml + sizeIndexHtml,
                                TCP_WRITE_FLAG_COPY);
                // tcp_output(pcb); // send data now
                tcp.my_es[i].state = Tcp::ES_CLOSE;
                pbuf_free(tcp.my_es[i].p);
                // tcp.server_connection_close(pcb, &tcp.my_es[i]);
            } break;
            case Tcp::GET_ICO: {
                uint16_t sizeHeadIco = W25q::pThis->SizeHeadIco;
                uint16_t sizeIco = W25q::pThis->SizeIco;

                uint8_t tempArr[sizeHeadIco + sizeIco];
                memcpy(tempArr, W25q::pThis->headIco, sizeHeadIco);
                memcpy(tempArr + sizeHeadIco, W25q::pThis->ico, sizeIco);
                err = tcp_write(pcb, (const void*)(tempArr),
                                sizeHeadIco + sizeIco, TCP_WRITE_FLAG_COPY);
                // tcp_output(pcb); // send data now
                tcp.my_es[i].state = Tcp::ES_CLOSE;
                pbuf_free(tcp.my_es[i].p);
                // tcp.server_connection_close(pcb, &tcp.my_es[i]);
            } break;
            case Tcp::GET_CSS: {
                uint16_t sizeHeadCss = W25q::pThis->SizeHeadCss;
                uint16_t sizeCss = W25q::pThis->SizeCss;
                uint8_t tempArr[sizeHeadCss + sizeCss];
                memcpy(tempArr, W25q::pThis->headCss, sizeHeadCss);
                memcpy(tempArr + sizeHeadCss, W25q::pThis->css, sizeCss);
                err = tcp_write(pcb, (const void*)(tempArr),
                                sizeHeadCss + sizeCss, TCP_WRITE_FLAG_COPY);
                tcp.my_es[i].state = Tcp::ES_CLOSE;
                pbuf_free(tcp.my_es[i].p);
                // tcp.server_connection_close(pcb, &tcp.my_es[i]);
            } break;
            case Tcp::GET_JS: {
                uint16_t sizeHeadJs = W25q::pThis->SizeHeadJs;
                uint16_t sizeJs = W25q::pThis->SizeJs;
                uint8_t tempArr[sizeHeadJs + sizeJs];
                memcpy(tempArr, W25q::pThis->headJs, sizeHeadJs);
                memcpy(tempArr + sizeHeadJs, W25q::pThis->js, sizeJs);
                err = tcp_write(pcb, (const void*)(tempArr),
                                sizeHeadJs + sizeJs, TCP_WRITE_FLAG_COPY);
                tcp.my_es[i].state = Tcp::ES_CLOSE;
                pbuf_free(tcp.my_es[i].p);
                // tcp.server_connection_close(pcb, &tcp.my_es[i]);
            } break;
            case Tcp::GET_CONTENT: {
                // TODO: send answer
                uint16_t sizeHeadContent = W25q::pThis->SizeHeadContentStream;
                uint16_t sizeContent = 2;
                uint8_t tempArr[sizeHeadContent + sizeContent];
                memcpy(tempArr, W25q::pThis->headContentStream,
                       sizeHeadContent);
                debugVal = Adc::pThis->getAdc();
                memcpy(tempArr + sizeHeadContent, &debugVal, sizeContent);
                err = tcp_write(pcb, (const void*)(tempArr),
                                sizeHeadContent + sizeContent,
                                TCP_WRITE_FLAG_COPY);
                tcp.my_es[i].state = Tcp::ES_CLOSE;
                pbuf_free(tcp.my_es[i].p);
                // debugVal++;
                // tcp.server_connection_close(pcb, tcp.current_es);
            } break;
            case Tcp::GET_LED_ON: {
                Gpio::Out::setBlue();
                uint16_t sizeHeadContent = W25q::pThis->SizeHeadContentStream;
                uint16_t sizeContent = 1;
                uint8_t tempArr[sizeHeadContent + sizeContent];
                memcpy(tempArr, W25q::pThis->headContentStream,
                       sizeHeadContent);
                tempArr[sizeHeadContent - 1 - 4] = '1';
                bool state = true;
                memcpy(tempArr + sizeHeadContent, &state, sizeContent);
                err = tcp_write(pcb, (const void*)(tempArr),
                                sizeHeadContent + sizeContent,
                                TCP_WRITE_FLAG_COPY);
                tcp.my_es[i].state = Tcp::ES_CLOSE;
                pbuf_free(tcp.my_es[i].p);
                // tcp.server_connection_close(pcb, tcp.current_es);
            } break;
            case Tcp::GET_LED_OFF: {
                Gpio::Out::resetBlue();
                uint16_t sizeHeadContent = W25q::pThis->SizeHeadContentStream;
                uint16_t sizeContent = 1;
                uint8_t tempArr[sizeHeadContent + sizeContent];
                memcpy(tempArr, W25q::pThis->headContentStream,
                       sizeHeadContent);
                tempArr[sizeHeadContent - 1 - 4] = '1';
                bool state = false;
                memcpy(tempArr + sizeHeadContent, &state, sizeContent);
                err = tcp_write(pcb, (const void*)(tempArr),
                                sizeHeadContent + sizeContent,
                                TCP_WRITE_FLAG_COPY);
                tcp.my_es[i].state = Tcp::ES_CLOSE;
                pbuf_free(tcp.my_es[i].p);
                // tcp.server_connection_close(pcb, tcp.current_es);
            } break;
            default:
                tcp.my_es[i].state = Tcp::ES_CLOSE;
                pbuf_free(tcp.my_es[i].p);
                // tcp.server_connection_close(pcb, &tcp.my_es[i]);
                break;
            }
            if (err == ERR_MEM) {
                Gpio::Out::setBlue();
                tcp.my_es[i].state = Tcp::ES_CLOSE;
                // tcp.server_connection_close(pcb, &tcp.my_es[i]);
            }
        } else {
            // tcp.my_es[i].state = Tcp::ES_CLOSE;
            pcb = tcp.my_es[i].answer_pcb;
            if (pcb) { tcp.server_connection_close(pcb, &tcp.my_es[i]); }
        }
    }
}

void Http::init() {}
