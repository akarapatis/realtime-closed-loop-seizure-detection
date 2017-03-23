/**
 * @file		FIR.c
 * @brief		
 * @authors		thanasis
 * @copyright	Delft University of Technology
 * @date 		2013
 * @version
 */


#include <math.h>
#include <arm_neon.h>

#include "FIR.h"
#include "board/adc.h"
#include "board/SPI.h"
#include "board/PinDefs.h"
#include "board/board.h"

#include <consoleUtils.h>

/******************************************************************************
 **                      EXTERNAL VARIABLE DEFINITIONS
 *******************************************************************************/
extern int16_t AIN[ NR_CHANNELS ][NR_SAMPLES];
extern int16_t coeffRE[NR_SAMPLES];
extern int16_t coeffIM[NR_SAMPLES];

//int16_t valueRE[64] = { -60, -33, 13, 75, 140, 195, 222, 207, 141, 28, -116,
//		-266, -388, -452, -434, -328, -148, 76, 299, 476, 569, 559, 447, 258,
//		31, -187, -355, -444, -445, -368, -238, -87, 53, 158, 214, 220, 186,
//		128, 62, 3, -40, -63, -67, -58, -41, -23, -7, 5, 11, 13, 11, 8, 5, 2, 0,
//		-1, -1, -1, -1, -1, 0, 0, 0, 0 };
//int16_t valueIM[64] = { -66, -101, -126, -128, -101, -40, 50, 154, 253, 321,
//		336, 283, 162, -11, -207, -386, -509, -543, -477, -319, -97, 143, 356,
//		498, 544, 491, 355, 169, -25, -191, -299, -338, -311, -235, -134, -31,
//		54, 109, 130, 122, 95, 59, 23, -5, -23, -31, -30, -24, -15, -7, -1, 3,
//		4, 4, 4, 2, 1, 0, 0, 0, 0, 0, 0, 0 };
//

int16_t valueRE[64]={
	   -4,   -2,    1,    5,    9,   12,   14,   13,    9,    2,
	   -7,  -17,  -24,  -28,  -27,  -20,   -9,    5,   19,   30,
	   36,   35,   28,   16,    2,  -12,  -22,  -28,  -28,  -23,
	  -15,   -5,    3,   10,   13,   14,   12,    8,    4,    0,
	   -2,   -4,   -4,   -4,   -3,   -1,    0,    0,    1,    1,
	    1,    1,    0,    0,    0,    0,    0,    0,    0,    0,
	    0,    0,    0,    0
};
int16_t valueIM[64]={
	   -4,   -6,   -8,   -8,   -6,   -2,    3,   10,   16,   20,
	   21,   18,   10,   -1,  -13,  -24,  -32,  -34,  -30,  -20,
	   -6,    9,   22,   31,   34,   31,   22,   11,   -2,  -12,
	  -19,  -21,  -19,  -15,   -8,   -2,    3,    7,    8,    8,
	    6,    4,    1,    0,   -1,   -2,   -2,   -1,   -1,    0,
	    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	    0,    0,    0,    0
};

/******************************************************************************
 **                      INTERNAL VARIABLE DEFINITIONS
 *******************************************************************************/
/* Coefficient sel */
//static uint8_t	coeffSel   = COEFFS0;


/* TODO: COEFFICIENTS initialization */
void resetArrays(void){
	int smpl_itr=0;
	int chan=0;

	for (chan = 0; chan < NR_CHANNELS; ++chan) {
		for(smpl_itr=0; smpl_itr<NR_SAMPLES; smpl_itr++){
				AIN[chan][smpl_itr] = 0;
			}
	}

	for(smpl_itr=0; smpl_itr<NR_SAMPLES; smpl_itr++){

		coeffRE[smpl_itr] = valueRE[smpl_itr];
		coeffIM[smpl_itr] = valueIM[smpl_itr];
	}

}


