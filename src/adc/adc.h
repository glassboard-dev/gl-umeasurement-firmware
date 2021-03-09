#ifndef _ADC_H_
#define _ADC_H_

typedef void(*CB_adc_complete_fptr_t)(uint16_t adc_count);

void adc_init(CB_adc_complete_fptr_t cb);
float adc_read_sync(void);
void adc_read(void);

#endif // _ADC_H_