#ifndef MY_TCP_H
#define MY_TCP_H

#include "eth.h"
#include "firmware_update.h"
#include "lwip/tcp.h"
#include "main.h"

class Tcp {
  public:
    Tcp();
    static Tcp* pThis;

    // static struct server_struct *ss; //  global server structure ptr for
    // transmittion

    // for listening
    static tcp_pcb* tcp_server_pcb;
    // for receive - send
    static tcp_pcb* tpcbPtr;
    struct client_struct* cs; // state structure

    bool isPC_connected = false;
    bool isFrameReadyForSend = false;
    volatile uint32_t lenSendTCP = 0;
    volatile uint8_t sendPacketCounter = 0;
    volatile uint16_t packetCounter = 0;

    bool getDataForSend() { return isFrameReadyForSend; }
    void setDataForSend(bool state) { isFrameReadyForSend = state; }
    bool FirmwareUpdateFlag = false;
    volatile uint32_t totalLen = 0;
    static err_t server_send(const uint8_t* data, uint16_t len);

    //------- FUNCTION CALLED ON CLOSE CONNECTION -----------------------------
    /**
     * @brief  This functions closes the tcp connection
     * @param  tcp_pcb: pointer on the tcp connection
     * @param  es: pointer on echo_state structure
     * @retval None
     */
    static void server_connection_close(struct tcp_pcb* tpcb,
                                        struct server_struct* es);

    bool IsDataIn = false;
    tcp_pcb* current_tcp_pcb;         // state structure
    struct server_struct* current_es; // state structure
    inline bool getIsDataIn() { return IsDataIn; }
    inline void setIsDataIn(bool state, tcp_pcb* pcb,
                            struct server_struct* es) {
        IsDataIn = state;
        current_tcp_pcb = pcb;
        current_es = es;
    }
    void create_server();

    uint8_t frame[2048] = {0};

    uint16_t checkSum(const uint8_t *_data, uint16_t _length);

  private:
    void init();
    //--------------- ACCEPT NEW CONNECTION TO SERVER -------------------------
    /**
     * @brief  This function is the implementation of tcp_accept LwIP callback
     * @param  arg: not used
     * @param  newpcb: pointer on tcp_pcb struct for the newly created tcp
     * connection
     * @param  err: not used
     * @retval err_t: error status
     */
    static err_t server_accept(void* arg, struct tcp_pcb* newpcb, err_t err);

    //---------------- RECEIVE DATA FROM CLIENT ----------------------------
    /**
     * @brief  This function is the implementation for tcp_recv LwIP callback
     * @param  arg: pointer on a argument for the tcp_pcb connection
     * @param  tpcb: pointer on the tcp_pcb connection
     * @param  pbuf: pointer on the received pbuf
     * @param  err: error information regarding the reveived pbuf
     * @retval err_t: error code
     */
    static err_t server_recv(void* arg, struct tcp_pcb* tpcb, struct pbuf* p,
                             err_t err);

    // FUNCTION CALLED ON ACK SIGNAL FROM REMOTE HOST WHEN IT GETS SENDED DATA
    /**
     * @brief  This function implements the tcp_sent LwIP callback (called when
     * ACK is received from remote host for sent data)
     * @param  None
     * @retval None
     */
    static err_t server_sent(void* arg, struct tcp_pcb* tpcb, u16_t len);

    //----------------- POLL FUNCTION ---------------------------------------
    /**
     * @brief  This function implements the tcp_poll LwIP callback function
     * @param  arg: pointer on argument passed to callback
     * @param  tpcb: pointer on the tcp_pcb for the current tcp connection
     * @retval err_t: error code
     */
    static err_t server_poll(void* arg, struct tcp_pcb* tpcb);

    static void server_error(void* arg, err_t er);
    ip_addr_t PC_IP = IPADDR4_INIT_BYTES(192, 168, 1, 200);
    // constant addresses
    static constexpr uint8_t ip[4] = {192, 168, 1, 5};
    static constexpr uint16_t TCP_PORT = 55555;
};

#endif // MY_TCP_H
