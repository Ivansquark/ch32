#ifndef MY_UDP_H
#define MY_UDP_H

#include "eth.h"
#include "lwip/udp.h"
#include "main.h"
#include "protocol.h"
#include "string.h"

class Udp {
  public:
    explicit Udp(uint8_t ip0 = 192, uint8_t ip1 = 168, uint8_t ip2 = 1,
                 uint8_t ip3 = 5);
    static Udp* pThis;
    // send check speed flag
    bool SendFlag = false;

    void sendToPC(const uint8_t* buff, uint8_t len);
    static void receive_Local_callback(void*, struct udp_pcb*, struct pbuf*,
                                       const ip_addr_t*, u16_t);
    static uint8_t udp_msg_send[255];
    static uint8_t udp_msg_receive[255];

    static udp_pcb* udpLocalPcb;

    static uint8_t ip[4];
    void setNewIP(uint8_t ip0, uint8_t ip1, uint8_t ip2, uint8_t ip3);

    bool WaitForReply = false;
    bool MustSendReply = false;
    bool IsDataReceived = false;
    bool IsDataSetReceived = false;
    bool MustSend = false;
    bool MustResend = false;

    void sendReply();

  private:
    static void init();

    static void my_connect(void);
    static void disconnect();

    //---------------------    Adresses   -------------------------------------
    //  temporary IP adresses, thats can changed on PC request
    ip_addr_t local_IP = IPADDR4_INIT_BYTES(ip[0], ip[1], ip[2], ip[3]);
    // static uint8_t local_ip[4];
    ip_addr_t PC_IP = IPADDR4_INIT_BYTES(192, 168, 1, 200);

    //  constant adresses
    // static constexpr uint8_t ip[4] = {192, 168, 1, 100};
    static constexpr uint16_t PORT_IN = 4000;
    static constexpr uint16_t PC_PORT_IN = 4001;
    static constexpr uint16_t BROADCAST_PORT = 0xFFFF;
};

#endif // MY_UDP_H
