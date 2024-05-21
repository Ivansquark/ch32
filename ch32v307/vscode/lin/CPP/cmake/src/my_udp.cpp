#include "my_udp.h"

Udp* Udp::pThis = nullptr;
udp_pcb* Udp::udpLocalPcb = nullptr;
uint8_t Udp::udp_msg_send[255] = {0};
uint8_t Udp::udp_msg_receive[255] = {0};
uint8_t Udp::ip[4] = {0};

Udp::Udp([[maybe_unused]] uint8_t ip0, [[maybe_unused]] uint8_t ip1,
         [[maybe_unused]] uint8_t ip2, [[maybe_unused]] uint8_t ip3) {
    ip[0] = Eth::pThis->IP_ADDRESS[0];
    ip[1] = Eth::pThis->IP_ADDRESS[1];
    ip[2] = Eth::pThis->IP_ADDRESS[2];
    ip[3] = Eth::pThis->IP_ADDRESS[3];
    pThis = this;
    init();
}

void Udp::init() { my_connect(); }

void Udp::my_connect(void) {
    // ip_set_option(upcb, SOF_BROADCAST); IP_ADDR_BROADCAST
    udpLocalPcb = udp_new();
    if (udpLocalPcb != NULL) {
        ip_addr_t srcaddr;
        IP4_ADDR(&srcaddr, ip[0], ip[1], ip[2], ip[3]);
        udp_bind(udpLocalPcb, &srcaddr, PORT_IN);
        udp_recv(udpLocalPcb, receive_Local_callback, NULL);
    }
}

void Udp::disconnect() {
    if (udpLocalPcb) {
        udp_remove(udpLocalPcb);
    }
}

void Udp::receive_Local_callback(void*, [[maybe_unused]] struct udp_pcb* upcb,
                                 struct pbuf* p,
                                 [[maybe_unused]] const ip_addr_t* addr,
                                 [[maybe_unused]] u16_t port) {
    uint16_t pLen = p->len;
    // ip_addr_t received_ip = *addr;
    memcpy(udp_msg_receive, p->payload, pLen);
    // Set current interface to UDP

    pbuf_free(p);
}

void Udp::sendToPC(const uint8_t* buff, uint8_t len) {
    struct pbuf* p;
    p = pbuf_alloc(PBUF_RAW, 256, PBUF_REF);
    p->payload = const_cast<uint8_t*>(buff);
    p->len = p->tot_len = len;
    // send only from local pcb
    udp_sendto(udpLocalPcb, p, &PC_IP, PC_PORT_IN);
    pbuf_free(p);
}
void Udp::sendReply() {
    uint8_t arr[4] = {};
    sendToPC(arr, sizeof(arr));
}

