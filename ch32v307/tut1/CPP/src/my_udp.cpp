#include "my_udp.h"

Udp* Udp::pThis = nullptr;
udp_pcb* Udp::udpLocalPcb = nullptr;
udp_pcb* Udp::udpBroadcastPcb = nullptr;

Udp::Udp() {
    pThis = this;
    init();
}

void Udp::init() { connect(); }

void Udp::connect(void) {
    udpBroadcastPcb = udp_new();
    // ip_set_option(upcb, SOF_BROADCAST); IP_ADDR_BROADCAST
    if (udpBroadcastPcb != NULL) {
        udp_bind(udpBroadcastPcb, IP_ADDR_BROADCAST, BROADCAST_PORT);
        udp_recv(udpBroadcastPcb, receive_BROADCAST_callback, NULL);
    }
    udpLocalPcb = udp_new();
    if (udpLocalPcb != NULL) {
        ip_addr_t srcaddr;
        IP4_ADDR(&srcaddr, ip[0], ip[1], ip[2], ip[3]);
        udp_bind(udpLocalPcb, &srcaddr, PORT_IN);
        udp_recv(udpLocalPcb, receive_Local_callback, NULL);
    }
}

void Udp::disconnect() {
    if (udpLocalPcb) { udp_remove(udpLocalPcb); }
    if (udpBroadcastPcb) { udp_remove(udpBroadcastPcb); }
}

void Udp::receive_BROADCAST_callback([[maybe_unused]] void* arg,
                                     [[maybe_unused]] struct udp_pcb* upcb,
                                     struct pbuf* p, const ip_addr_t* addr,
                                     [[maybe_unused]] u16_t port) {
    uint16_t pLen = p->len;
    memcpy(Udp::pThis->udp_msg_receive, p->payload, pLen);
    // Net::parseUdpBROADCAST(pLen);
    p->payload = Udp::pThis->udp_msg_receive;
    Udp::pThis->udp_msg_receive[0] = '0';
    p->len = pLen;
    udp_sendto(udpLocalPcb, p, addr, PORT_OUT);
    pbuf_free(p);
}

void Udp::receive_Local_callback([[maybe_unused]] void* arg,
                                 [[maybe_unused]] struct udp_pcb* upcb,
                                 struct pbuf* p,
                                 [[maybe_unused]] const ip_addr_t* addr,
                                 [[maybe_unused]] u16_t port) {
    uint16_t pLen = p->len;
    // ip_addr_t received_ip = *addr;
    memcpy(Udp::pThis->udp_msg_receive, p->payload, pLen);

    if (Udp::pThis->udp_msg_receive[6] == '1') {

        Udp::pThis->SendFlag = true;
    } else if (Udp::pThis->udp_msg_receive[6] == '0') {

        Udp::pThis->SendFlag = false;
    }
    pbuf_free(p);
}

void Udp::sendToPC(const uint8_t* buff, uint16_t len) {
    // wait while transmit previous packet
    uint32_t timeout = 0xFFFFF;
    // while (timeout-- && (ETH->DMASR & ETH_DMASR_TPS) != 0 &&
    //                     (ETH->DMASR & ETH_DMASR_TPS) != 0x6) {}
    while ((ETH->DMASR & ETH_DMASR_TPS) != 0 &&
           (ETH->DMASR & ETH_DMASR_TPS) != 0x600000) {}
    // while (timeout-- && ((ETH->DMASR & ETH_DMASR_TS) == 0 )) {}
    struct pbuf* p;
    p = pbuf_alloc(PBUF_RAW, 1600, PBUF_REF);
    p->payload = (uint8_t*)buff;
    p->len = p->tot_len = len;
    // send only from local pcb
    udp_sendto(udpLocalPcb, p, &PC_IP, PORT_OUT);
    pbuf_free(p);
}
