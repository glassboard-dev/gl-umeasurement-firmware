#include "board.h"
#include "fsl_lpadc.h"

#define LPADC_USER_CMDID            1U /* CMD1 */
#define LPADC_DO_OFFSET_CALIBRATION false
#define LPADC_OFFSET_VALUE_A        0x10U
#define LPADC_OFFSET_VALUE_B        0x10U
#define ADC_LSB_VAL                 (3.3/65536)

const uint32_t g_LpadcFullRange   = 65536U;
const uint32_t g_LpadcResultShift = 0U;

static lpadc_config_t mLpadcConfigStruct;
static lpadc_conv_command_config_t mLpadcCommandConfigStruct;
static lpadc_conv_trigger_config_t mLpadcTriggerConfigStruct;
static lpadc_conv_result_t mLpadcResultConfigStruct;

void adc_init(void) {

    // Get the default ADC configuration
    LPADC_GetDefaultConfig(&mLpadcConfigStruct);
    // Enable prelim ADC readiness at the cost of current draw
    mLpadcConfigStruct.enableAnalogPreliminary = true;
    // Set our reference to VDDA - 3.3V
    mLpadcConfigStruct.referenceVoltageSource = kLPADC_ReferenceVoltageAlt2;
    // Average 128 samples in software before reporting a result
    mLpadcConfigStruct.conversionAverageMode = kLPADC_ConversionAverage128;
    // Apply the new configuration for the LPADC module
    LPADC_Init(ADC0, &mLpadcConfigStruct);

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

    // Get the default ADC Conversion configuration
    LPADC_GetDefaultConvCommandConfig(&mLpadcCommandConfigStruct);
    // Set the conversion to be differential
    mLpadcCommandConfigStruct.sampleChannelMode = kLPADC_SampleChannelDiffBothSide;
    // Set the ADC channel to 0
    mLpadcCommandConfigStruct.channelNumber = 0U;
    // Set the conversion resolution to high (16bit).. Effectively 15bit since the result is signed
    mLpadcCommandConfigStruct.conversionResolutionMode = kLPADC_ConversionResolutionHigh;
    // Apply the new configuration for the ADC Conversions
    LPADC_SetConvCommandConfig(ADC0, LPADC_USER_CMDID, &mLpadcCommandConfigStruct);


    // Get the default ADC conversion trigger configuration
    LPADC_GetDefaultConvTriggerConfig(&mLpadcTriggerConfigStruct);
    mLpadcTriggerConfigStruct.targetCommandId = LPADC_USER_CMDID;
    // Disable any hardware triggers of the ADC
    mLpadcTriggerConfigStruct.enableHardwareTrigger = false;
    // Apply the new conversion trigger configuration
    LPADC_SetConvTriggerConfig(ADC0, 0U, &mLpadcTriggerConfigStruct);
}

float adc_read_sync(void) {

    float answer = 0;

     // Trigger an ADC read
    LPADC_DoSoftwareTrigger(ADC0, 1U); /* 1U is trigger0 mask. */


    while (!LPADC_GetConvResult(ADC0, &mLpadcResultConfigStruct, 0U))
    {
    }

    // Calculate the answer
    answer = ((mLpadcResultConfigStruct.convValue) >> g_LpadcResultShift) * ADC_LSB_VAL;

    return answer;
}