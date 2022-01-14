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

#define ADC_COUNT_OFFSET    13636

uint8_t rx_buffer[1024] = {0x00};
uint8_t tx_buffer[1024] = {0x00};

void adc_cb(uint16_t count);

int main(void)
{
    // Init the the board pins & clocks
    BOARD_Init();

    // Module init
    USB_DeviceApplicationInit(tx_buffer, rx_buffer);
    adc_init(adc_cb);
    ui_init();

    // Other init code
    PRINTF("Glasslabs - uMeasurement\n\r");

    adc_start_read();

    while (1)
    {
        // Run our UI update function
        ui_update();
    }
}

void adc_cb(uint16_t count) {
    tx_buffer[0] = (uint8_t)(count);
    tx_buffer[1] = (uint8_t)(count >> 8);
}
