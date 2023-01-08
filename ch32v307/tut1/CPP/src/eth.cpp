#include "eth.h"

/* Globe variable */
LIST(ch307_mac_rec);
MEMB(ch307_mac_rec_frame_mem, FrameTypeDef, ETH_RXBUFNB);

__attribute__((aligned(4))) ETH_DMADESCTypeDef DMARxDscrTab[ETH_RXBUFNB];
__attribute__((aligned(4))) ETH_DMADESCTypeDef DMATxDscrTab[ETH_TXBUFNB];
__attribute__((aligned(4))) uint8_t Rx_Buff[ETH_RXBUFNB][ETH_MAX_PACKET_SIZE];
__attribute__((aligned(4))) uint8_t Tx_Buff[ETH_TXBUFNB][ETH_MAX_PACKET_SIZE];

Eth::Eth() { init(); }

void Eth::init() {
    SysTim::init();
    __enable_irq();
    init_lwip();
}

void Eth::rx_handler() {
    if (list_head(ch307_mac_rec) != NULL) {
        /* received a packet */
        ethernetif_input(&WCH_NetIf);
    }
    sys_check_timeouts();
}

void Eth::init_phy() {
    /* SET_MCO */
    GPIO_InitTypeDef GPIO;
    memset(&GPIO, 0, sizeof(GPIO_InitTypeDef));
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO.GPIO_Pin = GPIO_Pin_8;
    GPIO.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO);

    RCC_PLL3Cmd(DISABLE);
    RCC_PREDIV2Config(RCC_PREDIV2_Div2);
    RCC_PLL3Config(RCC_PLL3Mul_15);
    RCC_MCOConfig(RCC_MCO_PLL3CLK);
    RCC_PLL3Cmd(ENABLE);
    if (RESET == RCC_GetFlagStatus(RCC_FLAG_PLL3RDY)) {
        SysTim::delay_10us(100);
    }
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    /* Ethernet LED Configuration */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO);
    LED_LINKSET(1); /* turn off link led. */
    LED_DATASET(1); /* turn off data led. */

    /* Ethernet_Configuration */
    /* MUST use static in OS_TASK */
    ETH_InitTypeDef* ETH_InitStructure;
    uint32_t timeout;

    // allocate structure
    ETH_InitStructure = (ETH_InitTypeDef*)mem_malloc(sizeof(ETH_InitTypeDef));
    memset(ETH_InitStructure, 0, sizeof(ETH_InitTypeDef));
    /* Enable Ethernet MAC clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETH_MAC | RCC_AHBPeriph_ETH_MAC_Tx |
                              RCC_AHBPeriph_ETH_MAC_Rx,
                          ENABLE);

#ifdef USE10BASE_T
    /* Enable internal 10BASE-T PHY*/
    EXTEN->EXTEN_CTR |= EXTEN_ETH_10M_EN;
#endif
#ifdef USE_GIGA_MAC
    /* Enable 1G MAC*/
    EXTEN->EXTEN_CTR |= EXTEN_ETH_RGMII_SEL;       /* 1G MAC */
    RCC_ETH1GCLKConfig(RCC_ETH1GCLKSource_PB1_IN); /* 125MHz */
    RCC_ETH1G_125Mcmd(ENABLE);                     /* 125MHz */
    /*  Enable RGMII GPIO */
    GETH_pin_init();
#endif
#ifdef USE_FAST_MAC
    /*  Enable MII or RMII GPIO */
    FETH_pin_init();
