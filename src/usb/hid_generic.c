/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017, 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_hid.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"
#include "hid_generic.h"

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"
#include "usb_phy.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitHardware(void);
void USB_DeviceClockInit(void);
void USB_DeviceIsrEnable(void);

static usb_status_t USB_DeviceHidGenericInterruptIn(usb_device_handle handle,
                                                    usb_device_endpoint_callback_message_struct_t *message,
                                                    void *callbackParam);
static usb_status_t USB_DeviceHidGenericInterruptOut(usb_device_handle handle,
                                                     usb_device_endpoint_callback_message_struct_t *message,
                                                     void *callbackParam);

/*******************************************************************************
 * Variables
 ******************************************************************************/

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_SetupOutBuffer[8];
// USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
// static uint32_t s_GenericBuffer0[USB_HID_GENERIC_OUT_BUFFER_LENGTH >> 2];
// USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
// static uint32_t s_GenericBuffer1[USB_HID_GENERIC_OUT_BUFFER_LENGTH >> 2];
usb_hid_generic_struct_t g_UsbDeviceHidGeneric;
static CB_usb_complete_fptr_t usb_cb = NULL;

extern uint8_t g_UsbDeviceCurrentConfigure;
extern uint8_t g_UsbDeviceInterface[USB_HID_GENERIC_INTERFACE_COUNT];

/*******************************************************************************
 * Code
 ******************************************************************************/
void USB1_IRQHandler(void)
{
    USB_DeviceLpcIp3511IsrFunction(g_UsbDeviceHidGeneric.deviceHandle);
}

void USB_DeviceClockInit(void)
{
    /* enable USB IP clock */
    CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_UsbPhySrcExt, BOARD_XTAL0_CLK_HZ);
    CLOCK_EnableUsbhs0DeviceClock(kCLOCK_UsbSrcUnused, 0U);
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, NULL);
#if defined(FSL_FEATURE_USBHSD_USB_RAM) && (FSL_FEATURE_USBHSD_USB_RAM)
    for (int i = 0; i < FSL_FEATURE_USBHSD_USB_RAM; i++)
    {
        ((uint8_t *)FSL_FEATURE_USBHSD_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
    }
#endif
}
void USB_DeviceIsrEnable(void)
{
    uint8_t irqNumber;
    uint8_t usbDeviceIP3511Irq[] = USBHSD_IRQS;
    irqNumber                    = usbDeviceIP3511Irq[CONTROLLER_ID - kUSB_ControllerLpcIp3511Hs0];
    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ((IRQn_Type)irqNumber);
}

/* The hid generic interrupt IN endpoint callback */
static usb_status_t USB_DeviceHidGenericInterruptIn(usb_device_handle handle,
                                                    usb_device_endpoint_callback_message_struct_t *message,
                                                    void *callbackParam)
{
    // uint8_t buff[10] = {'B', 'E', 'E', 'F', '\0'};
    // memcpy((void*)&g_UsbDeviceHidGeneric.buffer[1][0], buff, 5);
    if (g_UsbDeviceHidGeneric.attach)
    {
        // Call our USB callback.
        usb_cb();
        return kStatus_USB_Error;
    }

    return kStatus_USB_Error;
}

/* The hid generic interrupt OUT endpoint callback */
static usb_status_t USB_DeviceHidGenericInterruptOut(usb_device_handle handle,
                                                     usb_device_endpoint_callback_message_struct_t *message,
                                                     void *callbackParam)
{
    if (g_UsbDeviceHidGeneric.attach)
    {
        // Initiate a RCV
        return USB_DeviceRecvRequest(g_UsbDeviceHidGeneric.deviceHandle, USB_HID_GENERIC_INT_ENDPOINT_OUT,
                                     (uint8_t *)&g_UsbDeviceHidGeneric.buffer[0][0],
                                     USB_HID_GENERIC_OUT_BUFFER_LENGTH);
    }

    return kStatus_USB_Error;
}

