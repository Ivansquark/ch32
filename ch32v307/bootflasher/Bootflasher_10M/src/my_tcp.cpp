#include "my_tcp.h"

Tcp* Tcp::pThis = nullptr;
tcp_pcb* Tcp::tcp_server_pcb = nullptr;
tcp_pcb* Tcp::tpcbPtr = nullptr;

Tcp::Tcp() {
    pThis = this;
    init();
}
void Tcp::init() {}
void Tcp::create_server() {
    ip_addr_t DestIPaddr;
    // IP4_ADDR(&DestIPaddr, ip[0], ip[1], ip[2], ip[3]);
    IP4_ADDR(&DestIPaddr, Eth::pThis->IP_ADDRESS[0], Eth::pThis->IP_ADDRESS[1],
             Eth::pThis->IP_ADDRESS[2], Eth::pThis->IP_ADDRESS[3]);
    //* create protocol control block for new connection
    tcp_server_pcb = tcp_new();
    err_t err;
    if (tcp_server_pcb != NULL) {
        // bind tcp pcb to IP and PORT
        err = tcp_bind(tcp_server_pcb, &DestIPaddr, TCP_PORT);
        if (err == ERR_OK) {
            /* start tcp listening for echo_pcb */
            tcp_server_pcb = tcp_listen(tcp_server_pcb);
            /* initialize LwIP tcp_accept callback function for new connection*/
            tcp_accept(tcp_server_pcb, server_accept);
        } else {
            /* deallocate the pcb */
            memp_free(MEMP_TCP_PCB, tcp_server_pcb);
        }
    }
}

err_t Tcp::server_accept(void* arg, struct tcp_pcb* newpcb, err_t err) {
    err_t ret_err = ERR_OK;
    LWIP_UNUSED_ARG(arg);
    LWIP_UNUSED_ARG(err);
    /* set priority for the newly accepted tcp connection newpcb */
    tcp_setprio(newpcb, TCP_PRIO_MIN);
    struct server_struct* es = NULL;

    // this goes to memory fragmentation

    // pass newly allocated es structure as argument to newpcb (argument to
    // all callback functions)*/
    tcp_arg(newpcb, NULL);
    //* lwIP tcp_recv callback function for newpcb (receive packet)*/
    tcp_recv(newpcb, server_recv);
    //* initialize lwIP tcp_err callback function for newpcb */
    tcp_err(newpcb, server_error);
    //* initialize lwIP tcp_poll callback function for newpcb */
    tcp_poll(newpcb, server_poll, 10); // every 5 secs
    //* set callback on ack event from remote host (when data was sended)
    tcp_sent(newpcb, server_sent);
    //* set callback on connect to server function
    ret_err = ERR_OK;

    return ret_err;
}

//---------------- RECEIVE DATA FROM CLIENT (callback)-------------------------
err_t Tcp::server_recv(void* arg, struct tcp_pcb* tpcb, struct pbuf* p,
                       err_t err) {
    //! this function executes in lwip_input (We must set state and checked in
    // another)
    //
    // - Mutex that disable receive (whith ACK answer) while transmit proceed -
    // if (pThis->isFrameReadyForSend) {
    //    return ERR_USE;
    //}
    //----  Receive function ------------------------------------------------
    struct server_struct* es;
    err_t ret_err;
    LWIP_ASSERT("arg != NULL", arg != NULL);
    /* if we receive an empty tcp frame from client => close connection */
    if (p == NULL) {
        /* remote host closed connection */
        server_connection_close(tpcb, NULL);
        ret_err = ERR_OK;
    } else if (err != ERR_OK) {
        /* a non empty frame received but for some reason err != ERR_OK */
        pbuf_free(p); /* free received pbuf (nothing do with data)*/
        server_connection_close(tpcb, es);
        ret_err = err;
    } else {
        pThis->lenSendTCP = p->len;
        // copy received data in global buffer
        memcpy((char*)pThis->frame, (const char*)p->payload, p->len);
        //! - CHECK FOR FULL FRAME IN PACKET (ALL FRAMES EXCEPT cmdSet) --------
        // if (isFullFrameInPacket(p->len)) {
        Tcp::pThis->tpcbPtr = tpcb;
        FirmwareUpdate::parsePayload((uint8_t*)pThis->frame, p->len);
        pThis->totalLen = 0;
        //} else {
        //    pThis->totalLen = p->len;
        //}
        tcp_recved(tpcb, p->tot_len);
        pbuf_free(p);
        ret_err = ERR_OK;
    }
    return ret_err;
}

err_t Tcp::server_send(const uint8_t* data, uint16_t len) {
    tcp_write(tpcbPtr, (const void*)(data), len, 1);
    tcp_output(tpcbPtr); // send data now
    return ERR_OK;
    // tcp_recved(tpcbPtr, TCP_MSS);
}
// FUNCTION CALLED ON ACK SIGNAL FROM REMOTE HOST WHEN IT GETS SENDED DATA
err_t Tcp::server_sent(void* arg, struct tcp_pcb* tpcb, u16_t len) {
    struct server_struct* es;
    LWIP_UNUSED_ARG(len);
    es = (struct server_struct*)arg;
    /* still got pbufs to send */
    return ERR_OK;
}

void Tcp::server_connection_close(struct tcp_pcb* tpcb,
                                  struct server_struct* es) {
    // remove all callbacks
    tcp_arg(tpcb, NULL);
    tcp_sent(tpcb, NULL);
    tcp_recv(tpcb, NULL);
    tcp_err(tpcb, NULL);
    tcp_poll(tpcb, NULL, 0);
    tcp_close(tpcb);
}

//----------------- POLL FUNCTION ---------------------------------------
err_t Tcp::server_poll(void* arg, struct tcp_pcb* tpcb) {
    err_t ret_err;
    struct server_struct* es;
    es = (struct server_struct*)arg;
    ret_err = ERR_OK;
    return ret_err;
}

void Tcp::server_error([[maybe_unused]] void* arg, [[maybe_unused]] err_t er) {
    // dummy function
}

uint16_t Tcp::checkSum(const uint8_t * _data, uint16_t _length) {
    uint16_t count = _length;
    uint16_t res = 0xFFFF; // Load a 16–bit register with FFFF hex (all 1’s).
    while (count--) {
        uint8_t data1 = *_data;
        _data++;
        res ^= data1;
        for (int j = 0; j < 8; j++) {
            // Shift the CRC register one bit to the right (toward the LSB),
            //(If the LSB was 1): Exclusive OR the CRC register with the
            // polynomial value 0xA001 (1010 0000 0000 0001).
            if (res & 0x0001) {
                res = (res >> 1) ^ 0xA001;
            } // LSB(b0)=1
            else
                res = res >> 1; // (If the LSB was 0): Repeat Step 3.
        }
    }
    return res;
}
