/**
 * @file		FIR_funct.h
 * @brief		
 * @authors		thanasis
 * @copyright	Delft University of Technology
 * @date 		2013
 * @version
 */

#ifndef FIR_FUNCT_H_
#define FIR_FUNCT_H_

#include "FIR.h"

__inline__ void fir_simple_wav(int16_t * const sum, const int16_t *Ain, const int16_t *COEFF_re,
		const int16_t *COEFF_im, const int startIndex ) {
	int k;
	int32_t sum_partRE = 0;
	int32_t sum_partIM = 0;
	int32_t sq1 = 0;
	int32_t sq2 = 0;
	int32_t sum_inter;
	float sumf;

	for (k = 0; k < NR_SAMPLES; k++) {
		/** Imaginary Part*/
		sum_partIM += COEFF_im[(startIndex+k)%NR_SAMPLES] * Ain[(startIndex+k)%NR_SAMPLES];
		/** Real Part*/
		sum_partRE += COEFF_re[(startIndex+k)%NR_SAMPLES] * Ain[(startIndex+k)%NR_SAMPLES];
	}

	/** Absolute value of Complex*/
	sq1 = sum_partRE * sum_partRE;
	sq2 = sum_partIM * sum_partIM;
	sum_inter = sq2 + sq1;


	sumf = (float) sum_inter;
	sumf = sqrtf(sumf);
	*sum = (int16_t) sumf;

}

#endif /* FIR_FUNCT_H_ */