#endif

    /* Reset ETHERNET on AHB Bus */
    ETH_DeInit();
    /* Software reset */
    ETH_SoftwareReset();

    /* Wait for software reset */
    timeout = 10;
    if (ETH->DMABMR & ETH_DMABMR_SR) {
        timeout--;
        if (timeout == 0) {}
        SysTim::delay_10us(100);
    }

    /* ETHERNET Configuration
     * ------------------------------------------------------*/
    /* Call ETH_StructInit if you don't like to configure all ETH_InitStructure
     * parameter */
    ETH_StructInit(ETH_InitStructure);
    /* Fill ETH_InitStructure parametrs */
    /*------------------------   MAC   -----------------------------------*/
    ETH_InitStructure->ETH_Mode = ETH_Mode_FullDuplex;
    ETH_InitStructure->ETH_Speed = ETH_Speed_10M; // ETH_Speed_1000M;
    ETH_InitStructure->ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;
    ETH_InitStructure->ETH_LoopbackMode = ETH_LoopbackMode_Disable;
    ETH_InitStructure->ETH_RetryTransmission = ETH_RetryTransmission_Disable;
    ETH_InitStructure->ETH_AutomaticPadCRCStrip =
        ETH_AutomaticPadCRCStrip_Disable;
    ETH_InitStructure->ETH_ReceiveAll = ETH_ReceiveAll_Enable;
    ETH_InitStructure->ETH_BroadcastFramesReception =
        ETH_BroadcastFramesReception_Enable;
    ETH_InitStructure->ETH_PromiscuousMode = ETH_PromiscuousMode_Enable;
    ETH_InitStructure->ETH_MulticastFramesFilter =
        ETH_MulticastFramesFilter_Perfect;
    ETH_InitStructure->ETH_UnicastFramesFilter =
        ETH_UnicastFramesFilter_Perfect;
#ifdef CHECKSUM_BY_HARDWARE
    ETH_InitStructure->ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
#endif
    /*------------------------   DMA   -----------------------------------*/
    /* When we use the Checksum offload feature, we need to enable the Store and
    Forward mode: the store and forward guarantee that a whole frame is stored
    in the FIFO, so the MAC can insert/verify the checksum, if the checksum is
    OK the DMA can handle the frame otherwise the frame is dropped */
    ETH_InitStructure->ETH_DropTCPIPChecksumErrorFrame =
        ETH_DropTCPIPChecksumErrorFrame_Enable;
    ETH_InitStructure->ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;
    ETH_InitStructure->ETH_TransmitStoreForward =
        ETH_TransmitStoreForward_Enable;
    ETH_InitStructure->ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Enable;
    ETH_InitStructure->ETH_ForwardUndersizedGoodFrames =
        ETH_ForwardUndersizedGoodFrames_Enable;
    ETH_InitStructure->ETH_SecondFrameOperate = ETH_SecondFrameOperate_Disable;
    ETH_InitStructure->ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;
    ETH_InitStructure->ETH_FixedBurst = ETH_FixedBurst_Enable;
    ETH_InitStructure->ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;
    ETH_InitStructure->ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;
    ETH_InitStructure->ETH_DMAArbitration =
        ETH_DMAArbitration_RoundRobin_RxTx_2_1;

    /* Configure Ethernet */
    uint32_t tmpreg = 0;
    static uint16_t RegValue = 0;

    /* SMI  */
    tmpreg = ETH->MACMIIAR;
    tmpreg &= MACMIIAR_CR_MASK;
    tmpreg |= (uint32_t)ETH_MACMIIAR_CR_Div42;
    ETH->MACMIIAR = (uint32_t)tmpreg;

    ETH_WritePHYRegister(PHY_ADDRESS, PHY_BCR, PHY_Reset);

    // delay
    SysTim::delay_10us(1000); // 10 ms

    timeout = 10000;
    RegValue = 0;
    RegValue = ETH_ReadPHYRegister(PHY_ADDRESS, PHY_BCR);
    if ((RegValue & (PHY_Reset))) {
        timeout--;
        if (timeout <= 0) {
            while (1) {}
        }
        SysTim::delay_10us(100); // 1 ms
    }

    timeout = 10000;
    RegValue = 0;
    RegValue = ETH_ReadPHYRegister(PHY_ADDRESS, PHY_BSR);
    if ((RegValue & (PHY_Linked_Status)) == 0) {
        timeout--;
        if (timeout <= 0) {
            // not linked
            while (1) {}
        }
        SysTim::delay_10us(100); // 1 ms
    }

    timeout = 10000;
    RegValue = 0;
    RegValue = ETH_ReadPHYRegister(PHY_ADDRESS, PHY_BSR);
    if ((RegValue & PHY_AutoNego_Complete) == 0) {
        timeout--;
        if (timeout <= 0) {
            // autonegotiation fault
            while (1) {}
        }
        SysTim::delay_10us(100); // 1 ms
    }

    RegValue = ETH_ReadPHYRegister(PHY_ADDRESS, 0x10);

    if (RegValue & (1 << 2)) {
        ETH_InitStructure->ETH_Mode = ETH_Mode_FullDuplex;
    } else {
        ETH_InitStructure->ETH_Mode = ETH_Mode_HalfDuplex;
    }
    ETH_InitStructure->ETH_Speed = ETH_Speed_10M;
    if (RegValue & (1 << 3)) {
        // Loopback_10M
    } else {
    }

    SysTim::delay_10us(1000);
    LED_LINKSET(0);

    //------------------------ MAC  -----------------------
    /* MACCCR RGMII */
    tmpreg = ETH->MACCR;
    tmpreg &= MACCR_CLEAR_MASK;
    tmpreg |= (uint32_t)(ETH_InitStructure->ETH_Watchdog |
                         ETH_InitStructure->ETH_Jabber |
                         ETH_InitStructure->ETH_InterFrameGap |
                         ETH_InitStructure->ETH_CarrierSense |
                         ETH_InitStructure->ETH_Speed |
                         ETH_InitStructure->ETH_ReceiveOwn |
                         ETH_InitStructure->ETH_LoopbackMode |
                         ETH_InitStructure->ETH_Mode |
                         ETH_InitStructure->ETH_ChecksumOffload |
                         ETH_InitStructure->ETH_RetryTransmission |
                         ETH_InitStructure->ETH_AutomaticPadCRCStrip |
                         ETH_InitStructure->ETH_BackOffLimit |
                         ETH_InitStructure->ETH_DeferralCheck);
    /* MAC */
    ETH->MACCR = (uint32_t)tmpreg;
