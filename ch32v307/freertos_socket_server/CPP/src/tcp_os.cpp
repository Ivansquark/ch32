#include "tcp_os.h"

TcpOS::TcpOS(const char* recv_buf) : FR_OS(4096) {
    FR_OS::init_FR_OS();
    this->recv_buf = (char*)recv_buf;
}

void TcpOS::runTask([[maybe_unused]] void* pvParameters) {
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
            // http obj
            HttpOS http(recv_buf, 4096, 0, &con_sock);
            // xTaskCreate((TaskFunction_t)http_task, (const char*)"http_task",
            //            (uint16_t)4096, (void*)&con_sock,
            //            (UBaseType_t)tskIDLE_PRIORITY, NULL);
            //(TaskHandle_t*)&TaskHttp_Handler);
        } else {
            // con_sock == 0
            // close(con_sock);
        }
        vTaskDelay(1); // every ms
    }
}
