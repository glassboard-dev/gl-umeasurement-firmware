#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_lpadc.h"

#include "fsl_inputmux.h"
#include "fsl_power.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_LED_PORT BOARD_LED_BLUE_GPIO_PORT
#define BOARD_LED_PIN  BOARD_LED_BLUE_GPIO_PIN

#define LPADC_BASE                  ADC0
#define LPADC_USER_CHANNEL          0U
#define LPADC_USER_CMDID            1U /* CMD1 */
#define LPADC_VREF_SOURCE           kLPADC_ReferenceVoltageAlt2
#define LPADC_DO_OFFSET_CALIBRATION false
#define LPADC_OFFSET_VALUE_A        0x10U
#define LPADC_OFFSET_VALUE_B        0x10U
#define LPADC_USE_HIGH_RESOLUTION   true
#define ADC_LSB_VAL                 (3.3/65536)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_systickCounter;
const uint32_t g_LpadcFullRange   = 65536U;
const uint32_t g_LpadcResultShift = 0U;

/*******************************************************************************
 * Code
 ******************************************************************************/
void SysTick_Handler(void)
{
    if (g_systickCounter != 0U)
    {
        g_systickCounter--;
    }
}

void SysTick_DelayTicks(uint32_t n)
{
    g_systickCounter = n;
    while (g_systickCounter != 0U)
    {
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    float answer = 0;
    uint32_t whole = 0;
    uint32_t dec = 0;
    lpadc_config_t mLpadcConfigStruct;
    lpadc_conv_trigger_config_t mLpadcTriggerConfigStruct;
    lpadc_conv_command_config_t mLpadcCommandConfigStruct;
    lpadc_conv_result_t mLpadcResultConfigStruct;

    GPIO_PortInit(GPIO, BOARD_LED_PORT);

    // Power Init
    POWER_SetBodVbatLevel(kPOWER_BodVbatLevel1650mv, kPOWER_BodHystLevel50mv, false);

    // Board pin init
    BOARD_InitPins();
    BOARD_InitDebugConsole();

    // Clock configuration
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    CLOCK_AttachClk(kMAIN_CLK_to_ADC_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivAdcAsyncClk, 8U, true);
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000U);

    // Power Init
    POWER_DisablePD(kPDRUNCFG_PD_LDOGPADC);

    // LPADC Configuration
    LPADC_GetDefaultConfig(&mLpadcConfigStruct);

    mLpadcConfigStruct.enableAnalogPreliminary = true;
    mLpadcConfigStruct.referenceVoltageSource = LPADC_VREF_SOURCE;
    mLpadcConfigStruct.conversionAverageMode = kLPADC_ConversionAverage128;

    LPADC_Init(LPADC_BASE, &mLpadcConfigStruct);

// #if defined(FSL_FEATURE_LPADC_HAS_CTRL_CALOFS) && FSL_FEATURE_LPADC_HAS_CTRL_CALOFS
// #if defined(FSL_FEATURE_LPADC_HAS_OFSTRIM) && FSL_FEATURE_LPADC_HAS_OFSTRIM
//     /* Request offset calibration. */
// #if defined(DEMO_LPADC_DO_OFFSET_CALIBRATION) && DEMO_LPADC_DO_OFFSET_CALIBRATION
//     LPADC_DoOffsetCalibration(DEMO_LPADC_BASE);
// #else
//     LPADC_SetOffsetValue(DEMO_LPADC_BASE, DEMO_LPADC_OFFSET_VALUE_A, DEMO_LPADC_OFFSET_VALUE_B);
// #endif /* DEMO_LPADC_DO_OFFSET_CALIBRATION */
// #endif /* FSL_FEATURE_LPADC_HAS_OFSTRIM */
//     /* Request gain calibration. */
//     LPADC_DoAutoCalibration(DEMO_LPADC_BASE);
// #endif /* FSL_FEATURE_LPADC_HAS_CTRL_CALOFS */

// #if (defined(FSL_FEATURE_LPADC_HAS_CFG_CALOFS) && FSL_FEATURE_LPADC_HAS_CFG_CALOFS)
//     /* Do auto calibration. */
//     LPADC_DoAutoCalibration(DEMO_LPADC_BASE);
// #endif /* FSL_FEATURE_LPADC_HAS_CFG_CALOFS */

    /* Set conversion CMD configuration. */
    LPADC_GetDefaultConvCommandConfig(&mLpadcCommandConfigStruct);
    mLpadcCommandConfigStruct.channelNumber = LPADC_USER_CHANNEL;
    mLpadcCommandConfigStruct.conversionResolutionMode = kLPADC_ConversionResolutionHigh;
    LPADC_SetConvCommandConfig(LPADC_BASE, LPADC_USER_CMDID, &mLpadcCommandConfigStruct);

    /* Set trigger configuration. */
    LPADC_GetDefaultConvTriggerConfig(&mLpadcTriggerConfigStruct);
    mLpadcTriggerConfigStruct.targetCommandId = LPADC_USER_CMDID;
    mLpadcTriggerConfigStruct.enableHardwareTrigger = false;
    LPADC_SetConvTriggerConfig(LPADC_BASE, 0U, &mLpadcTriggerConfigStruct); /* Configurate the trigger0. */

#if defined(FSL_FEATURE_LPADC_HAS_CMDL_CSCALE) && FSL_FEATURE_LPADC_HAS_CMDL_CSCALE
    if (kLPADC_SampleFullScale == mLpadcCommandConfigStruct.sampleScaleMode)
    {
        PRINTF("Full channel scale (Factor of 1).\r\n");
    }
    else if (kLPADC_SamplePartScale == mLpadcCommandConfigStruct.sampleScaleMode)
    {
        PRINTF("Divided input voltage signal. (Factor of 30/64).\r\n");
    }
#endif


    PRINTF("Glasslabs - uMeasurement!\n\r");
    PRINTF("ADC Full Range: %d\r\n", g_LpadcFullRange);
    PRINTF("Please press any key to get user channel's ADC value.\r\n");

    while (1)
    {
        // Trigger an ADC read
        LPADC_DoSoftwareTrigger(LPADC_BASE, 1U); /* 1U is trigger0 mask. */

        // Wait for the conversion to complete
#if (defined(FSL_FEATURE_LPADC_FIFO_COUNT) && (FSL_FEATURE_LPADC_FIFO_COUNT == 2U))
        while (!LPADC_GetConvResult(LPADC_BASE, &mLpadcResultConfigStruct, 0U))
#else
        while (!LPADC_GetConvResult(LPADC_BASE, &mLpadcResultConfigStruct))
#endif /* FSL_FEATURE_LPADC_FIFO_COUNT */
        {
        }

        // Calculate the answer
        answer = ((mLpadcResultConfigStruct.convValue) >> g_LpadcResultShift) * ADC_LSB_VAL;
        whole = (uint32_t)answer;
        dec = (answer * 100) - (whole * 100);

        // Print the result
        PRINTF("ADC value: %01d.%02d\r\n", whole, dec);

        // Toggle the LED to show we are running
        GPIO_PortToggle(GPIO, BOARD_LED_PORT, 1u << BOARD_LED_PIN);

        // Delay 500ms
        SysTick_DelayTicks(500U);
    }
}
