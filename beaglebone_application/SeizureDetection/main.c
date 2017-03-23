/**
 * \file 		main.c
 * \brief 		Main application
 * \authors 	Thanasis Karapatis \t <mailto:atkarapa@gmail.com>
 * \copyright 	Delft University of Technology/ Erasmus MC
 * \date 		2014
 * \version   	1.0
 *
 */


/*Armv7a*/
#include "cache.h"


/* Platform */
#include "beaglebone.h"
#include "soc_AM335x.h"
#include "hw_types.h"
#include "hw_control_AM335x.h"


/*Drivers*/
#include "tsc_adc.h"
#include "dmtimer.h"
#include "uart_irda_cir.h"

/*Utils*/
#include <perf.h>
#include <delay.h>
#include <consoleUtils.h>
#include "uartStdio.h"


/*C libs*/
#include <inttypes.h>

/*Custom libs*/
#include "board/board.h"
#include "board/SPI.h"
#include "board/adc.h"
#include "FIR.h"

//#include "FIR_funct.h"

/******************************************************************************
 **                      INTERNAL MACRO/TYPE DEFINITIONS
 *******************************************************************************/
#define RESOL_X_MILLION           (439u)

#define DBG

#define BUSY						1
#define DONE						(!BUSY)

#define OK							1
#define NOT_OK						(!OK)

#define ENABLED						1
#define DISABLED					(!ENABLED)

#define SEIZ_OFF					0
#define SEIZ_ON						(!SEIZ_OFF)

#define INIT_CHANNEL 				{0,0,0,ENABLED}

typedef struct{
	int16_t *ain;
	uint8_t seizure;
	uint8_t enabled;
	int32_t sum;
	int32_t threshUPPER;
	int32_t threshLOWER;
}channelSeiz;

typedef struct{
	int channelDAC;
	uint32_t counter;
	//TODO: enable/disable DAC
	uint8_t enabledDAC;
	uint8_t enabledRecord;
	//TODO: set DAC scale
	uint32_t DACscale;
	uint32_t ledOnTime;
	uint32_t ledlockTime;
	uint32_t ledDelayTime;

	uint8_t continuousSUM;
	uint32_t counter_c;

	int32_t maxSUM[NR_CHANNELS];

	uint8_t GUI;


}monitorT;

/*******************************************************************************
 **                     EXTERNAL VARIABLE DEFINITIONS
 *******************************************************************************/
extern volatile unsigned int dataAVL;
extern unsigned int sampleAIN[ NR_CHANNELS ];
extern unsigned int timerCntIsr;

/*******************************************************************************
 **                     INTERNAL VARIABLE DEFINITIONS
 *******************************************************************************/
/*
 * Purpose of this value is to see if everything is going as
 * planned, Turn LED on if timer trigger and processing is still BUSY
 * Maybe try to increase
 */
int processing = DONE;

int16_t AIN[ NR_CHANNELS ][NR_SAMPLES]  __attribute__ ((aligned (4)));
int16_t coeffRE[NR_SAMPLES] __attribute__ ((aligned (4)));
int16_t coeffIM[NR_SAMPLES] __attribute__ ((aligned (4)));


/*******************************************************************************
 **                      COMPILE TIME ERROR
 *******************************************************************************/
#define BUILD_BUG_ON_ZERO(e) (sizeof(struct _t{ int :-!!(e); }) )


/*******************************************************************************
 **                      INTERNAL FUNCTION PROTOTYPES
 *******************************************************************************/
// Commands
void processCommand(signed char c , channelSeiz *chanTable, monitorT *mon);
void selectChannelsCommand(channelSeiz *chanTable);
void changeThresholdCommand(channelSeiz *channelTable);
void setMonitorChannelCommand(monitorT *mon);
void setLEDdurationCommand(monitorT *mon);
void oneTimeRecordCommand(monitorT *mon, int fast);
void toggleGUI( monitorT *mon );


void resetLEDS();
void printMenu();


//TODO:Set Coeff scaling
void selectCoeffSet(void);
void printCoeffs(int16_t *coef_RE, int16_t *coef_IM );

