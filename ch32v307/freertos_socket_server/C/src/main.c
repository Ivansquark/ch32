#include "main.h"

#include "adc.h"
#include "basic_timer.h"
#include "eeprom.h"
#include "eth.h"
#include "gpio.h"
#include "rcc.h"
#include "w25q.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

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

// objects in static memory (.data section)
void timeout();

void receive_task(__attribute__((unused)) void* pvParameters);
void udp_task(__attribute__((unused)) void* pvParameters);
void tcp_task(__attribute__((unused)) void* pvParameters);
void http_task(void* pvParameters);

void setRamSize(uint32_t size);
int main(void)
{

    //setRamSize(0x20000);
    //-------------- inits -------------------------
    Rcc_init(8);
    gpio_init();
    Adc_init();
    Eeprom_init();
    W25_init();
    BT6_init();
    Eth_init(192, 168, 1, 100);
    //----------------------------------------------

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
    // add queues, ... 
    queue1 = xQueueCreate(10, sizeof(uint32_t));

    vTaskStartScheduler();
    // not go bellow (for debug)
    __enable_irq();
    BT6_setCallback(timeout);
    BT6_start(500);

    while (1) {
        Eth_rx_handler();
    }
}

void timeout()
{
    // callback function from hardware timer
    gpio_toggleRed();
}

void udp_task(__attribute__((unused)) void* pvParameters)
{
    //----------- echo server -------------------------------------------------
    int sock;
    struct sockaddr_in address, dest_addr;
    int size = sizeof(dest_addr);
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        return;
    }
    address.sin_family = AF_INET;
    address.sin_port = htons(55555);
    address.sin_addr.s_addr = INADDR_ANY;
    int err = bind(sock, (struct sockaddr*)&address, sizeof(address));
    if (err != ERR_OK)
        return;
    while (1) {
        int n = recvfrom(sock, (void*)recv_buf, RECV_BUF_SIZE, 0,
                         (struct sockaddr*)&dest_addr, (socklen_t*)&size);
        if (n < 0) {
            // error
            vTaskDelete(NULL); // Should delete this connection handler
            return;
        } else if (n > 0) {
            gpio_toggleBlue();
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
void tcp_task(__attribute__((unused)) void* pvParameters)
{
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
            // vTaskDelete(NULL); // Should delete this connection handler
        } else if (con_sock > 0) {
             xTaskCreate((TaskFunction_t)http_task, (const char*)"http_task",
                         (uint16_t)TASK_HTTP_STK_SIZE, (void*)&con_sock,
                         (UBaseType_t)tskIDLE_PRIORITY, NULL);
            //(TaskHandle_t*)&TaskHttp_Handler);
            //http_task(&con_sock);
        } else {
            // con_sock == 0
            // close(con_sock);
        }
        vTaskDelay(1); // every ms
    }
}

void receive_task(__attribute__((unused)) void* pvParameters)
{
    gpio_init();
    while (1) {
        Eth_rx_handler();
        Eth_net_led_tmr();
        vTaskDelay(1); // every ms
    }
}

