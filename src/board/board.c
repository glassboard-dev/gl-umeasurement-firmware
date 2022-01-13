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

#include <stdint.h>
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_inputmux.h"
#include "fsl_power.h"
#include "board.h"
#include "pin_mux.h"
#include "gpio.h"

static void BOARD_InitDebugConsole(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_systickCounter;
volatile uint32_t g_delayCounter;

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Initialize debug console. */
static void BOARD_InitDebugConsole(void)
{
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    RESET_ClearPeripheralReset(BOARD_DEBUG_UART_RST);
    uint32_t uartClkSrcFreq = BOARD_DEBUG_UART_CLK_FREQ;
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}

void SysTick_Handler(void)
{
    if (g_delayCounter != 0U)
    {
        g_delayCounter--;
    }

    g_systickCounter++;
}

void BOARD_Init(void) {
    // Power Init
    POWER_SetBodVbatLevel(kPOWER_BodVbatLevel1650mv, kPOWER_BodHystLevel50mv, false);

    // Board pin init
    BOARD_InitPins();
    // Init the Debug console on UART0
    BOARD_InitDebugConsole();

    // Power Init
    POWER_DisablePD(kPDRUNCFG_PD_LDOGPADC);
    POWER_DisablePD(kPDRUNCFG_PD_USB1_PHY);

    NVIC_ClearPendingIRQ(USB1_IRQn);
    NVIC_ClearPendingIRQ(USB1_NEEDCLK_IRQn);

    /* reset the IP to make sure it's in reset state. */
    RESET_PeripheralReset(kUSB1H_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB1D_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB1_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB1RAM_RST_SHIFT_RSTn);

    CLOCK_EnableClock(kCLOCK_Usbh1);
    /* Put PHY powerdown under software control */
    *((uint32_t *)(USBHSH_BASE + 0x50)) = USBHSH_PORTMODE_SW_PDCOM_MASK;
    /* According to reference mannual, device mode setting has to be set by access usb host register */
    *((uint32_t *)(USBHSH_BASE + 0x50)) |= USBHSH_PORTMODE_DEV_ENABLE_MASK;

    // Clock configuration
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    CLOCK_AttachClk(kMAIN_CLK_to_ADC_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivAdcAsyncClk, 8U, true);

    // Set our oscillator to 150Mhz
    BOARD_BootClockPLL150M();
    // Set our SysTick frequency
    SysTick_Config(SystemCoreClock / 1000U);

    // Set the initial LED states
    gpio_setStatusLED(false);
    gpio_setPowerLED(false);
    gpio_setCommLED(false);

    // Set the initial Range select states
    gpio_setMicroAmpEn(false);
    gpio_setNanoAmpEn(false);
    gpio_setMilliAmpEn(true);
}

void BOARD_DelayTicks(uint32_t n)
{
    g_delayCounter = n;
    while (g_delayCounter != 0U)
    {
    }
}

uint32_t BOARD_GetTick(void) {
    return g_systickCounter;
}