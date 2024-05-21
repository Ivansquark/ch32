#include "my_tcp.h"

Tcp* Tcp::pThis = nullptr;
tcp_pcb* Tcp::tcp_server_pcb = nullptr;
tcp_pcb* Tcp::tpcbPtr = nullptr;

Tcp::Tcp([[maybe_unused]] uint8_t* eth_data_buf_client, [[maybe_unused]] uint8_t* eth_data_buf_server,uint8_t ip1, uint8_t ip2,
         uint8_t ip3, uint8_t ip4) {
    pThis = this;
    local_IP = IPADDR4_INIT_BYTES(ip1, ip2, ip3, ip4);
    client_receivedBuf = eth_data_buf_client;
    server_receivedBuf = eth_data_buf_server;
}
err_t Tcp::create_server() {
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
    } else {
        err = ERR_MEM;
    }
    return err;
}

err_t Tcp::server_accept(void* arg, struct tcp_pcb* newpcb, err_t err) {
    // only one connection (from PC)
    err_t ret_err = ERR_OK;
    LWIP_UNUSED_ARG(arg);
    LWIP_UNUSED_ARG(err);
    /* set priority for the newly accepted tcp connection newpcb */
    if (!newpcb) {
        ret_err = ERR_MEM;
        return err;
        // Tcp::pThis->create_server();
    }
    tcp_setprio(newpcb, TCP_PRIO_MIN);
    // later may set this parameter as arg to callback
    // struct server_struct* es = NULL;

    // this goes to memory fragmentation
    //
    // TODO set Eth::MAC_PC
    // Eth::pThis->startMacFilteringSelf();
    // memcpy(Eth::pThis->MAC_PC, Eth::pThis->temp_MAC_PC, 6);
    // Eth::pThis->startMacFiltering();
    // Eth::pThis->MAC_PC[0] = newpcb.
    tcp_nagle_disable(newpcb);
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
    pThis->tpcbPtr = newpcb;
    pThis->setIsPC_connected(true);

    return ret_err;
}

//---------------- RECEIVE DATA FROM CLIENT (callback)-------------------------
err_t Tcp::server_recv([[maybe_unused]] void* arg, struct tcp_pcb* tpcb,
                       struct pbuf* p, err_t err) {
    //! this function executes in lwip_input (We must set state and checked in
    // another)
    // - Mutex that disable receive (without ACK answer) -
    if (pThis->server_IsReceived) {
        return ERR_USE;
    }
    //----  Receive function ------------------------------------------------
    struct server_struct* es = NULL;
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
        // copy received data in global buffer
        memcpy((char*)pThis->server_receivedBuf, (const char*)p->payload, p->len);
        pThis->server_lenReceived = p->len;
        // TODO: Set flag to Parse data
        pThis->server_IsReceived = true;

        pbuf_free(p);
        tcp_recved(tpcb, TCP_MSS); // send ACK (with new WND size)
        ret_err = ERR_OK;
    }
    return ret_err;
}

err_t Tcp::server_send(const uint8_t* data, uint16_t len) {
    tcp_write(tpcbPtr, (const void*)(data), len, 1);
    tcp_output(tpcbPtr); // send data now
    return ERR_OK;
}
// FUNCTION CALLED ON ACK SIGNAL FROM REMOTE HOST WHEN IT GETS SENDED DATA
err_t Tcp::server_sent([[maybe_unused]] void* arg,
                       [[maybe_unused]] struct tcp_pcb* tpcb,
                       [[maybe_unused]] u16_t len) {
    return ERR_OK;
}

void Tcp::server_connection_close(struct tcp_pcb* tpcb,
                                  [[maybe_unused]] struct server_struct* es) {
    // remove all callbacks
    tcp_arg(tpcb, NULL);
    tcp_sent(tpcb, NULL);
    tcp_recv(tpcb, NULL);
    tcp_err(tpcb, NULL);
    tcp_poll(tpcb, NULL, 0);
    // close connection with approvement
    tcp_close(tpcb);
    // close connection without approvement
    // tcp_abort(tpcb); //not worked (broke all stack)
    pThis->setIsPC_connected(false);
}

//----------------- POLL FUNCTION ---------------------------------------
err_t Tcp::server_poll([[maybe_unused]] void* arg,
                       [[maybe_unused]] struct tcp_pcb* tpcb) {
    err_t ret_err;
    // struct server_struct* es;
    // es = (struct server_struct*)arg;
    ret_err = ERR_OK;
    return ret_err;
}

void Tcp::server_error([[maybe_unused]] void* arg, [[maybe_unused]] err_t er) {
    // dummy function
}

///////////////////////////////////////////////////////////////////////////////
//-------------- Client -------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
Tcp::client_struct* Tcp::es = nullptr;
Tcp::client_struct* Tcp::cs = nullptr;
uint8_t Tcp::dataTx[100] = {0};

bool Tcp::client_connect() {
    ip_addr_t DestIPaddr;
    client_pcb = tcp_new();
    if (client_pcb != NULL) {
        IP4_ADDR(&DestIPaddr, ip_PC[0], ip_PC[1], ip_PC[2], ip_PC[3]);
        err_t error = tcp_connect(client_pcb, &DestIPaddr, TCP_PORT,
                                  client_connected);
        for (volatile int i = 0; i < 10000000; i++) {}
        if (error != ERR_OK) {
            return false;
        }
        return true;
    } else {
        return false;
    }
}

