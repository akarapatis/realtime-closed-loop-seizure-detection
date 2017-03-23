/**
 * @file		adc.h
 * @brief		
 * @authors		thanasis
 * @copyright	Delft University of Technology
 * @date 		2013
 * @version
 */

#ifndef ADC_H_
#define ADC_H_


#define AVAILABLE 		0
#define NOT_AVAIL		1

#define NR_CHANNELS		4

void SetupIntc(void);
void ADCConfigure(void);

void DisableTimerADC();
void EnableTimerADC();

#endif /* ADC_H_ */
