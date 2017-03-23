/**
 * @file		SPI.cpp
 * @brief		
 * @authors		Thanasis Karapatis  <mailto:atkarapa@gmail.com>
 * @copyright	Delft University of Technology
 * @date 		2013
 * @version		1.5
 */



#include "SPI.h"
#include "hw_control_AM335x.h"
#include "soc_AM335x.h"
#include "hw_cm_per.h"
#include "hw_types.h"
#include "mcspi.h"

#include "types.h"


static void McSPI0PinMuxSetup(void);
static void McSPI0ModuleClkConfig(void);

void McSPIinit(void){

	/* Enable the clocks for McSPI0 module.*/
	McSPI0ModuleClkConfig();

	 /* Perform Pin-Muxing for SPI0 Instance */
	McSPI0PinMuxSetup();

	/* Reset the McSPI instance.*/
	McSPIReset(SOC_SPI_0_REGS);

	/* Enable chip select pin.*/
	McSPICSEnable(SOC_SPI_0_REGS);

	/* Enable master mode of operation.*/
	McSPIMasterModeEnable(SOC_SPI_0_REGS);

}

/**
 * \brief Initiailizes the specific SPI channel
 *
 * \param chan	Channel to be initialized
 *
 * \note Incomplete
 * \todo Not supported by current design
 *
 */
void McSPIchXInit(int chan){
	/* Perform the necessary configuration for master mode.*/
	McSPIMasterModeConfig(SOC_SPI_0_REGS, MCSPI_SINGLE_CH,
						  MCSPI_TX_ONLY_MODE, MCSPI_DATA_LINE_COMM_MODE_1,
						  chan);

	/* Configure the McSPI bus clock depending on clock mode. */
	McSPIClkConfig(SOC_SPI_0_REGS, MCSPI_IN_CLK, MCSPI_OUT_FREQ, chan,
				   MCSPI_CLK_MODE_0);

	/* Configure the word length.*/
	McSPIWordLengthSet(SOC_SPI_0_REGS, MCSPI_WORD_LENGTH(32), chan);

	/* Set polarity of SPIEN to low.*/
	McSPICSPolarityConfig(SOC_SPI_0_REGS, MCSPI_CS_POL_LOW, chan);

	/* Disable the transmitter/receiver FIFO of McSPI peripheral.*/
	McSPITxFIFOConfig(SOC_SPI_0_REGS, MCSPI_TX_FIFO_DISABLE, chan);
	McSPIRxFIFOConfig(SOC_SPI_0_REGS, MCSPI_RX_FIFO_DISABLE, chan);

}

/**
 * \brief   This function selects the McSPI0 pins for use. The McSPI pins
 *          are multiplexed with pins of other peripherals in the SoC
 */
static void McSPI0PinMuxSetup(void) {

	HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_SCLK) =
		 (CONTROL_CONF_SPI0_SCLK_CONF_SPI0_SCLK_PUTYPESEL |
		  CONTROL_CONF_SPI0_SCLK_CONF_SPI0_SCLK_RXACTIVE);

	HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_D1) =
		 (CONTROL_CONF_SPI0_SCLK_CONF_SPI0_SCLK_PUTYPESEL  |
				 CONTROL_CONF_SPI0_SCLK_CONF_SPI0_SCLK_RXACTIVE);

	HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_CS0) =
			 (CONTROL_CONF_SPI0_SCLK_CONF_SPI0_SCLK_PUTYPESEL |
			  CONTROL_CONF_SPI0_SCLK_CONF_SPI0_SCLK_RXACTIVE);
}


/**
 * \brief   This function will configure the required clocks for McSPI0 instance.
 *
 * \return  None.
 *
 */
