/********************************** (C) COPYRIGHT
 ******************************** File Name          : usb_desc.c Author : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/20
 * Description        : usb device descriptor,configuration descriptor,
 *                      string descriptors and other descriptors.
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

#include "usb_desc.h"

/* Device Descriptor */
const uint8_t MyDevDescr[] = {
    0x12, // bLength
    0x01, // bDescriptorType (Device)
    0x00,
    0x02,               // bcdUSB 2.00
    0x00,               // bDeviceClass
    0x00,               // bDeviceSubClass
    0x00,               // bDeviceProtocol
    DEF_USBD_UEP0_SIZE, // bMaxPacketSize0 64
    (uint8_t)DEF_USB_VID,
    (uint8_t)(DEF_USB_VID >> 8), // idVendor 0x1A86
    (uint8_t)DEF_USB_PID,
    (uint8_t)(DEF_USB_PID >> 8), // idProduct 0xDEAD
    DEF_IC_PRG_VER,
    0x00, // bcdDevice 0.01
    0x01, // iManufacturer (String Index)
    0x02, // iProduct (String Index)
    0x03, // iSerialNumber (String Index)
    0x01, // bNumConfigurations 1
};

/* Configuration Descriptor (FS) */
constexpr uint8_t MyCfgDescr_FS[] = {0x09, 0x02, 0x6B + 23, 0x00};
constexpr uint8_t MyCfgDescr_FSS[0x6b + 32] = {
    /* Configure descriptor */
    0x09, 0x02, 0x6B + 23, 0x00,
    0x04, // bNumInterfaces
    0x01, 0x00, 0x80, 0x32,

    /* IAD Descriptor(interface 0/1)*/
    0x08, 0x0B, 0x00, 0x02, 0x02, 0x02, 0x01, 0x00,

    /* Interface 0 (CDC) descriptor */
    0x09, 0x04, 0x00, 0x00, 0x01, 0x02, 0x02, 0x01, 0x00,

    /* Functional Descriptors */
    0x05, 0x24, 0x00, 0x10, 0x01,

    /* Length/management descriptor (data class interface 1) */
    0x05, 0x24, 0x01, 0x00, 0x01, 0x04, 0x24, 0x02, 0x02, 0x05, 0x24, 0x06,
    0x00, 0x01,

    /* Interrupt upload endpoint descriptor */
    0x07, 0x05, 0x81, 0x03, (uint8_t)DEF_USB_EP3_HS_SIZE,
    (uint8_t)(DEF_USB_EP3_HS_SIZE >> 8), 0x01,

    /* Interface 1 (data interface) descriptor */
    0x09, 0x04, 0x01, 0x00, 0x02, 0x0a, 0x00, 0x00, 0x00,

    /* Endpoint descriptor */
    0x07, 0x05, 0x02, 0x02, (uint8_t)DEF_USB_EP2_HS_SIZE,
    (uint8_t)(DEF_USB_EP2_HS_SIZE >> 8), 0x00,

    /* Endpoint descriptor */
    0x07, 0x05, 0x82, 0x02, (uint8_t)DEF_USB_EP2_HS_SIZE,
    (uint8_t)(DEF_USB_EP2_HS_SIZE >> 8), 0x00,

    /* interface 2 (HID interface) descriptor */
    0x09, 0x04, 0x02, 0x00, 0x02,
    0x03, // bInterfaceClass
    0x00, 0x00, 0x00,

    /* interface 2 HID descriptor */
    0x09, 0x21, 0x00, 0x01, 0x00, 0x01, 0x22, DEF_USBD_REPORT_DESC_LEN, 0x00,

    /* interface 2 endpoint descriptor*/
    0x07, 0x05, 0x83, 0x03, (uint8_t)DEF_USB_EP4_HS_SIZE,
    (uint8_t)(DEF_USB_EP4_HS_SIZE >> 8), 0x01,

    /* interface 2 endpoint descriptor */
    0x07, 0x05, 0x03, 0x03, (uint8_t)DEF_USB_EP4_HS_SIZE,
    (uint8_t)(DEF_USB_EP4_HS_SIZE >> 8), 0x01,
    /* interface 3 (BULK Max speed interface) descriptor */
    0x09, 0x04,
    0x03, // bInterfaceNumber
    0x00, // bAlternateSetting
    0x02, // bNumEndpoints 2
    0x00, // bInterfaceClass own class
    0x00, 0x00, 0x00,

    /* interface 3 endpoint descriptor*/
    0x07, 0x05, 0x84,
    0x02, // Bulk endpoint type
    (uint8_t)DEF_USBD_HS_PACK_SIZE,
    (uint8_t)(DEF_USBD_HS_PACK_SIZE >> 8),
    0x00, // Polling interval in milliseconds

    /* interface 3 endpoint descriptor */
    0x07, 0x05, 0x04,
    0x02, // Bulk endpoint type
    (uint8_t)DEF_USBD_HS_PACK_SIZE,
    (uint8_t)(DEF_USBD_HS_ISO_PACK_SIZE >> 8),
    0x00 // Polling interval in milliseconds
};

