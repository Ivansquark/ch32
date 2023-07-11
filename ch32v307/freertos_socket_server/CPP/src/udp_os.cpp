#include "udp_os.h"

UdpOS::UdpOS(uint8_t* recv_buf) : FR_OS() {
    init_FR_OS(); // to set xTaskCreate
    this->recv_buf = recv_buf;
}

void UdpOS::runTask([[maybe_unused]] void* pvParameters) {
    //----------- echo server---------------------------------------------
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
        vTaskDelay(10); // every ms
    }
}
