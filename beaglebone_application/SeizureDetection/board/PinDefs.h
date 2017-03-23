/**
 * @file	PinDefs.h
 * @brief	Pins Definition file
 *
 * @todo 	Should be moved to platform as BoardPINS
 * 			Should
 * @note
 * From SinsV2.1 pin 15 is ENABLESTIM instead of pin 19
 * #define ERROR_PIN 19 // CHECK!! FIXME // version old...
 *
 */

#ifndef PinDefs_h
#define PinDefs_h

#include "soc_AM335x.h"
#include "gpio_v2.h"
#include <inttypes.h>


#define LOW GPIO_PIN_LOW
#define HIGH GPIO_PIN_HIGH


#define INPUT GPIO_DIR_INPUT
#define OUTPUT GPIO_DIR_OUTPUT

static uint32_t pin2reg [] ={
		0x00000001,   // register 0
		0x00000002,   // register 1
		0x00000004,   // register 2
		0x00000008,   // register 3
		0x00000010,   // register 4
		0x00000020,   // register 5
		0x00000040,   // register 6
		0x00000080,   // register 7
		0x00000100,   // register 8
		0x00000200,   // register 9
		0x00000400,   // register 10
		0x00000800,   // register 11
		0x00001000,   // register 12
		0x00002000,   // register 13
		0x00004000,   // register 14
		0x00008000,   // register 15
		0x00010000,   // register 16
		0x00020000,   // register 17
		0x00040000,   // register 18
		0x00080000,   // register 19
		0x00100000,   // register 20
		0x00200000,   // register 21
		0x00400000,   // register 22
		0x00800000,   // register 23
		0x01000000,   // register 24
		0x02000000,   // register 25
		0x04000000,   // register 26
		0x08000000,   // register 27
		0x10000000,   // register 28
		0x20000000,   // register 29
		0x40000000,   // register 30
		0x80000000,   // register 31
};


#define PINSET(pin, val)   ((val)==1)?(0xFFFFFFFF&(pin2reg[pin])): 0


#define GPIO1_0  0
#define GPIO1_1  1
#define GPIO1_2  2
#define GPIO1_3  3
#define GPIO1_4  4
#define GPIO1_5  5
#define GPIO1_6  6
#define GPIO1_7  7
#define GPIO1_8  8
#define GPIO1_9  9
#define GPIO1_10  10
#define GPIO1_11  11
#define GPIO1_12  12
#define GPIO1_13  13
#define GPIO1_14  14
#define GPIO1_15  15
#define GPIO1_16  16
#define GPIO1_17  17
#define GPIO1_18  18
#define GPIO1_19  19
#define GPIO1_20  20
#define GPIO1_21  21
#define GPIO1_22  22
#define GPIO1_23  23
#define GPIO1_24  24
#define GPIO1_25  25
#define GPIO1_26  26
#define GPIO1_27  27
#define GPIO1_28  28
#define GPIO1_29  29
#define GPIO1_30  30
#define GPIO1_31  31

/******************************************************
 *                      INPUT PINS
 ******************************************************/



/******************************************************
 *                       OUTPUT PINS
 ******************************************************/
#define CHAN0_PIN		GPIO1_6
#define CHAN1_PIN		GPIO1_7
#define CHAN2_PIN		GPIO1_2
#define CHAN3_PIN		GPIO1_3

#define OVF_PIN			GPIO1_0
#define SEIZ_PIN		GPIO1_13

#define ERROR_LED      GPIO1_21
#define WORKING_LED    GPIO1_23

/******************************************************
 *                          BANKS
 ******************************************************/

#define PINS_1					(SOC_GPIO_1_REGS)


#endif /* Pindefs_h */
