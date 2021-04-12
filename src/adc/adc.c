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

#include "fsl_lpadc.h"
#include "board.h"
#include "adc.h"

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
static bool adc_conv_complete = true;
static CB_adc_complete_fptr_t adc_cb = NULL;

// static void calibrate(void);

void ADC0_IRQHandler(void) {
    adc_conv_complete = true;

    if( adc_cb != NULL ) {
        if (LPADC_GetConvResult(ADC0, &mLpadcResultConfigStruct, 0U)) {
            adc_cb(mLpadcResultConfigStruct.convValue >> g_LpadcResultShift);
        }
    }
}

// static void calibrate(void) {
//     #if defined(FSL_FEATURE_LPADC_HAS_CTRL_CALOFS) && FSL_FEATURE_LPADC_HAS_CTRL_CALOFS
//     #if defined(FSL_FEATURE_LPADC_HAS_OFSTRIM) && FSL_FEATURE_LPADC_HAS_OFSTRIM
//         /* Request offset calibration. */
//     #if defined(DEMO_LPADC_DO_OFFSET_CALIBRATION) && DEMO_LPADC_DO_OFFSET_CALIBRATION
//         LPADC_DoOffsetCalibration(ADC0);
//     #else
//         LPADC_SetOffsetValue(ADC0, DEMO_LPADC_OFFSET_VALUE_A, DEMO_LPADC_OFFSET_VALUE_B);
//     #endif /* DEMO_LPADC_DO_OFFSET_CALIBRATION */
//     #endif /* FSL_FEATURE_LPADC_HAS_OFSTRIM */
//         /* Request gain calibration. */
//         LPADC_DoAutoCalibration(ADC0);
//     #endif /* FSL_FEATURE_LPADC_HAS_CTRL_CALOFS */

//     #if (defined(FSL_FEATURE_LPADC_HAS_CFG_CALOFS) && FSL_FEATURE_LPADC_HAS_CFG_CALOFS)
//         /* Do auto calibration. */
//         LPADC_DoAutoCalibration(ADC0);
//     #endif /* FSL_FEATURE_LPADC_HAS_CFG_CALOFS */
// }

void adc_init(CB_adc_complete_fptr_t cb) {

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

    // Calibrate the ADC
    // calibrate();

    // Get the default ADC Conversion configuration
    LPADC_GetDefaultConvCommandConfig(&mLpadcCommandConfigStruct);
    // Set the conversion to be differential
    mLpadcCommandConfigStruct.sampleChannelMode = kLPADC_SampleChannelDiffBothSide;
    // Set the ADC channel to 0
    mLpadcCommandConfigStruct.channelNumber = 0U;
    // Set the conversion resolution to high (16bit).. Effectively 15bit since the result is signed
    mLpadcCommandConfigStruct.conversionResolutionMode = kLPADC_ConversionResolutionHigh;
    // Set the sample time to 3 cycles
    mLpadcCommandConfigStruct.sampleTimeMode = kLPADC_SampleTimeADCK3;
    // Apply the new configuration for the ADC Conversions
    LPADC_SetConvCommandConfig(ADC0, LPADC_USER_CMDID, &mLpadcCommandConfigStruct);


    // Get the default ADC conversion trigger configuration
    LPADC_GetDefaultConvTriggerConfig(&mLpadcTriggerConfigStruct);
    mLpadcTriggerConfigStruct.targetCommandId = LPADC_USER_CMDID;
    // Disable any hardware triggers of the ADC
    mLpadcTriggerConfigStruct.enableHardwareTrigger = false;
    // Apply the new conversion trigger configuration
    LPADC_SetConvTriggerConfig(ADC0, 0U, &mLpadcTriggerConfigStruct);

    // Enable the ADC interrupt
    LPADC_EnableInterrupts(ADC0, kLPADC_FIFO0WatermarkInterruptEnable);
    EnableIRQ(ADC0_IRQn);

    // Store the callback that we should call when a conversion completes
    adc_cb = cb;
}

void adc_read(void) {
    // Check our flag to see if a conversion is in progress
    if( adc_conv_complete ) {
        adc_conv_complete = false;
        LPADC_DoSoftwareTrigger(ADC0, 1U); /* 1U is trigger0 mask. */
    }
}