#ifdef USE10BASE_T
    ETH->MACCR |= ETH_Internal_Pull_Up_Res_Enable;
#endif
    ETH->MACFFR = (uint32_t)(ETH_InitStructure->ETH_ReceiveAll |
                             ETH_InitStructure->ETH_SourceAddrFilter |
                             ETH_InitStructure->ETH_PassControlFrames |
                             ETH_InitStructure->ETH_BroadcastFramesReception |
                             ETH_InitStructure->ETH_DestinationAddrFilter |
                             ETH_InitStructure->ETH_PromiscuousMode |
                             ETH_InitStructure->ETH_MulticastFramesFilter |
                             ETH_InitStructure->ETH_UnicastFramesFilter);
    /*- ETHERNET MACHTHR and MACHTLR Configuration -----------*/
    /* Write to ETHERNET MACHTHR */
    ETH->MACHTHR = (uint32_t)ETH_InitStructure->ETH_HashTableHigh;
    /* Write to ETHERNET MACHTLR */
    ETH->MACHTLR = (uint32_t)ETH_InitStructure->ETH_HashTableLow;
    /*------ ETHERNET MACFCR Configuration --------------------*/
    /* Get the ETHERNET MACFCR value */
    tmpreg = ETH->MACFCR;
    /* Clear xx bits */
    tmpreg &= MACFCR_CLEAR_MASK;

    tmpreg |= (uint32_t)((ETH_InitStructure->ETH_PauseTime << 16) |
                         ETH_InitStructure->ETH_ZeroQuantaPause |
                         ETH_InitStructure->ETH_PauseLowThreshold |
                         ETH_InitStructure->ETH_UnicastPauseFrameDetect |
                         ETH_InitStructure->ETH_ReceiveFlowControl |
                         ETH_InitStructure->ETH_TransmitFlowControl);
    ETH->MACFCR = (uint32_t)tmpreg;

    ETH->MACVLANTR = (uint32_t)(ETH_InitStructure->ETH_VLANTagComparison |
                                ETH_InitStructure->ETH_VLANTagIdentifier);

    tmpreg = ETH->DMAOMR;
    /* Clear xx bits */
    tmpreg &= DMAOMR_CLEAR_MASK;

    tmpreg |= (uint32_t)(ETH_InitStructure->ETH_DropTCPIPChecksumErrorFrame |
                         ETH_InitStructure->ETH_ReceiveStoreForward |
                         ETH_InitStructure->ETH_FlushReceivedFrame |
                         ETH_InitStructure->ETH_TransmitStoreForward |
                         ETH_InitStructure->ETH_TransmitThresholdControl |
                         ETH_InitStructure->ETH_ForwardErrorFrames |
                         ETH_InitStructure->ETH_ForwardUndersizedGoodFrames |
                         ETH_InitStructure->ETH_ReceiveThresholdControl |
                         ETH_InitStructure->ETH_SecondFrameOperate);
    ETH->DMAOMR = (uint32_t)tmpreg;

    ETH->DMABMR =
        (uint32_t)(ETH_InitStructure->ETH_AddressAlignedBeats |
                   ETH_InitStructure->ETH_FixedBurst |
                   ETH_InitStructure
                       ->ETH_RxDMABurstLength | /* !! if 4xPBL is selected for
                                                   Tx or Rx it is applied for
                                                   the other */
                   ETH_InitStructure->ETH_TxDMABurstLength |
                   (ETH_InitStructure->ETH_DescriptorSkipLength << 2) |
                   ETH_InitStructure->ETH_DMAArbitration | ETH_DMABMR_USP);
    mem_free(ETH_InitStructure);
    /* Enable the Ethernet Rx Interrupt */
    ETH_DMAITConfig(ETH_DMA_IT_NIS | ETH_DMA_IT_R | ETH_DMA_IT_T, ENABLE);

    NVIC_EnableIRQ(ETH_IRQn);
    ETH_DMATxDescChainInit(DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);
    ETH_DMARxDescChainInit(DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);
    ETH_Start();
}