void initMonitor(monitorT *mon);

void resetChannels(channelSeiz *channelTable);
void setChanEnable(channelSeiz *chanTable, int sel);
void setChanThres(channelSeiz *chan, int32_t upper, int32_t lower);
void printEnabledChannels(channelSeiz *chanTable);

int main(void) {

	/* Temp variables */
	int ticks=0;
	int chan=0;

	int counterLED = 0;
	int toggleLED  = HIGH;

	/* Local data */
	channelSeiz chanTable[NR_CHANNELS];
	uint8_t GLOBAL_SEIZ   = SEIZ_OFF;
	uint8_t SEIZ_LOCK     = DISABLED;
	signed char command  = -1;
	int initialized = NOT_OK;
	int position    = 0;
	int	sum_inter   = 0;
	float sumf      = 0.0;
	int avgTicks    = 0;

	monitorT monitor;

	/*=======================================================================*/
	/*         Initialization                                                */
	/*=======================================================================*/
	/* Initialize the board */
	boardInit();
	ConsoleUtilsPrintf("Initializing....\n");

	/* Setup Interrupts for ADC and timer */
	SetupIntc();

	/* Performance timer*/
	SysPerfTimerSetup();

	/* Initialize ADC */
	ADCConfigure();
	McSPIinit();
	McSPIchXInit(0);


	/* Initialize arrays */
	resetArrays();
	resetLEDS();
	initMonitor(&monitor);

	/* Select channel) */
	resetChannels( chanTable );

	ConsoleUtilsPrintf("Initialized....\n");
	/*=======================================================================*/


	printEnabledChannels( chanTable );
	ConsoleUtilsPrintf("Monitor channel is:%d\n", monitor.channelDAC);
	ConsoleUtilsPrintf("|TIME| Delay:%d  ON:%d  Lock:%d\n", monitor.ledDelayTime, monitor.ledOnTime, monitor.ledlockTime );
	printCoeffs( coeffRE, coeffIM );

	/* reset position */
	position = 0;
	while (1) {

		/* Wait for sample */
		while (dataAVL == NOT_AVAIL);

		/* Reset variable */
		dataAVL = NOT_AVAIL;
		SysPerfTimerConfig(1);

		/* TODO: Will be used to check if triggering  */
		processing = BUSY;


		/*8888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888*/
		/* Store new sample for all channels  */
		for (chan = 0; chan < NR_CHANNELS; ++chan) {
			//TODO: add macro or input variable for input values scaling
			chanTable[chan].ain[position] = sampleAIN[chan]/64;
		}

		if (initialized == OK) {
			/*  Filtering  */
			for (chan = 0; chan < NR_CHANNELS; ++chan) {
				fir_simple_wav(&(chanTable[chan].sum), chanTable[chan].ain,
						coeffRE, coeffIM, position);
			}

			/* Output filter sum to DAC */
			if (monitor.enabledDAC == ENABLED) {
				sum_inter = chanTable[monitor.channelDAC].sum;
				sumf = sqrtf((float) sum_inter);
				sum_inter = (int32_t) sumf;
				setDACA((monitor.DACscale) * (uint16_t) sum_inter);
			}

			if (monitor.continuousSUM == ENABLED) {

				if (monitor.GUI == ENABLED) {


					// Start of telemetry data
					UARTPutc((unsigned char) 200);


					UARTPutc(( sampleAIN[monitor.channelDAC] & 0xFF000000) >> 24);
					UARTPutc(( sampleAIN[monitor.channelDAC] & 0x00FF0000) >> 16);
					UARTPutc(( sampleAIN[monitor.channelDAC] & 0x0000FF00) >> 8);
					UARTPutc( sampleAIN[monitor.channelDAC] & 0x000000FF);


					UARTPutc((chanTable[monitor.channelDAC].sum & 0xFF000000)>> 24);
					UARTPutc((chanTable[monitor.channelDAC].sum & 0x00FF0000)>> 16);
					UARTPutc((chanTable[monitor.channelDAC].sum & 0x0000FF00)>> 8);
					UARTPutc(chanTable[monitor.channelDAC].sum & 0x000000FF);

					UARTPutc('\r');
					UARTPutc('\n');
					// End of telemetry data


				} else {
					monitor.counter_c++;
					if (monitor.counter_c == 6) {
						monitor.counter_c = 0;

						ConsoleUtilsPrintf("CH0:%10d CH1:%10d CH2:%10d CH3:%10d\n",
								chanTable[CHANNEL_0].sum,
								chanTable[CHANNEL_1].sum,
								chanTable[CHANNEL_2].sum,
								chanTable[CHANNEL_3].sum);

					}
				}
			}

			/* Print last 200 values */
			if (monitor.enabledRecord == ENABLED) {
				monitor.counter++;

				/* Get max value */
				for (chan = 0; chan < NR_CHANNELS; ++chan) {
					if (chanTable[chan].sum > monitor.maxSUM[chan]) {
						monitor.maxSUM[chan] = chanTable[chan].sum;
					}
				}

				if (monitor.counter == 200) {
					monitor.counter = 0;
					monitor.enabledRecord = DISABLED;

					ConsoleUtilsPrintf(" Max values\n");
					ConsoleUtilsPrintf("CH0:%10d CH1:%10d CH2:%10d CH3:%10d\n",
							monitor.maxSUM[CHANNEL_0],
							monitor.maxSUM[CHANNEL_1],
							monitor.maxSUM[CHANNEL_2],
							monitor.maxSUM[CHANNEL_3]);
				} else {
					ConsoleUtilsPrintf("CH0:%10d CH1:%10d CH2:%10d CH3:%10d\n",
							chanTable[CHANNEL_0].sum, chanTable[CHANNEL_1].sum,
							chanTable[CHANNEL_2].sum, chanTable[CHANNEL_3].sum);
				}

			}

			/* Channel checking  */
			for (chan = 0; chan < NR_CHANNELS; ++chan) {

				//if( chanTable[chan].enabled == DISABLED )
				//	continue;

				if ( chanTable[chan].seizure == SEIZ_OFF &&
				     chanTable[chan].sum > chanTable[chan].threshUPPER)
				{

					chanTable[chan].seizure = SEIZ_ON;
					// TURN LED ON
					pinWrite(chan2pin[chan], PINS_1, HIGH);

					//Consider removing enabled
					if( chanTable[chan].enabled == ENABLED )
						ConsoleUtilsPrintf("CHAN:%d DETECTED %11d\n", chan,chanTable[chan].sum );

				}

				if (chanTable[chan].seizure == SEIZ_ON
						&& chanTable[chan].sum < chanTable[chan].threshLOWER) {
					chanTable[chan].seizure = SEIZ_OFF;
					//TURN LED OFF
					pinWrite(chan2pin[chan], PINS_1, LOW);
				}
			}

			/*
			 * en seiz   result
			 *  0    0    as is (1)
			 *  0    1    as is (1)
			 *  1    0    0
			 *  1    1    1
			 *
			 * In boolean logic
			 * Result = en' + seiz
			 *
			 */
			GLOBAL_SEIZ = SEIZ_ON;
			/* If channel disabled then leave value as is, otherwise check whether channel detected seizure */
			GLOBAL_SEIZ &= (~chanTable[CHANNEL_0].enabled)
					| chanTable[CHANNEL_0].seizure;
			GLOBAL_SEIZ &= (~chanTable[CHANNEL_1].enabled)
					| chanTable[CHANNEL_1].seizure;
			GLOBAL_SEIZ &= (~chanTable[CHANNEL_2].enabled)
					| chanTable[CHANNEL_2].seizure;
			GLOBAL_SEIZ &= (~chanTable[CHANNEL_3].enabled)
					| chanTable[CHANNEL_3].seizure;
			/*if all are disabled the result is SEIZ_OFF */
			GLOBAL_SEIZ &= chanTable[CHANNEL_0].enabled
					| chanTable[CHANNEL_1].enabled
					| chanTable[CHANNEL_2].enabled
					| chanTable[CHANNEL_3].enabled;

			if (GLOBAL_SEIZ == SEIZ_ON && SEIZ_LOCK == DISABLED) {
				//TURN LED ON
				SEIZ_LOCK = ENABLED;
				EnableTimerSEIZ(monitor.ledDelayTime, monitor.ledOnTime,
						monitor.ledlockTime, &SEIZ_LOCK);
			}

		}

		else {
			if (position == NR_SAMPLES - 1) {
				ConsoleUtilsPrintf("S\n");
				initialized = OK;
			}
		}
		/*8888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888*/

		ticks = SysPerfTimerConfig(0);

		// This should be used to get a time indication
		if(avgTicks == 0 ){
			avgTicks = ticks;
		}
		else{
			avgTicks = (avgTicks+ticks)/2;
		}

		if(ticks/TIMER_1MS_COUNT > SAMPLING_PERIOD_MS  ){
			ConsoleUtilsPrintf(" Time issues usecs :%d\n", ticks/TIMER_1US_COUNT);
		}
		else{
			counterLED++;
			if(counterLED==30){
				// TODO: Enable/Disable time printing
				// ConsoleUtilsPrintf(" Ticks :%d\n", avgTicks/TIMER_1US_COUNT);
				counterLED=0;
				pinWrite(WORKING_LED, PINS_1, toggleLED);
				toggleLED = !toggleLED;
			}
		}

		processing = DONE;
		position = (position+1)%NR_SAMPLES;


		command = UARTCharGetNonBlocking(SOC_UART_0_REGS);
		if(command == 'a'){
			//Enable telemetry data
			UARTCharGetNonBlocking(SOC_UART_0_REGS);
			monitor.continuousSUM = !monitor.continuousSUM;
			monitor.counter_c = 0;
		}

		else if(command == 'k'){

			UARTCharGetNonBlocking(SOC_UART_0_REGS);
				oneTimeRecordCommand(&monitor, ENABLED);
		}
		else if(command == 'z'){
				// Give
				UARTCharGetNonBlocking(SOC_UART_0_REGS);
				EnableTimerSEIZ(monitor.ledDelayTime, monitor.ledOnTime,
									monitor.ledlockTime, &SEIZ_LOCK);
		}
		else if(command == '\n' || command == '\r'){
				// Discard any extra new line chars or carriage returns
				UARTCharGetNonBlocking(SOC_UART_0_REGS);
		}
		else if(command != -1){

			DisableTimerADC();

			//Just in case to remove any other pennding chars
			UARTCharGetNonBlocking(SOC_UART_0_REGS);

			processCommand( command, chanTable, &monitor );

			/* Check Data are ok */
			ConsoleUtilsPrintf("Configuration\n");
			ConsoleUtilsPrintf("===================\n");
			printEnabledChannels(chanTable);
			ConsoleUtilsPrintf("|Monitor| channel is:%d\n", monitor.channelDAC);
			ConsoleUtilsPrintf("|TIME   | Delay:%d  ON:%d  Lock:%d\n", monitor.ledDelayTime, monitor.ledOnTime, monitor.ledlockTime );
			ConsoleUtilsPrintf("|GUI    | is :");
			if(monitor.GUI == ENABLED){
				ConsoleUtilsPrintf("ENABLED\n");
			}
			else{
				ConsoleUtilsPrintf("DISABLED\n");
			}


			resetLEDS();
			EnableTimerADC();
			initialized = NOT_OK;

		}

	}
	return 0;
}


