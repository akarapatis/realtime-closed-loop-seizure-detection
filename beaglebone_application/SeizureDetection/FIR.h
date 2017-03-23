/**
 * @file		FIR.h
 * @brief		
 * @authors		thanasis
 * @copyright	Delft University of Technology
 * @date 		2013
 * @version
 */

#ifndef FIR_H_
#define FIR_H_

#include <inttypes.h>
#include <math.h>

/* Set input sampling period  */
#define SAMPLING_PERIOD_MS		10

/* Number of samples */
#define NR_SAMPLES				64


/* Coefficient sets */
#define COEFF_SETS				4
#define COEFFS0					0
#define COEFFS1					1
#define COEFFS2					2
#define COEFFS3					3


/* Channels masks */
#define CHAN_NONE				0x0
#define CHAN_SEL0				0x1
#define CHAN_SEL1				0x2
#define CHAN_SEL2				0x4
#define CHAN_SEL3  				0x8
#define CHAN_ALL				0xF

#define CHANNEL_0				0
#define CHANNEL_1				1
#define CHANNEL_2				2
#define CHANNEL_3  				3


/* Threshold Defaults  */
#define THRES_U_DEFAULT			25000000
#define THRES_L_DEFAULT			 1000000


void resetArrays(void);
void fir_simple_wav(int32_t * const sum, const int16_t *Ain, const int16_t *COEFF_re,
					  const int16_t *COEFF_im, const int startIndex ) ;



#endif /* FIR_H_ */
