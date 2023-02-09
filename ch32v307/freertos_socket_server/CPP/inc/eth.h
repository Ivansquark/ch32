#ifndef ETH_H
#define ETH_H

#include "ch32v30x_eth.h"
#include "ch32v30x_gpio.h"
#include "ch32v30x_rcc.h"
#include "ch32v30x_rng.h"
#include "gpio.h"
#include "main.h"
#include "string.h"
#include "systim.h"

#include "lwip_list.h"
#include "memb.h"

#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"

#include "lwip/init.h"
#include "lwip/netif.h"
#include <lwip/arch.h>
#include <lwip/opt.h>
//#include "lwip/dhcp.h"
#include "ethernetif.h"
#include "lwip/def.h"
#include "lwip/timeouts.h"
#include "netif/ethernet.h"

#include "frwrapper.h"
//#include "task.h"

#define ETH_RXBUFNB 4
#define ETH_TXBUFNB 4
#define NET_LED_PERIOD_MSECS 100
/* MII/MDI interface select */
#define PHY_ADDRESS 0x01
#define ETH_DMARxDesc_FrameLengthShift 16
/* Macro */
#ifndef ETH_ERROR
    #define ETH_ERROR ((uint32_t)0)
#endif

#ifndef ETH_SUCCESS
    #define ETH_SUCCESS ((uint32_t)1)
#endif

#define USE10BASE_T

typedef struct {
    void* next;
    u32 length;
    u32 buffer;
    ETH_DMADESCTypeDef* descriptor;
} FrameTypeDef;

extern ETH_DMADESCTypeDef* DMATxDescToSet;
extern ETH_DMADESCTypeDef* DMARxDescToGet;

extern ETH_DMADESCTypeDef DMARxDscrTab[ETH_RXBUFNB];
extern ETH_DMADESCTypeDef DMATxDscrTab[ETH_TXBUFNB];

uint32_t CH30x_RNG_GENERATE();
extern volatile uint8_t net_data_led_require;
LIST_EXTERN(ch307_mac_rec);
MEMB_EXTERN(ch307_mac_rec_frame_mem);
class Eth : FR_OS {
  public:
    Eth(uint8_t ip0 = 0, uint8_t ip1 = 0, uint8_t ip2 = 0, uint8_t ip3 = 0,
        uint16_t stackSize = 256);
    void runTask(void* pvParameters = nullptr);
    static Eth* pThis;
    // @fn      ETH_RxPkt_ChainMode
    // @brief   MAC receive a ethernet frame in chain mode.
    // @return  Frame information.
    static void* ETH_RxPkt_ChainMode(void);
    // @fn      ETH_TxPkt_ChainMode
    // @brief   MAC send a ethernet frame in chain mode.
    // @param   Send length
    // @return  Send status.
    static uint32_t ETH_TxPkt_ChainMode(u16 FrameLength);
    void mac_send(uint8_t* content_ptr, uint16_t content_len);

    static void net_led_tmr(void);

    void rx_handler();

    static uint8_t RxBuff[2048];
    static uint8_t TxBuff[2048];
    static uint16_t currentRxBuffLen;
    static uint16_t currentTxBuffLen;

    uint8_t IP_ADDRESS[4];

    void init_lwip();
    struct netif WCH_NetIf;

    bool IsInterrupt = false;

    void delay(volatile uint32_t us);

  private:
    void init();
    void init_phy();
    static void LED_LINKSET(bool state);
    static void LED_DATASET(bool state);

    ip4_addr_t ipaddr;
    ip4_addr_t netmask;
    ip4_addr_t gw;
    uint8_t NETMASK_ADDRESS[4];
    uint8_t GATEWAY_ADDRESS[4];
};

extern "C"
    //__attribute__((interrupt("WCH-Interrupt-fast")))
    __attribute__((interrupt)) void
    ETH_IRQHandler(void);

extern "C" __attribute__((weak)) uint32_t xTaskGetTickCount(void);
#endif // ETH_H