void resetLEDS(void){

	pinWrite(CHAN0_PIN, PINS_1, LOW);
	pinWrite(CHAN1_PIN, PINS_1, LOW);
	pinWrite(CHAN2_PIN, PINS_1, LOW);
	pinWrite(CHAN3_PIN, PINS_1, LOW);

	pinWrite(OVF_PIN, PINS_1, LOW);
	pinWrite(SEIZ_PIN, PINS_1, LOW);

	pinWrite(WORKING_LED, PINS_1, LOW);

}



void processCommand(signed char c, channelSeiz *chanTable, monitorT *mon){
	switch (c) {
		case 'm':
			printMenu();
			break;
		case 't':
			changeThresholdCommand(chanTable);
			break;
		case 'c':
			selectChannelsCommand(chanTable);
			break;
		case 'r':
			oneTimeRecordCommand(mon, DISABLED);
			break;
		case 'd':
			setMonitorChannelCommand(mon);
			break;
		case 'l':
			setLEDdurationCommand(mon);
			break;
		case 'g':
			toggleGUI(mon);
			break;
		default:
			ConsoleUtilsPrintf("Wrong command:%c\n", c);
			break;
	}

}

void resetChannels(channelSeiz *channelTable){
	int iter_ch=0;

	for (iter_ch = 0; iter_ch < NR_CHANNELS; ++iter_ch) {
		channelTable[iter_ch].ain     	  = AIN[iter_ch];
		channelTable[iter_ch].seizure 	  = SEIZ_OFF;
		channelTable[iter_ch].enabled 	  = DISABLED;
		channelTable[iter_ch].sum     	  = 0;
		channelTable[iter_ch].threshUPPER = THRES_U_DEFAULT;
		channelTable[iter_ch].threshLOWER = THRES_L_DEFAULT;
	}

	// Set channel 3 to high threshold
	channelTable[CHANNEL_3].threshUPPER = 175000000;

}