static void McSPI0ModuleClkConfig(void)
{
    HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) =
                             CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
     CM_PER_L3S_CLKSTCTRL_CLKTRCTRL) != CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) =
                             CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
     CM_PER_L3_CLKSTCTRL_CLKTRCTRL) != CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) =
                             CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
                               CM_PER_L3_INSTR_CLKCTRL_MODULEMODE) !=
                                   CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) =
                             CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) &
        CM_PER_L3_CLKCTRL_MODULEMODE) != CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) =
                             CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
                              CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL) !=
                                CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) =
                             CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) &
                             CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL) !=
                               CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKCTRL) =
                             CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKCTRL) &
      CM_PER_L4LS_CLKCTRL_MODULEMODE) != CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_CM_PER_REGS + CM_PER_SPI0_CLKCTRL) &= ~CM_PER_SPI0_CLKCTRL_MODULEMODE;

    HWREG(SOC_CM_PER_REGS + CM_PER_SPI0_CLKCTRL) |=
                             CM_PER_SPI0_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_SPI0_CLKCTRL) &
      CM_PER_SPI0_CLKCTRL_MODULEMODE) != CM_PER_SPI0_CLKCTRL_MODULEMODE_ENABLE);


    while(!(HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
            CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK));

    while(!(HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
            CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK));

    while(!(HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
           (CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK |
            CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L4_GCLK)));

    while(!(HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) &
           (CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_L4LS_GCLK |
            CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_SPI_GCLK)));

}


static volatile uint32_t *p_tx;
static uint32_t tx_length=0;

/**
 * \brief	Transfer the data stored in the buffer over the SPI0 channel.
 *			The transfer sends 32bits at a time
 *
 * \param 	txbuf	The buffer with the data
 * \param 	size	The size of the buffer
 *
 */
void McSPITransfer(uint32_t *txbuf, uint32_t size)
{
    p_tx = txbuf;
    tx_length = size;


    /* SPIEN line is forced to low state.*/
    McSPICSAssert(SOC_SPI_0_REGS, 0);

    /* Enable the McSPI channel for communication.*/
    McSPIChannelEnable(SOC_SPI_0_REGS, 0);

    /* Wait until control returns back from McSPI ISR.*/

    while(tx_length != 0){
		while ( (McSPIIntStatusGet(SOC_SPI_0_REGS)
				& MCSPI_INT_TX_EMPTY(0) ) != MCSPI_INT_TX_EMPTY(0));
		McSPIIntStatusClear(SOC_SPI_0_REGS, MCSPI_INT_TX_EMPTY(0));

		McSPITransmitData(SOC_SPI_0_REGS, (uint32_t) (*p_tx++), 0);
		tx_length--;
	}

    /* Force SPIEN line to the inactive state.*/
    McSPICSDeAssert(SOC_SPI_0_REGS, 0);

    /* Disable the McSPI channel.*/
    McSPIChannelDisable(SOC_SPI_0_REGS, 0);

}


static uint8_t TX_DATA[4] = { 0, 0, 0, 0 };

void setDACA(const uint16_t value){

	TX_DATA[2] = WRITE_DACA_UPDATE_ALL;
	TX_DATA[1] = UPPER_BYTE(value);
	TX_DATA[0] = LOWER_BYTE(value);

	McSPITransfer((uint32_t *)TX_DATA, 1);

}


void setDACB(uint16_t value){
	TX_DATA[2] = WRITE_DACB_UPDATE_ALL;
	TX_DATA[1] = UPPER_BYTE(value);
	TX_DATA[0] = LOWER_BYTE(value);

	McSPITransfer((uint32_t *)TX_DATA, 1);

}

void resetALL(void){

	/** Reset DACA */
	TX_DATA[2] = WRITE_DACA_UPDATE;
	TX_DATA[1] = UPPER_BYTE(0);
	TX_DATA[0] = LOWER_BYTE(0);
	McSPITransfer((uint32_t *)TX_DATA, 1);

	/** Reset DACB */
	TX_DATA[2] = WRITE_DACB_UPDATE;
	TX_DATA[1] = UPPER_BYTE(0);
	TX_DATA[0] = LOWER_BYTE(0);
	McSPITransfer((uint32_t *)TX_DATA, 1);
}



