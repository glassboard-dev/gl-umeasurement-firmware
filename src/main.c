#include <stdio.h>
#include <string.h>
#include "fsl_debug_console.h" // Needed for PRINTF
#include "board.h"
#include "pin_mux.h"
#include "adc.h"

#define LED_UPDATE_RATE_uS  500000
#define ADC_READ_RATE_uS    50
#define ADC_REPORT_RATE_uS  250000
#define ADC_BUFF_SIZE       30000

uint32_t led_refTime = 0x0000;
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

    // memset(adc_report, 0x00, sizeof(adc_report));
    // sprintf(adc_report, "%d samples collected in %dms - Max: %d\n\r", adc_count, ADC_REPORT_RATE_uS/1000, max_reported);
    // // PRINTF("\033[2J");
    // PRINTF("%s", adc_report);

    adc_count = 0;

    adc_reporting = false;
}

int main(void)
{
    // Init the the board pins & clocks
    BOARD_Init();
    // Init the ADC module
    adc_init(adc_cb);

    PRINTF("Glasslabs - uMeasurement\n\r");

    led_refTime = BOARD_GetTick();

    while (1)
    {
        // Start an async read of the ADC
        if( (adc_read_refTime + ADC_READ_RATE_uS) < BOARD_GetTick() ) {
            // Toggle the LED to show we are running
            // if( !adc_reporting )
                adc_read();
            adc_read_refTime = BOARD_GetTick();
        }

        // Start an async read of the ADC
        if( (adc_report_refTime + ADC_REPORT_RATE_uS) < BOARD_GetTick() ) {
            // Toggle the LED to show we are running
            reportADC_samples();
            adc_report_refTime = BOARD_GetTick();
        }

        if( (led_refTime + LED_UPDATE_RATE_uS) < BOARD_GetTick() ) {
            // Toggle the LED to show we are running
            GPIO_PortToggle(GPIO, BOARD_LED_RED_GPIO_PORT, 1u << BOARD_LED_RED_GPIO_PIN);
            led_refTime = BOARD_GetTick();
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
