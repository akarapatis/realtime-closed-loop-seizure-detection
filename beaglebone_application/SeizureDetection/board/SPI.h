/**
 * @file		SPI.h
 * @brief		
 * @authors		Thanasis Karapatis  <mailto:atkarapa@gmail.com>
 * @copyright	Delft University of Technology
 * @date 		2013
 * @version
 */

#ifndef SPI_H_
#define SPI_H_

#include "binary.h"
#include "inttypes.h"

#define CHAN_0	0

#define WRITE_DACA				B00000000
#define WRITE_DACB				B00000001

#define UPDATE_DACA				B00010000
#define UPDATE_DACB				B00010001

#define WRITE_DACA_UPDATE		B00110000
#define WRITE_DACB_UPDATE		B00110001

#define WRITE_DACA_UPDATE_ALL 	B00100000
#define WRITE_DACB_UPDATE_ALL 	B00100001

#define POWERDOWN_DACA			B01000000
#define POWERDOWN_DACB			B01000001

#define POWERDOWN				B01001111

#define VDD						3.3

#define MCSPI_OUT_FREQ                   (24000000u) /* 12 MHz */
#define MCSPI_IN_CLK                     (48000000u) /* 48 MHz*/

void McSPIinit(void);
void McSPIchXInit(int chan);
void McSPITransfer(uint32_t *txbuf, uint32_t size);
void setDACA(const uint16_t value);
void setDACB(const uint16_t value);
void resetALL(void);

#endif /* SPI_H_ */