void changeThresholdCommand(channelSeiz *chanTable){
	int32_t upper=-1;
	int32_t lower=-1;
	int channel = 0;
	int chan_itr=0;


	ConsoleUtilsPrintf("\nSet channel thresholds in multiples of 1000 \n");
	ConsoleUtilsPrintf("Enter -1 if you wish to CANCEL/RETURN \n");

	do {
		ConsoleUtilsPrintf("\nEnter channel mask to select channels: \n");
		ConsoleUtilsScanf("%d", &channel);

		if(channel == -1){
			return;
		}

	} while (channel > CHAN_ALL || channel < 0 );


	for (chan_itr = 0; chan_itr < NR_CHANNELS; ++chan_itr) {
		if( ((1<<chan_itr) & channel) != 0 ){
			ConsoleUtilsPrintf("\nThresholds for channel : %d\n", chan_itr);

			do {
				ConsoleUtilsPrintf("Enter Upper Threshold: \n");
				ConsoleUtilsScanf("%d", &upper);
				ConsoleUtilsPrintf("value read\n");
			} while (upper <= 0);

			do {
				ConsoleUtilsPrintf("Enter lower Threshold: \n");
				ConsoleUtilsScanf("%d", &lower);
			} while (upper <= lower || lower < 0);



			setChanThres(&chanTable[chan_itr],  upper*1000, lower*1000);

		}
	}

}

