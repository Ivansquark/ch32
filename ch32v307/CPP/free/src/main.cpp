#include "main.h"

#include "adc.h"
#include "basic_timer.h"
#include "eeprom.h"
#include "eth.h"
#include "gpio.h"
#include "my_http.h"
#include "my_tcp.h"
#include "my_udp.h"
#include "rcc.h"
#include "sd.h"
#include "w25q.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
enum ParseState : uint8_t
{
    NOT,
    GET_HTML,
    GET_ICO,
    GET_CSS,
    GET_JS,
    GET_CONTENT,
    GET_LED_ON,
    GET_LED_OFF
};
// objects in static memory (.data section)
Rcc rcc(8);
Adc adc1;
Eeprom eeprom;
W25q flash;
// Eth eth(192, 168, 0, 100);
Eth eth(192, 168, 1, 100);
// Http http;
// Sd sd;

void timeout();
void delay(volatile uint32_t val);

/* Global define */
#define TASK_HTTP_TASK_PRIO 1
#define TASK_HTTP_STK_SIZE 2048
#define TASK_RECEIVE_TASK_PRIO 5
#define TASK_RECEIVE_STK_SIZE 512
#define RECV_BUF_SIZE 2048
char recv_buf[RECV_BUF_SIZE] = {0};

/* Global Variable */
uint32_t SystemCoreClock = 144000000;
extern sys_mutex_t lock_tcpip_core;
TaskHandle_t TaskHttp_Handler;
TaskHandle_t TaskReceive_Handler;
TaskHandle_t TaskUdp_Handler;
TaskHandle_t TaskTcp_Handler;
xQueueHandle queue1;
char* queue_buf;

void receive_task([[maybe_unused]] void* pvParameters) {
    Gpio::Out::init();
    while (1) {
        eth.rx_handler();
        vTaskDelay(1); // every ms
    }
}
void http_task(void* pvParameters);

void tcp_task([[maybe_unused]] void* pvParameters) {
    int sock;
    struct sockaddr_in address, dest_addr;
    int size = sizeof(dest_addr);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        vTaskDelete(NULL); // Should delete this connection handler
        return;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("192.168.1.100");
    address.sin_port = htons(80);
    int err = bind(sock, (struct sockaddr*)&address, sizeof(address));
    if (err != ERR_OK) {
        vTaskDelete(NULL); // Should delete this connection handler
        return;
    }
    err = listen(sock, 4);
    if (err != ERR_OK) {
        vTaskDelete(NULL); // Should delete this connection handler
        return;
    }
    while (1) {
        // connected socket
        int con_sock =
            accept(sock, (struct sockaddr*)&dest_addr, (socklen_t*)&size);
        if (con_sock < 0) {
            close(con_sock);
            //vTaskDelete(NULL); // Should delete this connection handler
        } else if (con_sock > 0) {
            xTaskCreate((TaskFunction_t)http_task, (const char*)"http_task",
                        (uint16_t)TASK_HTTP_STK_SIZE, (void*)&con_sock,
                        (UBaseType_t)tskIDLE_PRIORITY, NULL);
            //(TaskHandle_t*)&TaskHttp_Handler);
        } else {
            // con_sock == 0
            //close(con_sock);
        }
        vTaskDelay(1); // every ms
    }
}

void udp_task([[maybe_unused]] void* pvParameters) {
    //----------- echo server -------------------------------------------------
    int sock;
    struct sockaddr_in address, dest_addr;
    int size = sizeof(dest_addr);
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) { return; }
    address.sin_family = AF_INET;
    address.sin_port = htons(55555);
    address.sin_addr.s_addr = INADDR_ANY;
    int err = bind(sock, (struct sockaddr*)&address, sizeof(address));
    if (err != ERR_OK) return;
    while (1) {
        int n = recvfrom(sock, (void*)recv_buf, RECV_BUF_SIZE, 0,
                         (struct sockaddr*)&dest_addr, (socklen_t*)&size);
        if (n < 0) {
            // error
            vTaskDelete(NULL); // Should delete this connection handler
            return;
        } else if (n > 0) {
            Gpio::Out::toggleBlue();
            // read complete
            recv_buf[n - 1] = ' ';
            recv_buf[n] = 'o';
            recv_buf[n + 1] = 'k';
            recv_buf[n + 2] = '\r';
            recv_buf[n + 3] = '\n';
            sendto(sock, recv_buf, n + 4, 0, (struct sockaddr*)&dest_addr,
                   size);
        }
        vTaskDelay(1); // every ms
    }
}

int main(void) {
    // initialize mutex for proper socket work
    sys_mutex_new(&lock_tcpip_core);

    xTaskCreate((TaskFunction_t)receive_task, (const char*)"receive_task",
                (uint16_t)TASK_RECEIVE_STK_SIZE, (void*)NULL,
                (UBaseType_t)TASK_RECEIVE_TASK_PRIO,
                (TaskHandle_t*)&TaskReceive_Handler);

    xTaskCreate((TaskFunction_t)udp_task, (const char*)"udp_task",
                (uint16_t)TASK_HTTP_STK_SIZE, (void*)NULL,
                (UBaseType_t)TASK_HTTP_TASK_PRIO,
                (TaskHandle_t*)&TaskUdp_Handler);

    xTaskCreate((TaskFunction_t)tcp_task, (const char*)"tcp_task",
                (uint16_t)TASK_HTTP_STK_SIZE, (void*)NULL,
                (UBaseType_t)TASK_HTTP_TASK_PRIO,
                (TaskHandle_t*)&TaskTcp_Handler);

    /* add queues, ... */
    queue1 = xQueueCreate(10, sizeof(uint32_t));

    vTaskStartScheduler();
    __enable_irq();
    BasicTimer6::Instance().setCallback(timeout);
    BasicTimer6::Instance().start(500);
    Gpio::Out::init();

    while (1) {}
}

void timeout() {
    // callback function from hardware timer
}

void delay(volatile uint32_t val) {
    while (val--) {}
}
ParseState parse(const uint8_t* data, uint16_t len);
void http_task(void* pvParameters) {
    int con_sock = *(int*)pvParameters;
    while (1) {
        int len = lwip_recv(con_sock, recv_buf, RECV_BUF_SIZE, 0);
        if (len > 0) {
            switch (parse((const uint8_t*)recv_buf, 7)) {
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

            //recv_buf[len - 1] = ' ';
            //recv_buf[len] = 'o';
            //recv_buf[len + 1] = 'k';
            //recv_buf[len + 2] = '\r';
            //recv_buf[len + 3] = '\n';
            //write(con_sock, recv_buf, len + 4);
        } else if (len < 0) {
            // close connection
            close(con_sock);
            vTaskDelete(NULL);
        }
        Gpio::Out::toggleBlue();
        vTaskDelay(1);
    }
}

ParseState parse(const uint8_t* data, uint16_t len) {
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
