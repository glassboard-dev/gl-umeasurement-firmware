#include "fsl_debug_console.h" // Needed for PRINTF
#include "board.h"
#include "pin_mux.h"
#include "adc.h"

int main(void)
{
    uint32_t whole, dec = 0;
    float result = 0.0;
    // Init the the board pins & clocks
    BOARD_Init();
    // Init the ADC module
    adc_init();

    PRINTF("Glasslabs - uMeasurement!\n\r");

    while (1)
    {
        // Do a synchronous read of the ADC
        result = adc_read_sync();
        whole = (uint32_t)result;
        dec = (result * 100) - (whole * 100);

        // Print the result
        PRINTF("ADC value: %01d.%02d\r\n", whole, dec);

        // Toggle the LED to show we are running
        GPIO_PortToggle(GPIO, BOARD_LED_RED_GPIO_PORT, 1u << BOARD_LED_RED_GPIO_PIN);

        // Delay 500ms
        BOARD_DelayTicks(500U);
    }
}