void selectCoeffSet(void){

}


void selectChannelsCommand(channelSeiz *channelTable){
	int mask=0;

	ConsoleUtilsPrintf("Channel ENABLED selection\n");
	ConsoleUtilsPrintf("Enter -1 to CANCEL\n");


	do{
		ConsoleUtilsPrintf("Enter channel mask in HEX: \n");
		ConsoleUtilsScanf("%X", &mask);

		if( mask == -1){
			//Canceling
			return;
		}

	}while(mask > 0xF || mask < 0 );
	ConsoleUtilsPrintf("Mask selected is: %X\n", mask);

	if (mask < 0 ) {
		pinWrite(ERROR_LED, PINS_1, HIGH);
		ConsoleUtilsPrintf(
				"ERROR: Mask is negative, set DEFAULT VALUE\n");
		setChanEnable(channelTable, 0);

	} else {
		setChanEnable(channelTable, mask);
	}


}


void printMenu(){
	ConsoleUtilsPrintf("===========================\n");
	ConsoleUtilsPrintf("Menu:\n");
	ConsoleUtilsPrintf("t  : Threshold change\n");
	ConsoleUtilsPrintf("c  : Select channels(0-3)\n");
	ConsoleUtilsPrintf("m  : Menu print and reset\n");
	ConsoleUtilsPrintf("d  : Monitor output ( DAC/GUI )  selection\n");
	ConsoleUtilsPrintf("k  : Immediately show last 200 values\n");
	ConsoleUtilsPrintf("l  : Set LED Delay/ON/Lock durations\n");
	ConsoleUtilsPrintf("g  : Toggle GUI\n");
	ConsoleUtilsPrintf("z  : TEST LED\n");
	ConsoleUtilsPrintf("a  : Enable continuous monitoring\n");
	ConsoleUtilsPrintf("=============================\n");
	ConsoleUtilsPrintf("\n");

}