void fir_simple_wav(int32_t * const sum, const int16_t *Ain, const int16_t *COEFF_re,
		const int16_t *COEFF_im, const int startIndex ) {
	int k;
	int32_t sum_partRE = 0;
	int32_t sum_partIM = 0;
	int32_t sq1 = 0;
	int32_t sq2 = 0;
//	float sumf=0.0;
//	int32_t sum_inter=0;
	int sample_idx=0;


	*sum = 0;
	sample_idx = startIndex;

	for (k = 0; k < NR_SAMPLES; k++ ) {
		if( sample_idx == -1){
			sample_idx = NR_SAMPLES-1;
		}

		/** Imaginary Part*/
		sum_partIM += COEFF_im[k] * Ain[sample_idx];
		/** Real Part*/
		sum_partRE += COEFF_re[k] * Ain[sample_idx];

//		ConsoleUtilsPrintf("COEFF_re:%4d sum_partRE:%4d Ain:%4d k:%d\n", COEFF_re[k] , sum_partRE, Ain[sample_idx], k );

		sample_idx--;
	}


	/** Absolute value of Complex*/
	sq1 = sum_partRE * sum_partRE;
	if(sq1 < 0){
		pinWrite(OVF_PIN, PINS_1, LOW);
	}

	sq2 = sum_partIM * sum_partIM;
	if(sq2 < 0){
			pinWrite(OVF_PIN, PINS_1, LOW);
	}

	*sum  = sq2 + sq1;

//	sum_inter = *sum;

//	sumf = sqrtf( (float)sum_inter );
//	sum_inter = sumf ;
//	setDACA((uint16_t)sum_inter);


}


void firSIMD_wav(int32_t * const sum_ptr, const int16_t *x, const int16_t *h_re,
		const int16_t *h_im) {

	int k;
	int16x4_t h_RE_vec;
	int16x4_t h_IM_vec;
	int16x4_t x_vec;
	int32x4_t result_vec_RE;
	int32x4_t result_vec_IM;

	int32_t sq1 = 0;
	int32_t sq2 = 0;
	int32_t sum_partRE = 0;
	int32_t sum_partIM = 0;
	int32_t sum_inter;
//	float sumf;

	/* Clear the scalar and vector sums */
	*sum_ptr = 0;
	result_vec_RE = vdupq_n_s32(0);
	result_vec_IM = vdupq_n_s32(0);

	for (k = 0; k < NR_SAMPLES >> 2; k++) {
		/* Four vector multiply-accumulate operations in parallel */
		h_RE_vec = vld1_s16(&h_re[k << 2]);
		h_IM_vec = vld1_s16(&h_im[k << 2]);
		x_vec = vld1_s16(&x[k << 2]);

		result_vec_RE = vmlal_s16(result_vec_RE, h_RE_vec, x_vec);
		result_vec_IM = vmlal_s16(result_vec_IM, h_IM_vec, x_vec);
	}
	/* Reduction operation add each vector lane result to the sum */
	sum_partRE += vgetq_lane_s32(result_vec_RE, 0);
	sum_partRE += vgetq_lane_s32(result_vec_RE, 1);
	sum_partRE += vgetq_lane_s32(result_vec_RE, 2);
	sum_partRE += vgetq_lane_s32(result_vec_RE, 3);

	sum_partIM += vgetq_lane_s32(result_vec_IM, 0);
	sum_partIM += vgetq_lane_s32(result_vec_IM, 1);
	sum_partIM += vgetq_lane_s32(result_vec_IM, 2);
	sum_partIM += vgetq_lane_s32(result_vec_IM, 3);

	/** Absolute value of Complex*/
	sq1 = sum_partRE * sum_partRE;
	sq2 = sum_partIM * sum_partIM;
	sum_inter = sq2 + sq1;

	*sum_ptr = sum_inter;

//	sumf = (float) sum_inter;
//	sumf = sqrtf(sumf);
//	sum_inter = sumf;
//	*sum_ptr = (int16_t) sum_inter;

}
