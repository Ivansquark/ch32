#ifndef USB_H
#define USB_H

#include "main.h"
#include "ch32v30x_usbhs_device.h"

class Usb {
  public:
    Usb();

  private:
    void init();
    void init_rcc();
    void init_phy();
};

#endif // USB_H
