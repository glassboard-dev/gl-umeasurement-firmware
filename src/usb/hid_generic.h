/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __USB_DEVICE_HID_GENERIC_H__
#define __USB_DEVICE_HID_GENERIC_H__

#include "usb.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CONTROLLER_ID kUSB_ControllerLpcIp3511Hs0

#define USB_DEVICE_INTERRUPT_PRIORITY (3U)
typedef struct _usb_hid_generic_struct
{
    usb_device_handle deviceHandle;
    uint8_t *buffer[3];
    uint8_t bufferIndex;
    uint8_t idleRate;
    uint8_t speed;
    uint8_t attach;
} usb_hid_generic_struct_t;

typedef void(*CB_usb_complete_fptr_t)(void);

/*******************************************************************************
 * API
 ******************************************************************************/
void USB_DeviceApplicationInit(uint8_t *INT_IN_EP_BUFF, uint8_t *INT_OUT_EP_BUFF, uint8_t *ISO_IN_EP_BUFF, CB_usb_complete_fptr_t cb);
void USB_DeviceReqRead(void);

#endif /* __USB_DEVICE_HID_GENERIC_H__ */