void Eth::LED_LINKSET(bool state) {
    if (state) {
        // GPIO_SetBits(GPIOC, GPIO_Pin_0);
        GPIOC->BSHR = GPIO_BSHR_BS0;
    } else {
        // GPIO_ResetBits(GPIOC, GPIO_Pin_0);
        GPIOC->BSHR = GPIO_BSHR_BR0;
    }
}
void Eth::LED_DATASET(bool state) {
    if (state) {
        // GPIO_SetBits(GPIOC, GPIO_Pin_1);
        GPIOC->BSHR = GPIO_BSHR_BS1;
    } else {
        // GPIO_ResetBits(GPIOC, GPIO_Pin_1);
        GPIOC->BSHR = GPIO_BSHR_BR1;
    }
}
void Eth::init_lwip() {
    /* mem_init of lwip, init outside the lwip_init for user to using outside.
     */
    mem_init();

    /* call sub task ch307 init phy */
    init_phy();

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_RNG, ENABLE);
    RNG_Cmd(ENABLE);
#if LWIP_DHCP
    ip_addr_set_zero_ip4(&(WCH_NetIf.ip_addr));
    ip_addr_set_zero_ip4(&ipaddr);
    ip_addr_set_zero_ip4(&netmask);
    ip_addr_set_zero_ip4(&gw);
#else
    /* IP addresses initialization */
    IP4_ADDR(&ipaddr, 192, 168, 1, 100);
    IP4_ADDR(&netmask, 255, 255, 255, 0);
    IP4_ADDR(&gw, 192, 168, 1, 1);
