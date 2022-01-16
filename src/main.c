/******************************************************************************
MIT License

Copyright (c) 2021 Glassboard

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "fsl_debug_console.h" // Needed for PRINTF
#include "board.h"
#include "pin_mux.h"
#include "adc.h"
#include "gpio.h"
#include "ui.h"
#include "hid_generic.h"
#include "usb_device_descriptor.h"

#define ADC_COUNT_OFFSET    13636
#define USB_INTERVAL        1

uint8_t int_rx_buffer[USB_HID_GENERIC_OUT_BUFFER_LENGTH] = {0x00};
uint8_t int_tx_buffer[USB_HID_GENERIC_IN_BUFFER_LENGTH] = {0x00};
uint8_t iso_tx_buffer[USB_HID_GENERIC_IN_BUFFER_LENGTH] = {0x00};
uint16_t adc_samples = 0x00;
uint16_t sample_index = 0x02;

uint32_t usb_ref_time = 0x00;

void adc_cb(uint16_t count);
void usb_cb(void);

int main(void)
{
    // Init the the board pins & clocks
    BOARD_Init();

    // Module init
    USB_DeviceApplicationInit(int_tx_buffer, int_rx_buffer, iso_tx_buffer, usb_cb);
    adc_init(adc_cb);
    ui_init();

    // Other init code
    PRINTF("Glasslabs - uMeasurement\n\r");

    adc_start_read();

    while (1)
    {
        // Run our UI update function
        ui_update();

        if( BOARD_GetTick() > (usb_ref_time + USB_INTERVAL)) {
            // Request a USB read of us.
            USB_DeviceReqRead();
            usb_ref_time = BOARD_GetTick();
        }
    }
}

void adc_cb(uint16_t count) {
    if( adc_samples >= ((USB_HID_GENERIC_IN_BUFFER_LENGTH / 2)-2)) {
        return;
    }

    // DATA & ADDRESS IS LOADED LSB FIRST
    memcpy(&int_tx_buffer[0], &adc_samples, 2);
    memcpy(&int_tx_buffer[sample_index++], &count, 2);
    adc_samples++;
    sample_index++;
    // tx_buffer[0] = (uint8_t)(adc_samples >> 8);
    // tx_buffer[1] = (uint8_t)(adc_samples);
    // tx_buffer[sample_index++] = (uint8_t)(count);
    // tx_buffer[sample_index++] = (uint8_t)(count >> 8);
}

void usb_cb(void) {
    memset(int_tx_buffer, 0x00, USB_HID_GENERIC_IN_BUFFER_LENGTH);
    adc_samples = 0x00;
    sample_index = 0x02;
}
