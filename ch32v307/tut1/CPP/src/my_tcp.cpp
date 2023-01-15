#include "my_tcp.h"

Tcp* Tcp::pThis = nullptr;
tcp_pcb* Tcp::tcp_server_pcb = nullptr;
tcp_pcb* Tcp::tpcbPtr = nullptr;

Tcp::Tcp() {
    pThis = this;
    init();
}
void Tcp::init() { create_server(); }
void Tcp::create_server() {
    ip_addr_t DestIPaddr;
    IP4_ADDR(&DestIPaddr, ip[0], ip[1], ip[2], ip[3]);
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
    err_t ret_err;
    LWIP_UNUSED_ARG(arg);
    LWIP_UNUSED_ARG(err);
    /* set priority for the newly accepted tcp connection newpcb */
    tcp_setprio(newpcb, TCP_PRIO_MIN);
    struct server_struct* es;
    /* allocate structure es to maintain tcp connection informations (different
     * for each connection) */
    es = (struct server_struct*)mem_malloc(sizeof(struct server_struct));
    if (es != NULL) {
        es->state = ES_ACCEPTED;
        es->pcb = newpcb;
        es->retries = 0;
        es->p = NULL;
        // pass newly allocated es structure as argument to newpcb (argument to
        // all callback functions)*/
        tcp_arg(newpcb, es);
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
        err = ERR_OK;
        pThis->isPC_connected = true;
    } else {
        server_connection_close(newpcb, es);
        /* return memory error */
        ret_err = ERR_MEM;
        err = ERR_OK;
    }
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
    es = (struct server_struct*)arg;
    pThis->tpcbPtr = tpcb; // copy ptr of control block
    /* if we receive an empty tcp frame from client => close connection */
    if (p == NULL) {
        /* remote host closed connection */
        es->state = ES_CLOSE;
        if (es->p == NULL) {
            /* we're done sending, close connection */
            server_connection_close(tpcb, es);
        }
        ret_err = ERR_OK;
    } else if (err != ERR_OK) {
        /* a non empty frame received but for some reason err != ERR_OK */
        if (p != NULL) {
            pbuf_free(p); /* free received pbuf (nothing do with data)*/
            es->p = NULL;
        }
        ret_err = err;
    } else if (es->state == ES_ACCEPTED) {
        //------ FIRST RECEIVED DATA /* first data chunk in p->payload */ -----
        es->p = p; ///* store reference to incoming pbuf (chain) */
        pThis->lenSendTCP = es->p->len;
        //* initialize LwIP tcp_sent callback function */
        tcp_sent(es->pcb, server_sent);
        // copy received data in global buffer
        es->state = ES_RECEIVE;
        // TODO: parse frame (full not full) goto ES_RECEIVE
        Eth::currentRxBuffLen = p->len;
        memcpy(Eth::RxBuff, (const uint8_t*)p->payload, p->len);
        pThis->isFrameReadyForSend = true;
        pThis->setIsDataIn(true, tpcb, es);
        pbuf_free(p);
        // pbuf_free(es->p);
        // es->p = NULL;
        tcp_recved(tpcb, TCP_MSS); // send ACK (with new WND size)
        ret_err = ERR_OK;
    } else if (es->state == ES_RECEIVE) {
        ///* more data recved from client and previous data has already sent*/
        //long GET request or some other requests
        ret_err = ERR_OK;
    } else if (es->state == ES_CLOSE) {
        /* data received when connection already closed */
        /* Acknowledge data reception */
        tcp_recved(tpcb, p->tot_len);
        /* free pbuf and do nothing */
        pbuf_free(p);
        ret_err = ERR_OK;
    } else {
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
    es->retries = 0;
    /* still got pbufs to send */
    if (es->p != NULL) {
        pbuf_free(es->p);
        // es->p = NULL;
    } else {
        /* if no more data to send and client closed connection*/
        if (es->state == ES_CLOSE) { server_connection_close(tpcb, es); }
    }
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
    if (es != NULL) { mem_free(es); }
    tcp_close(tpcb);
    pThis->isPC_connected = false;
}

//----------------- POLL FUNCTION ---------------------------------------
err_t Tcp::server_poll(void* arg, struct tcp_pcb* tpcb) {
    err_t ret_err;
    struct server_struct* es;
    es = (struct server_struct*)arg;
    if (es != NULL) {
        if (es->p != NULL) {
        } else {
            if (es->state == ES_CLOSE) { server_connection_close(tpcb, es); }
        }
        ret_err = ERR_OK;
    } else {
        tcp_abort(tpcb);
        ret_err = ERR_ABRT;
    }
    return ret_err;
}

void Tcp::server_error([[maybe_unused]] void* arg, [[maybe_unused]] err_t er) {
    // dummy function
}
