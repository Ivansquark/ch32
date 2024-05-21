#ifndef MY_TCP_H
#define MY_TCP_H

#include "eth.h"
// #include "firmware_update.h"
#include "lwip/tcp.h"
#include "main.h"

class Tcp {
  public:
    Tcp(uint8_t* eth_data_buf_client, uint8_t* eth_data_buf_server,
        uint8_t ip1 = ip[0], uint8_t ip2 = ip[1], uint8_t ip3 = ip[2],
        uint8_t ip4 = ip[3]);
    static Tcp* pThis;

    bool isDataForSend();

    // for listening
    static tcp_pcb* tcp_server_pcb;
    // for receive - send
    static tcp_pcb* tpcbPtr;

    inline bool getIsPC_connected() { return IsPC_connected; }
    inline void setIsPC_connected(bool state) { IsPC_connected = state; }
    inline bool getServerIsConnected() { return IsServer_connected; }
    inline void setServerIsConnected(bool state) { IsServer_connected = state; }

    inline uint32_t getServerLenReceived() { return server_lenReceived; }
    inline void setServerLenReceived(uint32_t len) { server_lenReceived = len; }
    inline uint32_t getClientLenReceived() { return client_lenReceived; }
    inline void setClientLenReceived(uint32_t len) { client_lenReceived = len; }
    inline bool getClientIsReceived() { return client_IsReceived; }
    inline void setClientIsReceived(bool state) { client_IsReceived = state; }
    inline bool getServerIsReceived() { return server_IsReceived; }
    inline void setServerIsReceived(bool state) { server_IsReceived = state; }
    bool FirmwareUpdateFlag = false;
    // uint32_t totalLen = 0;
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

    // bool IsDataIn = false;
    // tcp_pcb* current_tcp_pcb;         // state structure
    // struct server_struct* current_es; // state structure
    // inline bool getIsDataIn() { return IsDataIn; }
    // inline void setIsDataIn(bool state, tcp_pcb* pcb,
    //                         struct server_struct* es) {
    //     IsDataIn = state;
    //     current_tcp_pcb = pcb;
    //     current_es = es;
    // }
    //
    err_t create_server();

    enum client_states {
        ES_NOT_CONNECTED = 0,
        ES_CONNECTED,
        ES_RECEIVED,
        ES_CLOSING,
    };
    //-------------------------------------------------------
    struct client_struct {
        enum client_states state; /* connection status */
        struct tcp_pcb* pcb;      /* pointer on the current tcp_pcb */
        struct pbuf* p_tx;        /* pointer on pbuf to be transmitted */
    };
    static client_struct* es;
    static client_struct* cs;

    bool client_connect();
    bool client_disconnect();
    void client_writeToServer(const uint8_t* data, uint16_t len);

    static void client_connection_close(tcp_pcb* tpcb, client_struct* es);

  private:
    void init();

    uint8_t* client_receivedBuf = nullptr;
    uint8_t* server_receivedBuf = nullptr;
    bool IsPC_connected = false;
    bool IsServer_connected = false;
    bool client_IsReceived = false;
    bool server_IsReceived = false;
    uint32_t server_lenReceived = 0;
    uint32_t client_lenReceived = 0;
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

    //----------------- POLL FUNCTION -----------------------------------------
    /**
     * @brief  This function implements the tcp_poll LwIP callback function
     * @param  arg: pointer on argument passed to callback
     * @param  tpcb: pointer on the tcp_pcb for the current tcp connection
     * @retval err_t: error code
     */
    static err_t server_poll(void* arg, struct tcp_pcb* tpcb);

    static void server_error(void* arg, err_t er);
    // ip_addr_t PC_IP = IPADDR4_INIT_BYTES(192, 168, 1, 200);
    //
    //---------------- CLIENT -------------------------------------------------
    static uint8_t dataTx[100];

    struct tcp_pcb* client_pcb = nullptr;
    static err_t client_connected(void* arg, struct tcp_pcb* tpcb, err_t err);
    static err_t client_recv(void* arg, struct tcp_pcb* tpcb, struct pbuf* p,
                             err_t err);
    static void client_send(struct tcp_pcb* tpcb, struct client_struct* es);
    static err_t client_sent(void* arg, struct tcp_pcb* tpcb, u16_t len);
    static err_t client_poll(void* arg, struct tcp_pcb* tpcb);

    //  constant addresses
    static constexpr uint8_t ip[4] = {192, 168, 1, 5};
    static constexpr uint8_t ip_PC[4] = {192, 168, 1, 200};
    static constexpr uint16_t TCP_PORT = 55555;
    //--------------------------    Adresses    -------------------------------
    //  temporary IP adresses, thats can changed on PC request
    ip_addr_t local_IP = IPADDR4_INIT_BYTES(ip[0], ip[1], ip[2], ip[3]);
};

#endif // MY_TCP_H