/**
 * \brief Prints enabled channel
 */
void printEnabledChannels(channelSeiz *chanTable){
	int chan=0;

	for (chan = 0; chan < NR_CHANNELS; ++chan) {
		ConsoleUtilsPrintf("Channel %d is %s UP:%10d DOWN:%10d\n",chan, chanTable[chan].enabled?"ENABLED ":"DISABLED" ,chanTable[chan].threshUPPER, chanTable[chan].threshLOWER );

	}
}


/**
 * \brief Prints coefficients table
 */
void printCoeffs(int16_t *coef_RE, int16_t *coef_IM ){
	int smpl_itr = 0;

	ConsoleUtilsPrintf("==================\n");
	ConsoleUtilsPrintf("Start Coefficients\n");
	for(smpl_itr=0; smpl_itr<NR_SAMPLES; smpl_itr++){
			ConsoleUtilsPrintf("Re:%4d Im:%4d\n",coef_RE[smpl_itr], coef_IM[smpl_itr]);
	}
	ConsoleUtilsPrintf("End Coefficients\n");
	ConsoleUtilsPrintf("==================\n");

}



/**
 * \brief Enables channels based on given mask
 *
 * \param chanTable Table that contains all the channels
 * \param sel		MASK where every bit stands for one channel
 *
 */
void setChanEnable(channelSeiz *chanTable, int sel){
	int iter_ch=0;

	for (iter_ch = 0; iter_ch < NR_CHANNELS; ++iter_ch) {
		chanTable[iter_ch].enabled 	 = ( (sel & (1<<iter_ch)) != 0);
	}

}

/**
 * \brief Set thresholds for one channel
 *
 * \param	chan	Pointer to channel that get thresholds values
 * \param	upper	Upper threshold
 * \param   lower	Lower threshold
 */
void setChanThres(channelSeiz *chan, int32_t upper, int32_t lower){
	chan->threshLOWER = lower;
	chan->threshUPPER = upper;
}

void initMonitor(monitorT *mon){
	int chan_itr=0;

	mon->channelDAC = CHANNEL_0;
	mon->counter = 0;
	mon->enabledDAC = ENABLED;
	mon->enabledRecord = DISABLED;
	mon->DACscale = 5;

	/* LED timing configurations */
	mon->ledOnTime		= 200;
	mon->ledlockTime	= 5000;
	mon->ledDelayTime 	= 0;


	mon->continuousSUM  = DISABLED;
	mon->counter_c      = 0;

	mon->GUI = DISABLED;

	/* Reseting maximum filter outputs */
	for (chan_itr = 0; chan_itr < NR_CHANNELS; ++chan_itr) {
		mon->maxSUM[ chan_itr ] = 0;
	}


}


void setMonitorChannelCommand(monitorT *mon) {
	int chan = 0;

	ConsoleUtilsPrintf("Enter which channel to see on the DAC\n");
	ConsoleUtilsPrintf("Enter -1 if you wish to CANCEL \n");


	do {
		ConsoleUtilsPrintf("Enter channel to monitor on DAC(0-3): \n");
		ConsoleUtilsScanf("%d", &chan);
		if( chan == -1  ){
			//Canceling
			return;
		}

	} while (chan >= NR_CHANNELS || chan < 0);

	mon->channelDAC = chan;

}