#endif
    /* Initilialize the LwIP stack without RTOS */
    lwip_init();

    /* add the network interface (IPv4/IPv6) without RTOS */
    netif_add(&WCH_NetIf, &ipaddr, &netmask, &gw, NULL, &ethernetif_init,
              &ethernet_input);

    /* Registers the default network interface */
    netif_set_default(&WCH_NetIf);

    if (netif_is_link_up(&WCH_NetIf)) {
        /* When the netif is fully configured this function must be called */
        netif_set_up(&WCH_NetIf);
#if LWIP_DHCP
        int err;
        /*  Creates a new DHCP client for this interface on the first call.
        Note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
        the predefined regular intervals after starting the client.
        You can peek in the netif->dhcp struct for the actual DHCP status.*/
        err = dhcp_start(&WCH_NetIf); //开启dhcp
        if (err == ERR_OK) {
            printf("lwip dhcp start success...\n\n");
        } else {
            printf("lwip dhcp start fail...\n\n");
        }
        sys_timeout(50, wait_dhcp, NULL);
#else
        // lwip_init_success_callback(&(WCH_NetIf.ip_addr));

#endif
        if (list_head(ch307_mac_rec) != NULL) {
            /* received a packet */
            ethernetif_input(&WCH_NetIf);
        }
    } else {
        /* When the netif link is down this function must be called */
        netif_set_down(&WCH_NetIf);
    }
}

void* Eth::ETH_RxPkt_ChainMode(void) {
    uint32_t framelength = 0;
    FrameTypeDef* rec_frame;
    rec_frame = (FrameTypeDef*)memb_alloc(&ch307_mac_rec_frame_mem);
    /* Check if the descriptor is owned by the ETHERNET DMA (when set) or CPU
     * (when reset) */
    if ((DMARxDescToGet->Status & ETH_DMARxDesc_OWN) != (u32)RESET) {
        if ((ETH->DMASR & ETH_DMASR_RBUS) != (u32)RESET) {
            /* Clear RBUS ETHERNET DMA flag */
            ETH->DMASR = ETH_DMASR_RBUS;
            /* Resume DMA reception */
            ETH->DMARPDR = 0;
        }
        if (rec_frame != NULL) {
            rec_frame->length = ETH_ERROR;
            memb_free(&ch307_mac_rec_frame_mem, rec_frame);
        }
        /* Return error: OWN bit set */
        return NULL;
    }

    if (rec_frame == NULL) { return NULL; }

    if (((DMARxDescToGet->Status & ETH_DMARxDesc_ES) == (u32)RESET) &&
        ((DMARxDescToGet->Status & ETH_DMARxDesc_LS) != (u32)RESET) &&
        ((DMARxDescToGet->Status & ETH_DMARxDesc_FS) != (u32)RESET)) {
        /* Get the Frame Length of the received packet: substruct 4 bytes of the
         * CRC */
        framelength = ((DMARxDescToGet->Status & ETH_DMARxDesc_FL) >>
                       ETH_DMARxDesc_FrameLengthShift) -
                      4;
        /* Get the addrees of the actual buffer */
        rec_frame->buffer = DMARxDescToGet->Buffer1Addr;
    } else {
        /* Return ERROR */
        framelength = ETH_ERROR;
    }
    // DMARxDescToGet->Status |= ETH_DMARxDesc_OWN; //lwip low level input
    rec_frame->length = framelength;
    rec_frame->descriptor = DMARxDescToGet;

    /* Update the ETHERNET DMA global Rx descriptor with next Rx decriptor */
    /* Chained Mode */
    /* Selects the next DMA Rx descriptor list for next buffer to read */
    DMARxDescToGet = reinterpret_cast<ETH_DMADESCTypeDef*>(
        DMARxDescToGet->Buffer2NextDescAddr);

    return rec_frame;
}