/* The device callback */
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Success;
    uint8_t *temp8     = (uint8_t *)param;

    switch (event)
    {
        case kUSB_DeviceEventBusReset:
        {
            /* USB bus reset signal detected */
            /* Initialize the control pipes */
            USB_DeviceControlPipeInit(g_UsbDeviceHidGeneric.deviceHandle);
            g_UsbDeviceHidGeneric.attach = 0U;

            /* Get USB speed to configure the device, including max packet size and interval of the endpoints. */
            if (kStatus_USB_Success == USB_DeviceGetStatus(g_UsbDeviceHidGeneric.deviceHandle, kUSB_DeviceStatusSpeed,
                                                           &g_UsbDeviceHidGeneric.speed))
            {
                USB_DeviceSetSpeed(g_UsbDeviceHidGeneric.speed);
            }
        }
        break;
        case kUSB_DeviceEventSetConfiguration:
            if (USB_HID_GENERIC_CONFIGURE_INDEX == (*temp8))
            {
                /* If the confguration is valid, initliaze the HID generic interrupt IN pipe */
                usb_device_endpoint_init_struct_t epInitStruct;
                usb_device_endpoint_callback_struct_t epCallback;

                epCallback.callbackFn    = USB_DeviceHidGenericInterruptIn;
                epCallback.callbackParam = handle;

                epInitStruct.zlt          = 0U;
                epInitStruct.transferType = USB_ENDPOINT_INTERRUPT;
                epInitStruct.endpointAddress = USB_HID_GENERIC_INT_ENDPOINT_IN | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
                if (USB_SPEED_HIGH == g_UsbDeviceHidGeneric.speed)
                {
                    epInitStruct.maxPacketSize = HS_HID_GENERIC_INTERRUPT_IN_PACKET_SIZE;
                    epInitStruct.interval      = HS_HID_GENERIC_INTERRUPT_IN_INTERVAL;
                }
                else
                {
                    epInitStruct.maxPacketSize = FS_HID_GENERIC_INTERRUPT_IN_PACKET_SIZE;
                    epInitStruct.interval      = FS_HID_GENERIC_INTERRUPT_IN_INTERVAL;
                }

                USB_DeviceInitEndpoint(handle, &epInitStruct, &epCallback);

                epCallback.callbackFn    = USB_DeviceHidGenericInterruptOut;
                epCallback.callbackParam = handle;

                epInitStruct.zlt          = 0U;
                epInitStruct.transferType = USB_ENDPOINT_INTERRUPT;
                epInitStruct.endpointAddress = USB_HID_GENERIC_INT_ENDPOINT_OUT | (USB_OUT << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
                if (USB_SPEED_HIGH == g_UsbDeviceHidGeneric.speed)
                {
                    epInitStruct.maxPacketSize = HS_HID_GENERIC_INTERRUPT_OUT_PACKET_SIZE;
                    epInitStruct.interval      = HS_HID_GENERIC_INTERRUPT_OUT_INTERVAL;
                }
                else
                {
                    epInitStruct.maxPacketSize = FS_HID_GENERIC_INTERRUPT_OUT_PACKET_SIZE;
                    epInitStruct.interval      = FS_HID_GENERIC_INTERRUPT_OUT_INTERVAL;
                }

                USB_DeviceInitEndpoint(handle, &epInitStruct, &epCallback);

                g_UsbDeviceHidGeneric.attach = 1U;

                error &= USB_DeviceRecvRequest(
                    g_UsbDeviceHidGeneric.deviceHandle, USB_HID_GENERIC_INT_ENDPOINT_OUT,
                    (uint8_t *)&g_UsbDeviceHidGeneric.buffer[0][0],
                    USB_HID_GENERIC_OUT_BUFFER_LENGTH);
                error &= USB_DeviceSendRequest(
                    g_UsbDeviceHidGeneric.deviceHandle, USB_HID_GENERIC_INT_ENDPOINT_IN,
                    (uint8_t *)&g_UsbDeviceHidGeneric.buffer[1][0],
                    USB_HID_GENERIC_IN_BUFFER_LENGTH);
            }
            break;
        default:
            break;
    }

    return error;
}

/* Get setup buffer */
usb_status_t USB_DeviceGetSetupBuffer(usb_device_handle handle, usb_setup_struct_t **setupBuffer)
{
    /* Keep the setup is 4-byte aligned */
    static uint32_t hid_generic_setup[2];
    if (NULL == setupBuffer)
    {
        return kStatus_USB_InvalidParameter;
    }
    *setupBuffer = (usb_setup_struct_t *)&hid_generic_setup;
    return kStatus_USB_Success;
}

/* Configure device remote wakeup */
usb_status_t USB_DeviceConfigureRemoteWakeup(usb_device_handle handle, uint8_t enable)
{
    return kStatus_USB_InvalidRequest;
}