void oneTimeRecordCommand(monitorT *mon, int fast) {
	int chan_itr = 0;

	if( fast == DISABLED )
		ConsoleUtilsPrintf("Recording.....\n");

	mon->enabledRecord = ENABLED;
	mon->counter = 0;

	// Reseting
	for (chan_itr = 0; chan_itr < NR_CHANNELS; ++chan_itr) {
		mon->maxSUM[ chan_itr ] = 0;
	}

}


void setLEDdurationCommand(monitorT *mon){
	int32_t lockTime=0;
	int32_t onTime=0;
	int32_t delayTime=0;

	ConsoleUtilsPrintf("Enter Delay/ON/Lock times for TRIGGER LED\n");
	ConsoleUtilsPrintf("Enter -1 if you wish to CANCEL \n");

	//Set delay time
	do {
		ConsoleUtilsPrintf("Enter Delay time (msecs): \n");
		ConsoleUtilsScanf("%d", &delayTime);

		if( delayTime == -1 ){
			// Canceling
			return;
		}

		if (delayTime < 0)
			ConsoleUtilsPrintf("Cannot have negative delay time\n");
		else if( (uint32_t)delayTime > TIMER_OVERFLOW/TIMER_1MS_COUNT )
			ConsoleUtilsPrintf("Above capabilities of timer\n");

	} while (delayTime < 0 || (uint32_t)delayTime > TIMER_OVERFLOW/TIMER_1MS_COUNT);

	//Set on time
	do {
		ConsoleUtilsPrintf("Enter ON time (msecs) : \n");
		ConsoleUtilsScanf("%d", &onTime);
		if (onTime == -1) {
			// Canceling
			return;
		}

		if (onTime < 0)
			ConsoleUtilsPrintf("Cannot have negative delay time\n");
		else if( (uint32_t)onTime > TIMER_OVERFLOW/TIMER_1MS_COUNT )
			ConsoleUtilsPrintf("Above capabilities of timer\n");

	} while (onTime < 0 || (uint32_t)onTime > TIMER_OVERFLOW/TIMER_1MS_COUNT);

	// Set lock time
	do {
		ConsoleUtilsPrintf("Enter Lock time (msecs): \n");
		ConsoleUtilsScanf("%d", &lockTime);
		if (lockTime == -1) {
			// Canceling
			return;
		}

		if (lockTime < 0)
			ConsoleUtilsPrintf("Cannot have negative delay time\n");
		else if( (uint32_t)lockTime > TIMER_OVERFLOW/TIMER_1MS_COUNT )
			ConsoleUtilsPrintf("Above capabilities of timer\n");


	} while (lockTime < 0 || (uint32_t)lockTime > TIMER_OVERFLOW/TIMER_1MS_COUNT);


	// Values checking
	if (onTime < 0) {
		pinWrite(ERROR_LED, PINS_1, HIGH);
		ConsoleUtilsPrintf("ERROR: ON time is negative, set DEFAULT VALUE\n");
		//TODO: Define MACRO for default value
		mon->ledOnTime = 200;
	} else {
		mon->ledOnTime = onTime;
	}

	if (lockTime < 0) {
		pinWrite(ERROR_LED, PINS_1, HIGH);
		ConsoleUtilsPrintf("ERROR: Lock time is negative, set DEFAULT VALUE\n");
		//TODO: Define MACRO for default value
		mon->ledlockTime = 3000;
	} else {
		mon->ledlockTime = lockTime;
	}

	if (delayTime < 0) {
		pinWrite(ERROR_LED, PINS_1, HIGH);
		ConsoleUtilsPrintf(
				"ERROR: Delay time is negative, set DEFAULT VALUE\n");
		//TODO: Define MACRO for default value
		mon->ledDelayTime = 0;

	} else {
		mon->ledDelayTime = delayTime;
	}

}


void toggleGUI( monitorT *mon ){

	mon->GUI = !mon->GUI;
	ConsoleUtilsPrintf("GUI is :");
	if(mon->GUI == ENABLED){
		ConsoleUtilsPrintf(" ENABLED\n");
	}
	else{
		ConsoleUtilsPrintf(" DISABLED\n");
	}

}
