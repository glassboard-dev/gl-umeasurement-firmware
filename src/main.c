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

#define ADC_READ_RATE_uS    50
#define ADC_REPORT_RATE_uS  250000
#define ADC_BUFF_SIZE       30000

#define ADC_COUNT_OFFSET    13636

uint32_t adc_read_refTime = 0x0000;
uint32_t adc_report_refTime = 0x0000;
uint16_t adc_count = 0;
uint16_t adc_samples[ADC_BUFF_SIZE] = {0};
uint16_t max_reported = 0;
bool adc_reporting = false;
char adc_report[5120] = {0};
char tmpLine[32] = {0};

void adc_cb(uint16_t count);

static void reportADC_samples(void) {
    adc_reporting = true;

    if( adc_count > max_reported)
        max_reported = adc_count;

    memset(adc_report, 0x00, sizeof(adc_report));
    // sprintf(adc_report, "%d samples collected in %dms - Max: %d\n\r", adc_count, ADC_REPORT_RATE_uS/1000, max_reported);
    sprintf(adc_report, "ADC: %d\n\r", (adc_samples[0]) - ADC_COUNT_OFFSET);
    PRINTF("%s", adc_report);

    adc_count = 0;

    adc_reporting = false;
}

int main(void)
{
    // Init the the board pins & clocks
    BOARD_Init();

    // Module init
    adc_init(adc_cb);
    ui_init();

    PRINTF("Glasslabs - uMeasurement\n\r");

    while (1)
    {
        // Run our UI update function
        ui_update();

        // Start an async read of the ADC
        if( (adc_read_refTime + ADC_READ_RATE_uS) < BOARD_GetTick() ) {
            // if( !adc_reporting )
                adc_read();
            adc_read_refTime = BOARD_GetTick();
        }

        // Start an async read of the ADC
        if( (adc_report_refTime + ADC_REPORT_RATE_uS) < BOARD_GetTick() ) {
            reportADC_samples();
            adc_report_refTime = BOARD_GetTick();
        }
    }
}

void adc_cb(uint16_t count) {
    // if( adc_reporting )
    //     return;

    if( adc_count >= ADC_BUFF_SIZE)
        return;

    adc_samples[adc_count] = count;
    adc_count++;
    // PRINTF("New adc measurement!: %d\n\r", count);
}