/* Configuration Descriptor (HS) */
const uint8_t MyCfgDescr_HS[0x6B + 32] = {
    /* Configure descriptor */
    0x09, 0x02, 0x6B + 32, 0x00,
    0x04, // bNumInterfaces
    0x01, 0x00, 0x80, 0x32,

    /* IAD Descriptor(interface 0/1)*/
    0x08, 0x0B, 0x00, 0x02, 0x02, 0x02, 0x01, 0x00,

    /* Interface 0 (CDC) descriptor */
    0x09, 0x04, 0x00, 0x00, 0x01, 0x02, 0x02, 0x01, 0x00,

    /* Functional Descriptors */
    0x05, 0x24, 0x00, 0x10, 0x01,

    /* Length/management descriptor (data class interface 1) */
    0x05, 0x24, 0x01, 0x00, 0x01, 0x04, 0x24, 0x02, 0x02, 0x05, 0x24, 0x06,
    0x00, 0x01,

    /* Interrupt upload endpoint descriptor */
    0x07, 0x05, 0x81, 0x03, (uint8_t)DEF_USB_EP3_HS_SIZE,
    (uint8_t)(DEF_USB_EP3_HS_SIZE >> 8), 0x01,

    /* Interface 1 (data interface) descriptor */
    0x09, 0x04, 0x01, 0x00, 0x02, 0x0a, 0x00, 0x00, 0x00,

    /* Endpoint 2 CDC descriptor */
    0x07, 0x05, 0x02, 0x02, (uint8_t)DEF_USB_EP2_HS_SIZE,
    (uint8_t)(DEF_USB_EP2_HS_SIZE >> 8), 0x00,

    /* Endpoint 2 CDC descriptor */
    0x07, 0x05, 0x82, 0x02, (uint8_t)DEF_USB_EP2_HS_SIZE,
    (uint8_t)(DEF_USB_EP2_HS_SIZE >> 8), 0x00,

    /* interface 2 (HID interface) descriptor */
    0x09, 0x04, 0x02, 0x00, 0x02,
    0x03, // bInterfaceClass
    0x00, 0x00, 0x00,

    /* interface 2 HID descriptor */
    0x09, 0x21, 0x11, 0x01, 0x00, 0x01, 0x22, DEF_USBD_REPORT_DESC_LEN, 0x00,

    /* interface 2 endpoint 3 descriptor*/
    0x07, 0x05, 0x83, 0x03, (uint8_t)DEF_USB_EP4_HS_SIZE,
    (uint8_t)(DEF_USB_EP4_HS_SIZE >> 8), 0x01,

    /* interface 2 endpoint 3 descriptor */
    0x07, 0x05, 0x03, 0x03, (uint8_t)DEF_USB_EP4_HS_SIZE,
    (uint8_t)(DEF_USB_EP4_HS_SIZE >> 8), 0x01,
    /* interface 3 (BULK Max speed interface) descriptor */
    0x09, 0x04,
    0x03, // bInterfaceNumber
    0x00, // bAlternateSetting
    0x02, // bNumEndpoints 2
    0x00, // bInterfaceClass own class
    0x00, 0x00, 0x00,

    /* interface 3 endpoint 4 descriptor*/
    0x07, 0x05, 0x84,
    0x02, // Bulk endpoint type
    (uint8_t)DEF_USBD_HS_PACK_SIZE,
    (uint8_t)(DEF_USBD_HS_PACK_SIZE >> 8),
    0x00, // Polling interval in milliseconds

    /* interface 3 endpoint 4 descriptor */
    0x07, 0x05, 0x04,
    0x02, // Bulk endpoint type
    (uint8_t)DEF_USBD_HS_PACK_SIZE,
    (uint8_t)(DEF_USBD_HS_PACK_SIZE >> 8),
    0x00 // Polling interval in milliseconds
};

/* HID Report Descriptor MOUSE */
const uint8_t MyHIDReportDesc_FSS[] = {
    0x05, 0x01, // Usage Page (Generic Desktop Ctrls)
    0x09, 0x02, // Usage (Mouse)
    0xA1, 0x01, // Collection (Application)
    0x09, 0x01, // Usage (Pointer)
    0xA1, 0x00, // Collection (Physical)
    0x05, 0x09, // Usage Page (Button)
    0x19, 0x01, // Usage Minimum (0x01)
    0x29, 0x03, // Usage Maximum (0x03)
    0x15, 0x00, // Logical Minimum (0)
    0x25, 0x01, // Logical Maximum (1)
    0x95, 0x03, // Report Count (3)
    0x75, 0x01, // Report Size (1)
    0x81, 0x02, // Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null
                // Position)
    0x95, 0x01, // Report Count (1)
    0x75, 0x05, // Report Size (5)
    0x81, 0x01, // Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null
                // Position)
    0x05, 0x01, // Usage Page (Generic Desktop Ctrls)
    0x09, 0x30, // Usage (X)
    0x09, 0x31, // Usage (Y)
    0x09, 0x38, // Usage (Wheel)
    0x15, 0x81, // Logical Minimum (-127)
    0x25, 0x7F, // Logical Maximum (127)
    0x75, 0x08, // Report Size (8)
    0x95, 0x03, // Report Count (3)
    0x81, 0x06, // Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null
                // Position)
    0xC0,       // End Collection

    0x09, 0x3C, // Usage (Motion Wakeup)
    0x05, 0xFF, // Usage Page (Reserved 0xFF)
    0x09, 0x01, // Usage (0x01)
    0x15, 0x00, // Logical Minimum (0)
    0x25, 0x01, // Logical Maximum (1)
    0x75, 0x01, // Report Size (1)
    0x95, 0x02, // Report Count (2)
    0xB1, 0x22, // Feature (Data,Var,Abs,No Wrap,Linear,No Preferred State,No
                // Null Position,Non-volatile)
    0x75, 0x06, // Report Size (6)
    0x95, 0x01, // Report Count (1)
    0xB1, 0x01, // Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No
                // Null Position,Non-volatile)
    0xC0,       // End Collection
};