bool Tcp::client_disconnect() {
    if (client_pcb) {
        tcp_recv(client_pcb, NULL);
        tcp_sent(client_pcb, NULL);
        tcp_poll(client_pcb, NULL, 0);
        tcp_close(client_pcb);
        return true;
    } else {
        return true;
    }
}

err_t Tcp::client_connected([[maybe_unused]] void* arg,
                                struct tcp_pcb* tpcb, err_t err) {
    client_struct* es = NULL;
    if (err == ERR_OK) {
        es = (struct client_struct*)mem_malloc(sizeof(client_struct));
        if (es != NULL) {
            es->state = ES_CONNECTED;
            es->pcb = tpcb;
            es->p_tx =
                pbuf_alloc(PBUF_TRANSPORT, strlen((char*)dataTx), PBUF_POOL);
            if (es->p_tx) {
                /* copy data to pbuf */
                pbuf_take(es->p_tx, (char*)dataTx, strlen((char*)dataTx));
                /* pass newly allocated es structure as argument to tpcb */
                tcp_arg(tpcb, es);
                /* initialize LwIP tcp_recv callback function */
                tcp_recv(tpcb, client_recv);
                /* initialize LwIP tcp_sent callback function */
                tcp_sent(tpcb, client_sent);
                /* initialize LwIP tcp_poll callback function */
                tcp_poll(tpcb, client_poll, 1);
                /* send data */
                client_send(tpcb, es);
                Tcp::pThis->IsServer_connected = true;
                return ERR_OK;
            }
        } else {
            /* close connection */
            client_connection_close(tpcb, es);
            /* return memory allocation error */
            return ERR_MEM;
        }
    } else {
        /* close connection */
        client_connection_close(tpcb, es);
    }
    return err;
}

void Tcp::client_connection_close(tcp_pcb* tpcb, client_struct* es) {
    /* remove callbacks */
    tcp_recv(tpcb, NULL);
    tcp_sent(tpcb, NULL);
    tcp_poll(tpcb, NULL, 0);
    if (es != NULL) {
        mem_free(es);
    }
    /* close tcp connection */
    tcp_close(tpcb);
    Tcp::pThis->IsServer_connected = false;
}

void Tcp::client_writeToServer(const uint8_t* data, uint16_t len) {
    tcp_write(client_pcb, data, len, 1);
}

//----------------------------------------------------------
err_t Tcp::client_recv(void* arg, tcp_pcb* tpcb, pbuf* p, err_t err) {
    if (pThis->client_IsReceived) {
        return ERR_USE;
    }
    client_struct* es;
    err_t ret_err;
    es = (client_struct*)arg;
    if (p == NULL) {
        /* remote host closed connection */
        server_connection_close(tpcb, NULL);
        ret_err = ERR_OK;
    } else if (err != ERR_OK) {
        /* a non empty frame received but for some reason err != ERR_OK */
        pbuf_free(p); /* free received pbuf (nothing do with data)*/
        client_connection_close(tpcb, es);
        ret_err = err;
    } else {
        // copy received data in global buffer
        memcpy((char*)pThis->client_receivedBuf, (const char*)p->payload, p->len);
        pThis->client_lenReceived = p->len;
        // TODO: Set flag to Parse data
        pThis->client_IsReceived = true;

        pbuf_free(p);
        tcp_recved(tpcb, TCP_MSS); // send ACK (with new WND size)
        ret_err = ERR_OK;
    }
    return ret_err;
}
//----------------------------------------------------------
void Tcp::client_send(tcp_pcb* tpcb, client_struct* es) {
    struct pbuf* ptr;
    err_t wr_err = ERR_OK;
    while ((wr_err == ERR_OK) && (es->p_tx != NULL) &&
           (es->p_tx->len <= tcp_sndbuf(tpcb))) {
        ptr = es->p_tx;
        /* enqueue data for transmission */
        wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);
        if (wr_err == ERR_OK) {
            es->p_tx = ptr->next;
            if (es->p_tx != NULL) {
                pbuf_ref(es->p_tx);
            }
            pbuf_free(ptr);
        } else if (wr_err == ERR_MEM) {
            es->p_tx = ptr;
        } else {
            /* other problem ?? */
        }
    }
}
//----------------------------------------------------------
err_t Tcp::client_sent(void* arg, tcp_pcb* tpcb, u16_t len) {
    struct client_struct* es;
    LWIP_UNUSED_ARG(len);
    es = (struct client_struct*)arg;
    if (es->p_tx != NULL) {
        client_send(tpcb, es);
    }
    return ERR_OK;
}
//----------------------------------------------------------
err_t Tcp::client_poll(void* arg, struct tcp_pcb* tpcb) {
    err_t ret_err;
    struct client_struct* es;
    es = (struct client_struct*)arg;
    if (es != NULL) {
        if (es->p_tx != NULL) {
        } else {
            if (es->state == ES_CLOSING) {
                client_connection_close(tpcb, es);
            }
        }
        ret_err = ERR_OK;
    } else {
        tcp_abort(tpcb);
        ret_err = ERR_ABRT;
    }
    return ret_err;
}