uint32_t Eth::ETH_TxPkt_ChainMode(u16 FrameLength) {
    /* Check if the descriptor is owned by the ETHERNET DMA (when set) or CPU
     * (when reset) */
    if ((DMATxDescToSet->Status & ETH_DMATxDesc_OWN) != (u32)RESET) {
        /* Return ERROR: OWN bit set */
        return ETH_ERROR;
    }

    /* Setting the Frame Length: bits[12:0] */
    DMATxDescToSet->ControlBufferSize = (FrameLength & ETH_DMATxDesc_TBS1);
#ifdef CHECKSUM_BY_HARDWARE
    /* Setting the last segment and first segment bits (in this case a frame is
     * transmitted in one descriptor) */
    DMATxDescToSet->Status |=
        ETH_DMATxDesc_LS | ETH_DMATxDesc_FS | ETH_DMATxDesc_CIC_TCPUDPICMP_Full;
#else
    DMATxDescToSet->Status |= ETH_DMATxDesc_LS | ETH_DMATxDesc_FS;
#endif
    /* Set Own bit of the Tx descriptor Status: gives the buffer back to
     * ETHERNET DMA */
    DMATxDescToSet->Status |= ETH_DMATxDesc_OWN;
    /* When Tx Buffer unavailable flag is set: clear it and resume transmission
     */
    if ((ETH->DMASR & ETH_DMASR_TBUS) != (u32)RESET) {
        /* Clear TBUS ETHERNET DMA flag */
        ETH->DMASR = ETH_DMASR_TBUS;
        /* Resume DMA transmission*/
        ETH->DMATPDR = 0;
    }
    /* Update the ETHERNET DMA global Tx descriptor with next Tx decriptor */
    /* Chained Mode */
    /* Selects the next DMA Tx descriptor list for next buffer to send */
    DMATxDescToSet = reinterpret_cast<ETH_DMADESCTypeDef*>(
        DMATxDescToSet->Buffer2NextDescAddr);

    /* Return SUCCESS */
    return ETH_SUCCESS;
}

void Eth::mac_send(uint8_t* content_ptr, uint16_t content_len) {
    u8* buffer = reinterpret_cast<u8*>(ETH_GetCurrentTxBufferAddress());
    memcpy(buffer, content_ptr, content_len);
    if (!ETH_TxPkt_ChainMode(content_len)) {
        // printf("Send failed.\n");
    }
}

volatile uint8_t net_data_led_require = 0;
void Eth::net_led_tmr(void) {
    static uint8_t net_data_led = 0;
    if (net_data_led) {
        net_data_led = 0;
        LED_DATASET(1); /* turn off data led. */
    } else {
        if (net_data_led_require != 0) {
            net_data_led = 1;
            LED_DATASET(0); /* turn on data led. */
            net_data_led_require = 0;
        }
    }
}

u32_t sys_now(void) { return SysTim::getCounter(); }

uint32_t CH30x_RNG_GENERATE() {
    while (1) {
        if (RNG_GetFlagStatus(RNG_FLAG_DRDY) == SET) { break; }
        if (RNG_GetFlagStatus(RNG_FLAG_CECS) == SET) {
            RNG_ClearFlag(RNG_FLAG_CECS);
            SysTim::delay_10us(10);
        }
        if (RNG_GetFlagStatus(RNG_FLAG_SECS) == SET) {
            RNG_ClearFlag(RNG_FLAG_SECS);
            RNG_Cmd(DISABLE);
            SysTim::delay_10us(10);
            RNG_Cmd(ENABLE);
            SysTim::delay_10us(10);
        }
    }
    return RNG_GetRandomNumber();
}
/*********************************************************************
 * @fn      ETH_IRQHandler
 * @brief   This function handles ETH exception.
 * @return  none
 */
void ETH_IRQHandler(void) {
    void* p;
    if (ETH->DMASR & ETH_DMA_IT_R) {
        ETH_DMAClearITPendingBit(ETH_DMA_IT_R);

        p = Eth::ETH_RxPkt_ChainMode();
        if (p != NULL) { list_add(ch307_mac_rec, p); /* add to rec list. */ }
    }
    if (ETH->DMASR & ETH_DMA_IT_T) { ETH_DMAClearITPendingBit(ETH_DMA_IT_T); }

    ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);
}