/* HID Report Descriptor GAMEPAD */
const uint8_t MyHIDReportDesc_HS[DEF_USBD_REPORT_DESC_LEN] = {
    0x05, 0x01,       // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,       // USAGE (Game Pad)
    0xa1, 0x01,       // COLLECTION (Application)
    0x09, 0x01,       //   USAGE (Pointer)
    0xa1, 0x00,       //   COLLECTION (Physical)
    0x09, 0x30,       //     USAGE (X)
    0x09, 0x31,       //     USAGE (Y)
    0x15, 0x00,       //     LOGICAL_MINIMUM (0)
    0x26, 0xe8, 0x03, //     LOGICAL_MAXIMUM (1000)
    0x75, 0x10,       //     REPORT_SIZE (16)
    0x95, 0x02,       //     REPORT_COUNT (2)
    0x81, 0x02,       //     INPUT (Data,Var,Abs)
    0xc0,             //   END_COLLECTION
    /*
    // second axis
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x09, 0x33,                    //     USAGE (Rx)
    0x09, 0x34,                    //     USAGE (Ry)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, 0xe8, 0x03,              //     LOGICAL_MAXIMUM (1000)
    0x75, 0x10,                    //     REPORT_SIZE (16)
    0x95, 0x02,                    //     REPORT_COUNT (2)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0xc0,                          //   END_COLLECTION
    */
    0x05, 0x09, //   USAGE_PAGE (Button)
    0x19, 0x01, //   USAGE_MINIMUM (Button 1)
    0x29, 0x11, //   USAGE_MAXIMUM (Button 17)
    0x15, 0x00, //   LOGICAL_MINIMUM (0)
    0x25, 0x01, //   LOGICAL_MAXIMUM (1)
    0x75, 0x01, //   REPORT_SIZE (1)
    0x95, 0x17, //   REPORT_COUNT (17) 17 values by one bit
    0x81, 0x02, //   INPUT (Data,Var,Abs)
    // PADDING to one byte for last button
    0x75, 0x01, //   REPORT_SIZE (7)
    0x95, 0x10, //   REPORT_COUNT (1) 1 value by one byte
    0x81, 0x02, //   INPUT (Data,Var,Abs)
    0xc0        // END_COLLECTION
};

/* Language Descriptor */
const uint8_t MyLangDescr[] = {0x04, 0x03, 0x09, 0x04};

/* Manufacturer Descriptor */
const uint8_t MyManuInfo[] = {0x0E, 0x03, 'S', 0,   'Q', 0,   'U',
                              0,    'A',  0,   'R', 0,   'K', 0};

/* Product Information */
const uint8_t MyProdInfo[] = {0x16, 0x03, 'U', 0x00, 'S', 0x00, 'B', 0x00,
                              ' ',  0x00, 'G', 0x00, 'A', 0x00, 'M', 0x00,
                              'I',  0x00, 'N', 0x00, 'G', 0x00};

/* Serial Number Information */
const uint8_t MySerNumInfo[] = {0x16, 0x03, '0', 0x00, '1', 0x00, '2', 0x00,
                                '3',  0x00, '4', 0x00, '5', 0x00, '6', 0x00,
                                '7',  0x00, '8', 0x00, '9', 0x00};

/* Device Qualified Descriptor */
const uint8_t MyQuaDesc[] = {
    0x0A, 0x06, 0x00, 0x02, 0xFF, 0xFF, 0xFF, 0x40, 0x01, 0x00,
};

/* Device BOS Descriptor */
const uint8_t MyBOSDesc[] = {
    0x05, 0x0F, 0x0C, 0x00, 0x01, 0x07, 0x10, 0x02, 0x02, 0x00, 0x00, 0x00,
};

/* USB Full-Speed Mode, Other speed configuration Descriptor */
uint8_t TAB_USB_FS_OSC_DESC[sizeof(MyCfgDescr_HS)] = {
    /* Other parts are copied through the program */
    0x09,
    0x07,
};

/* USB High-Speed Mode, Other speed configuration Descriptor */
uint8_t TAB_USB_HS_OSC_DESC[sizeof(MyCfgDescr_HS)] = {
    /* Other parts are copied through the program */
    0x09,
    0x07,
};
