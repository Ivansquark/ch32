#ifndef MY_UDP_H
#define MY_UDP_H

#include "eth.h"
#include "lwip/udp.h"
#include "main.h"
#include "string.h"
#include "protocol.h"

class Udp {
  public:
    Udp(uint8_t ip0 = 192, uint8_t ip1 = 168, uint8_t ip2 = 1,
        uint8_t ip3 = 5);
    static Udp* pThis;
    // send check speed flag
    bool SendFlag = false;

    void send_BROADCAST(const uint8_t *buff, uint8_t len);
    void sendToControl(const uint8_t *buff, uint8_t len);
    void sendToPC(const uint8_t *buff, uint8_t len);
    static void receive_Local_callback(void* arg, struct udp_pcb* upcb,
                                       struct pbuf* p, const ip_addr_t* addr,
                                       u16_t port);
    static void receive_BROADCAST_callback(void* arg, struct udp_pcb* upcb,
                                           struct pbuf* p,
                                           const ip_addr_t* addr, u16_t port);

    static uint8_t udp_msg_send[255];
    static uint8_t udp_msg_receive[255];

    static udp_pcb* udpLocalPcb;
    static udp_pcb* udpBroadcastPcb;

    static uint8_t ip[4];
    void setNewIP(uint8_t ip0, uint8_t ip1, uint8_t ip2, uint8_t ip3);
  private:
    void init();

    static void my_connect(void);
    static void disconnect();

    //--------------------------    Adresses
    //---------------------------------------------------
    //  temporary IP adresses, thats can changed on PC request
    ip_addr_t local_IP = IPADDR4_INIT_BYTES(ip[0], ip[1], ip[2], ip[3]);
    // static uint8_t local_ip[4];
    ip_addr_t PC_IP = IPADDR4_INIT_BYTES(192, 168, 1, 200);
    ip_addr_t control_IP = IPADDR4_INIT_BYTES(192, 168, 1, 222);

    //  constant adresses
    // static constexpr uint8_t ip[4] = {192, 168, 1, 100};
    static constexpr uint16_t EMITTER_PORT_IN = 40000;
    static constexpr uint16_t EMITTER_PORT_OUT = 40001;
    static constexpr uint16_t PC_PORT_IN = 60000;
    static constexpr uint16_t PC_PORT_OUT = 60001;
    static constexpr uint16_t BROADCAST_PORT = 0xFFFF;
};

#endif // MY_UDP_H
