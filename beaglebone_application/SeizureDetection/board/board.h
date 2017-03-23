/**
 * @file		board.h
 * @brief		
 * @authors		thanasis
 * @copyright	Delft University of Technology
 * @date 		2013
 * @version
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "PinDefs.h"
#include <inttypes.h>

void boardInit(void);


#define TIMER_1S_COUNT					24000000
#define TIMER_1MS_COUNT					24000
#define TIMER_1US_COUNT 				24
#define TIMER_OVERFLOW                 (0xFFFFFFFFu)

static const uint8_t chan2pin[]={ CHAN0_PIN,
					 CHAN1_PIN,
					 CHAN2_PIN,
					 CHAN3_PIN
};

void pinWrite(uint32_t pinNumber, uint32_t pinBank, uint8_t value);
void pinMode(uint32_t pinNumber, uint32_t pinBank, uint8_t mode) ;
void EnableTimerSEIZ(int millisDelay, int millisOFF, int milliLOCK,  const uint8_t *lockSeiz);

#endif /* BOARD_H_ */