/* Configure the endpoint status (idle or stall) */
usb_status_t USB_DeviceConfigureEndpointStatus(usb_device_handle handle, uint8_t ep, uint8_t status)
{
    if (status)
    {
        if (((USB_HID_GENERIC_INT_ENDPOINT_IN == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
             (ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK)) ||
            ((USB_HID_GENERIC_INT_ENDPOINT_OUT == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
             (!(ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK))))
        {
            return USB_DeviceStallEndpoint(handle, ep);
        }
        else
        {
        }
    }
    else
    {
        if (((USB_HID_GENERIC_INT_ENDPOINT_IN == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
             (ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK)) ||
            ((USB_HID_GENERIC_INT_ENDPOINT_OUT == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
             (!(ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK))))
        {
            return USB_DeviceUnstallEndpoint(handle, ep);
        }
        else
        {
        }
    }
    return kStatus_USB_InvalidRequest;
}

/* Get class-specific request buffer */
usb_status_t USB_DeviceGetClassReceiveBuffer(usb_device_handle handle,
                                             usb_setup_struct_t *setup,
                                             uint32_t *length,
                                             uint8_t **buffer)
{
    if ((NULL == buffer) || ((*length) > sizeof(s_SetupOutBuffer)))
    {
        return kStatus_USB_InvalidRequest;
    }
    *buffer = s_SetupOutBuffer;
    return kStatus_USB_Success;
}

/* Handle class-specific request */
usb_status_t USB_DeviceProcessClassRequest(usb_device_handle handle,
                                           usb_setup_struct_t *setup,
                                           uint32_t *length,
                                           uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;

    if (setup->wIndex != USB_HID_GENERIC_INTERFACE_INDEX)
    {
        return error;
    }

    switch (setup->bRequest)
    {
        case USB_DEVICE_HID_REQUEST_GET_REPORT:
            break;
        case USB_DEVICE_HID_REQUEST_GET_IDLE:
            break;
        case USB_DEVICE_HID_REQUEST_GET_PROTOCOL:
            break;
        case USB_DEVICE_HID_REQUEST_SET_REPORT:
            break;
        case USB_DEVICE_HID_REQUEST_SET_IDLE:
            break;
        case USB_DEVICE_HID_REQUEST_SET_PROTOCOL:
            break;
        default:
            break;
    }

    return error;
}

void USB_DeviceApplicationInit(uint8_t *IN_EP_BUFF, uint8_t *OUT_EP_BUFF, CB_usb_complete_fptr_t cb)
{
    USB_DeviceClockInit();

    // Store our USB cb
    usb_cb = cb;

    /* Set HID generic default state */
    g_UsbDeviceHidGeneric.speed        = USB_SPEED_FULL;
    g_UsbDeviceHidGeneric.attach       = 0U;
    g_UsbDeviceHidGeneric.deviceHandle = NULL;
    g_UsbDeviceHidGeneric.buffer[0]    = OUT_EP_BUFF;
    g_UsbDeviceHidGeneric.buffer[1]    = IN_EP_BUFF;

    /* Initialize the usb stack and class drivers */
    if (kStatus_USB_Success != USB_DeviceInit(CONTROLLER_ID, USB_DeviceCallback, &g_UsbDeviceHidGeneric.deviceHandle))
    {
        usb_echo("USB device generic failed\r\n");
        return;
    }
    else
    {
        usb_echo("USB device HID generic demo\r\n");
    }

    USB_DeviceIsrEnable();

    /* Start USB device HID generic */
    /*Add one delay here to make the DP pull down long enough to allow host to detect the previous disconnection.*/
    SDK_DelayAtLeastUs(5000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    USB_DeviceRun(g_UsbDeviceHidGeneric.deviceHandle);
}

void USB_DeviceReqRead(void) {
    USB_DeviceSendRequest(g_UsbDeviceHidGeneric.deviceHandle, USB_HID_GENERIC_INT_ENDPOINT_IN,
                               (uint8_t *)&g_UsbDeviceHidGeneric.buffer[1][0],
                               USB_HID_GENERIC_IN_BUFFER_LENGTH);
}

// static void BOARD_InitDebugConsole(void)
// {
//     /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
//     CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
//     RESET_ClearPeripheralReset(BOARD_DEBUG_UART_RST);
//     uint32_t uartClkSrcFreq = BOARD_DEBUG_UART_CLK_FREQ;
//     DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
// }

// void main(void)
// {
//     /* set BOD VBAT level to 1.65V */
//     POWER_SetBodVbatLevel(kPOWER_BodVbatLevel1650mv, kPOWER_BodHystLevel50mv, false);
//     /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
//     CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

//     BOARD_InitPins();
//     BOARD_BootClockPLL150M();
//     BOARD_InitDebugConsole();

//     NVIC_ClearPendingIRQ(USB1_IRQn);
//     NVIC_ClearPendingIRQ(USB1_NEEDCLK_IRQn);

//     POWER_DisablePD(kPDRUNCFG_PD_USB1_PHY); /*< Turn on USB1 Phy */

//     /* reset the IP to make sure it's in reset state. */
//     RESET_PeripheralReset(kUSB1H_RST_SHIFT_RSTn);
//     RESET_PeripheralReset(kUSB1D_RST_SHIFT_RSTn);
//     RESET_PeripheralReset(kUSB1_RST_SHIFT_RSTn);
//     RESET_PeripheralReset(kUSB1RAM_RST_SHIFT_RSTn);

//     CLOCK_EnableClock(kCLOCK_Usbh1);
//     /* Put PHY powerdown under software control */
//     *((uint32_t *)(USBHSH_BASE + 0x50)) = USBHSH_PORTMODE_SW_PDCOM_MASK;
//     /* According to reference mannual, device mode setting has to be set by access usb host register */
//     *((uint32_t *)(USBHSH_BASE + 0x50)) |= USBHSH_PORTMODE_DEV_ENABLE_MASK;
//     /* enable usb1 host clock */
//     // CLOCK_DisableClock(kCLOCK_Usbh1);

//     USB_DeviceApplicationInit();

//     while (1U)
//     {
//     }
// }
