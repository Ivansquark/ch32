#ifndef MY_UDP_H
#define MY_UDP_H

#include "eth.h"
#include "lwip/udp.h"
#include "main.h"
#include "string.h"

class Udp {
  public:
    Udp();
    static Udp* pThis;
    //send check speed flag
    bool SendFlag = false;


    void sendToPC(const uint8_t* buff, uint16_t len);
    static void receive_Local_callback(void* arg, struct udp_pcb* upcb,
                                       struct pbuf* p, const ip_addr_t* addr,
                                       u16_t port);
    static void receive_BROADCAST_callback(void* arg, struct udp_pcb* upcb,
                                           struct pbuf* p,
                                           const ip_addr_t* addr, u16_t port);

    uint8_t udp_msg_send[300*12];
    uint8_t udp_msg_receive[255];

    static udp_pcb* udpLocalPcb;
    static udp_pcb* udpBroadcastPcb;
    //------------ protocol ------------------
#pragma pack(push, 1)
    // reverse, cause little endian
    struct Len12bit_Num4bit {
        uint16_t dataNum : 4;
        uint16_t dataLen : 12;
    };

    struct Header {
        uint16_t PackFlag = 0;
        Len12bit_Num4bit pixel{0, 0};
        Len12bit_Num4bit line{0, 0};
    };
#pragma pack(pop)


  private:
    void init();

    static void connect(void);
    static void disconnect();

    //--------------------------    Adresses
    //---------------------------------------------------
    //  temporary IP adresses, thats can changed on PC request
    ip_addr_t local_IP = IPADDR4_INIT_BYTES(ip[0], ip[1], ip[2], ip[3]);
    // static uint8_t local_ip[4];
    ip_addr_t PC_IP = IPADDR4_INIT_BYTES(192, 168, 1, 200);

    //  constant adresses
    static constexpr uint8_t ip[4] = {192, 168, 1, 100};
    static constexpr uint16_t BROADCAST_PORT = 0xFFFF;

    static constexpr uint16_t PORT_OUT = 4001;
    static constexpr uint16_t PORT_IN = 3000;
};

#endif // MY_UDP_H