enum ParseState {
    NOT,
    GET_HTML,
    GET_ICO,
    GET_CSS,
    GET_JS,
    GET_CONTENT,
    GET_LED_ON,
    GET_LED_OFF
};
enum ParseState parse(const uint8_t* data, uint16_t len);
void http_task(void* pvParameters)
{
    int con_sock = *(int*)pvParameters;
    while (1) {
        //int len = lwip_recv(con_sock, recv_buf, RECV_BUF_SIZE, 0);
        int len = read(con_sock, recv_buf, RECV_BUF_SIZE);
        if (len > 0) {
            switch (parse((const uint8_t*)recv_buf, 7)) {
            case NOT:
                break;
            case GET_HTML: {
                uint16_t sizeHeadIndexHtml = SizeHeadIndexHtml;
                uint16_t sizeIndexHtml = SizeIndexHtml;
                uint8_t tempArr[sizeHeadIndexHtml + sizeIndexHtml];
                memcpy(tempArr, headIndexHtml, sizeHeadIndexHtml);
                memcpy(tempArr + sizeHeadIndexHtml, indexHtml, sizeIndexHtml);
                write(con_sock, tempArr, sizeof(tempArr));
            } break;
            case GET_ICO: {
                uint16_t sizeHeadIco = SizeHeadIco;
                uint16_t sizeIco = SizeIco;

                uint8_t tempArr[sizeHeadIco + sizeIco];
                memcpy(tempArr, headIco, sizeHeadIco);
                memcpy(tempArr + sizeHeadIco, ico, sizeIco);
                write(con_sock, tempArr, sizeof(tempArr));
            } break;
            case GET_CSS: {
                uint16_t sizeHeadCss = SizeHeadCss;
                uint16_t sizeCss = SizeCss;
                uint8_t tempArr[sizeHeadCss + sizeCss];
                memcpy(tempArr, headCss, sizeHeadCss);
                memcpy(tempArr + sizeHeadCss, css, sizeCss);
                write(con_sock, tempArr, sizeof(tempArr));
            } break;
            case GET_JS: {
                uint16_t sizeHeadJs = SizeHeadJs;
                uint16_t sizeJs = SizeJs;
                uint8_t tempArr[sizeHeadJs + sizeJs];
                memcpy(tempArr, headJs, sizeHeadJs);
                memcpy(tempArr + sizeHeadJs, js, sizeJs);
                write(con_sock, tempArr, sizeof(tempArr));
            } break;
            case GET_CONTENT: {
                // send answer
                uint16_t sizeHeadContent = SizeHeadContentStream;
                uint16_t sizeContent = 2;
                uint8_t tempArr[sizeHeadContent + sizeContent];
                memcpy(tempArr, headContentStream, sizeHeadContent);
                int debugVal = Adc_getAdc();
                memcpy(tempArr + sizeHeadContent, &debugVal, sizeContent);
                write(con_sock, tempArr, sizeof(tempArr));
            } break;
            case GET_LED_ON: {
                gpio_setBlue();
                uint16_t sizeHeadContent = SizeHeadContentStream;
                uint16_t sizeContent = 1;
                uint8_t tempArr[sizeHeadContent + sizeContent];
                memcpy(tempArr, headContentStream, sizeHeadContent);
                tempArr[sizeHeadContent - 1 - 4] = '1';
                bool state = true;
                memcpy(tempArr + sizeHeadContent, &state, sizeContent);
                write(con_sock, tempArr, sizeof(tempArr));
            } break;
            case GET_LED_OFF: {
                gpio_resetBlue();
                uint16_t sizeHeadContent = SizeHeadContentStream;
                uint16_t sizeContent = 1;
                uint8_t tempArr[sizeHeadContent + sizeContent];
                memcpy(tempArr, headContentStream, sizeHeadContent);
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
        //gpio_toggleBlue();
        vTaskDelay(1);
    }
}

enum ParseState parse(const uint8_t* data, uint16_t len)
{
    if (!len) {
        return NOT;
    }
    if (data[0] == 'G' && data[1] == 'E' && data[2] == 'T') {
        if (data[4] == '/' && data[5] == 0x20) {
            // GET /
            return GET_HTML;
        } else if (data[4] == '/' && data[5] == 'f') {
            // GET /facicon.ico
            return GET_ICO;
        } else if (data[4] == '/' && data[5] == 's' && data[6] == 't') {
            // GET /style.css
            return GET_CSS;
        } else if (data[4] == '/' && data[5] == 's' && data[6] == 'c') {
            // GET /script.js
            return GET_JS;
        } else if (data[4] == '/' && data[5] == 'c' && data[6] == 'o') {
            // GET /content.bin
            return GET_CONTENT;
        } else if (data[4] == '/' && data[5] == 'o' && data[6] == 'n') {
            // GET /on
            return GET_LED_ON;
        } else if (data[4] == '/' && data[5] == 'o' && data[6] == 'f') {
            // GET /off
            return GET_LED_OFF;
        }
    } else {
        return NOT;
    }
    return NOT;
}

void setRamSize(uint32_t size)
{
#define FLASH_KEY1 ((uint32_t)0x45670123)
#define FLASH_KEY2 ((uint32_t)0xCDEF89AB)
    //----- OPTION BYTE PROGRAMMING FOR CHANGE RAM SIZE -----------------------
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
    while (FLASH->STATR & FLASH_STATR_BSY) {}
    FLASH->OBKEYR = FLASH_KEY1;
    FLASH->OBKEYR = FLASH_KEY2;
    while (FLASH->STATR & FLASH_STATR_BSY) {}
    uint16_t pbuf[8] = {0};
    // write 0:0 RAM=128Kb CODE=192Kb
    uint8_t Data = 0;
    switch (size) {
    case 0x8000:
        // write 1:1 RAM=32Kb CODE=288Kb
        Data = 0xDF; // 1:1:0:1:1:1:1:1
        break;
    case 0x10000:
        // write 1:0 RAM=64Kb CODE=192Kb
        Data = 0x9F; // 1:0:0:1:1:1:1:1
        break;
    case 0x18000:
        // write 0:1 RAM=96Kb CODE=224Kb
        Data = 0x5F; // 0:1:0:1:1:1:1:1
        break;
    case 0x20000:
        // write 0:0 RAM=128Kb CODE=192Kb
        Data = 0x1F; // 0:0:0:1:1:1:1:1
        break;
    default:
        Data = 0x9F;
        break;
    }
    /* Read optionbytes */
    for (int i = 0; i < 8; i++) {
        pbuf[i] = *(uint16_t*)(0x1FFFF800 + 2 * i);
    }
    /* Erase optionbytes */
    FLASH->CTLR |= FLASH_CTLR_OPTER;
    FLASH->CTLR |= FLASH_CTLR_STRT;
    while (FLASH->STATR & FLASH_STATR_BSY) {}
    FLASH->CTLR &= ~FLASH_CTLR_OPTER;
    // Write optionbytes RAM
    pbuf[((0x1FFFF802 - 0x1FFFF800) / 2)] =
        ((((uint16_t) ~(Data)) << 8) | ((uint16_t)Data));

    FLASH->CTLR |= FLASH_CTLR_OPTPG;
    for (int i = 0; i < 8; i++) {
        *(uint16_t*)(0x1FFFF800 + 2 * i) = pbuf[i];
        while (FLASH->STATR & FLASH_STATR_BSY) {}
    }
    FLASH->CTLR &= ~FLASH_CTLR_OPTPG;
    // FLASH->CTLR |= FLASH_CTLR_LOCK;
}
