/**
 * @file		adc.c
 * @brief		
 * @authors		thanasis
 * @copyright	Delft University of Technology
 * @date 		2013
 * @version
 */
/*Armv7a*/
#include "interrupt.h"
#include "cache.h"
#include "mmu.h"
#include "clock.h"

/* Platform */
#include "beaglebone.h"
#include "soc_AM335x.h"
#include "hw_types.h"
#include "hw_control_AM335x.h"

/*Drivers*/
#include "tsc_adc.h"
#include "dmtimer.h"

/*Custom libs*/
#include "adc.h"
#include "board.h"
#include "../FIR.h"

/******************************************************************************
 **                      INTERNAL VARIABLE DEFINITIONS
 *******************************************************************************/
volatile unsigned int dataAVL = NOT_AVAIL;
unsigned int timerCntIsr;
unsigned int sampleAIN[ NR_CHANNELS ] = {0,0,0,0};

//unsigned int sampleAIN0=0;
//unsigned int sampleAIN1=0;
//unsigned int sampleAIN2=0;
//unsigned int sampleAIN3=0;


/******************************************************************************
 **                      FUNCTION DEFINITIONS
 *******************************************************************************/
static void TSCADCModuleClkConfig(void);
static unsigned int TSCADCPinMuxSetUp(void);
static void CleanUpInterrupts(void);
static void StepConfigure(unsigned int, unsigned int, unsigned int);
static void TimerIsr(void);
static void ADCIsr();
static void TriggerTimerSetup(void);

extern void TSCADCBiasConfig(unsigned int baseAdd, unsigned int adcBiasSel);


void TSCADCModuleClkConfig(void)
{
    /* Configuring L3 Interface Clocks. */

    /* Writing to MODULEMODE field of CM_PER_L3_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) |=
          CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) &
           CM_PER_L3_CLKCTRL_MODULEMODE));

    /* Writing to MODULEMODE field of CM_PER_L3_INSTR_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) |=
          CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
           CM_PER_L3_INSTR_CLKCTRL_MODULEMODE));

    /* Writing to CLKTRCTRL field of CM_PER_L3_CLKSTCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) |=
          CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /* Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
           CM_PER_L3_CLKSTCTRL_CLKTRCTRL));

    /* Writing to CLKTRCTRL field of CM_PER_OCPWP_L3_CLKSTCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) |=
          CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
           CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL));

    /* Writing to CLKTRCTRL field of CM_PER_L3S_CLKSTCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) |=
          CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
           CM_PER_L3S_CLKSTCTRL_CLKTRCTRL));

    /* Checking fields for necessary values.  */

    /* Waiting for IDLEST field in CM_PER_L3_CLKCTRL register to be set to 0x0. */
    while((CM_PER_L3_CLKCTRL_IDLEST_FUNC << CM_PER_L3_CLKCTRL_IDLEST_SHIFT)!=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) & CM_PER_L3_CLKCTRL_IDLEST));

    /*
    ** Waiting for IDLEST field in CM_PER_L3_INSTR_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_PER_L3_INSTR_CLKCTRL_IDLEST_FUNC <<
           CM_PER_L3_INSTR_CLKCTRL_IDLEST_SHIFT)!=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
           CM_PER_L3_INSTR_CLKCTRL_IDLEST));

    /*
    ** Waiting for CLKACTIVITY_L3_GCLK field in CM_PER_L3_CLKSTCTRL register to
    ** attain the desired value.
    */
    while(CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
           CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK));

    /*
    ** Waiting for CLKACTIVITY_OCPWP_L3_GCLK field in CM_PER_OCPWP_L3_CLKSTCTRL
    ** register to attain the desired value.
    */
    while(CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
           CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK));

    /*
    ** Waiting for CLKACTIVITY_L3S_GCLK field in CM_PER_L3S_CLKSTCTRL register
    ** to attain the desired value.
    */
    while(CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
          CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK));


    /* Configuring registers related to Wake-Up region. */

    /* Writing to MODULEMODE field of CM_WKUP_CONTROL_CLKCTRL register. */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) |=
          CM_WKUP_CONTROL_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_WKUP_CONTROL_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) &
           CM_WKUP_CONTROL_CLKCTRL_MODULEMODE));

    /* Writing to CLKTRCTRL field of CM_PER_L3S_CLKSTCTRL register. */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) |=
          CM_WKUP_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_WKUP_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
           CM_WKUP_CLKSTCTRL_CLKTRCTRL));

    /* Writing to CLKTRCTRL field of CM_L3_AON_CLKSTCTRL register. */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L3_AON_CLKSTCTRL) |=
          CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L3_AON_CLKSTCTRL) &
           CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKTRCTRL));

    /* Writing to MODULEMODE field of CM_WKUP_TSC_CLKCTRL register. */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_ADC_TSC_CLKCTRL) |=
          CM_WKUP_ADC_TSC_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_WKUP_ADC_TSC_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_ADC_TSC_CLKCTRL) &
           CM_WKUP_ADC_TSC_CLKCTRL_MODULEMODE));

    /* Verifying if the other bits are set to required settings. */

    /*
    ** Waiting for IDLEST field in CM_WKUP_CONTROL_CLKCTRL register to attain
    ** desired value.
    */
    while((CM_WKUP_CONTROL_CLKCTRL_IDLEST_FUNC <<
           CM_WKUP_CONTROL_CLKCTRL_IDLEST_SHIFT) !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) &
           CM_WKUP_CONTROL_CLKCTRL_IDLEST));

    /*
    ** Waiting for CLKACTIVITY_L3_AON_GCLK field in CM_L3_AON_CLKSTCTRL
    ** register to attain desired value.
    */
    while(CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKACTIVITY_L3_AON_GCLK !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L3_AON_CLKSTCTRL) &
           CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKACTIVITY_L3_AON_GCLK));

    /*
    ** Waiting for IDLEST field in CM_WKUP_L4WKUP_CLKCTRL register to attain
    ** desired value.
    */
    while((CM_WKUP_L4WKUP_CLKCTRL_IDLEST_FUNC <<
           CM_WKUP_L4WKUP_CLKCTRL_IDLEST_SHIFT) !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_L4WKUP_CLKCTRL) &
           CM_WKUP_L4WKUP_CLKCTRL_IDLEST));

    /*
    ** Waiting for CLKACTIVITY_L4_WKUP_GCLK field in CM_WKUP_CLKSTCTRL register
    ** to attain desired value.
    */
    while(CM_WKUP_CLKSTCTRL_CLKACTIVITY_L4_WKUP_GCLK !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
           CM_WKUP_CLKSTCTRL_CLKACTIVITY_L4_WKUP_GCLK));

    /*
    ** Waiting for CLKACTIVITY_L4_WKUP_AON_GCLK field in CM_L4_WKUP_AON_CLKSTCTRL
    ** register to attain desired value.
    */
    while(CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL_CLKACTIVITY_L4_WKUP_AON_GCLK !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL) &
           CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL_CLKACTIVITY_L4_WKUP_AON_GCLK));

    /*
    ** Waiting for CLKACTIVITY_ADC_FCLK field in CM_WKUP_CLKSTCTRL
    ** register to attain desired value.
    */
    while(CM_WKUP_CLKSTCTRL_CLKACTIVITY_ADC_FCLK !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
           CM_WKUP_CLKSTCTRL_CLKACTIVITY_ADC_FCLK));

    /*
    ** Waiting for IDLEST field in CM_WKUP_ADC_TSC_CLKCTRL register to attain
    ** desired value.
    */
    while((CM_WKUP_ADC_TSC_CLKCTRL_IDLEST_FUNC <<
           CM_WKUP_ADC_TSC_CLKCTRL_IDLEST_SHIFT) !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_ADC_TSC_CLKCTRL) &
           CM_WKUP_ADC_TSC_CLKCTRL_IDLEST));
}


unsigned int TSCADCPinMuxSetUp(void)
{

    HWREG( SOC_CONTROL_REGS + CONTROL_CONF_AIN0) = CONTROL_CONF_AIN0_CONF_AIN0_RXACTIVE;

    HWREG( SOC_CONTROL_REGS + CONTROL_CONF_AIN1) = CONTROL_CONF_AIN1_CONF_AIN1_RXACTIVE;

    HWREG( SOC_CONTROL_REGS +  CONTROL_CONF_AIN2)= CONTROL_CONF_AIN2_CONF_AIN2_RXACTIVE;

    HWREG( SOC_CONTROL_REGS + CONTROL_CONF_AIN3) = CONTROL_CONF_AIN3_CONF_AIN3_RXACTIVE;

    HWREG( SOC_CONTROL_REGS + CONTROL_CONF_AIN4) = CONTROL_CONF_AIN4_CONF_AIN4_RXACTIVE;

    HWREG( SOC_CONTROL_REGS + CONTROL_CONF_AIN5) = CONTROL_CONF_AIN5_CONF_AIN5_RXACTIVE;

    HWREG( SOC_CONTROL_REGS + CONTROL_CONF_AIN6) = CONTROL_CONF_AIN6_CONF_AIN6_RXACTIVE;

    HWREG( SOC_CONTROL_REGS + CONTROL_CONF_AIN7) = CONTROL_CONF_AIN7_CONF_AIN7_RXACTIVE;

    HWREG( SOC_CONTROL_REGS + CONTROL_CONF_VREFP)= CONTROL_CONF_VREFP_CONF_VREFP_RXACTIVE;

    HWREG( SOC_CONTROL_REGS +  CONTROL_CONF_VREFN)= CONTROL_CONF_VREFN_CONF_VREFN_RXACTIVE;
    return TRUE;
}


void ADCConfigure(void)
{
	int chan = 0;

	for (chan = 0; chan < NR_CHANNELS; ++chan) {
		sampleAIN[chan]=0;
	}

    /* Enable the clock for touch screen */
    TSCADCModuleClkConfig();

    TSCADCPinMuxSetUp();

    /* Configures ADC to 24Mhz */
    TSCADCConfigureAFEClock(SOC_ADC_TSC_0_REGS, 24000000, 24000000);

    /* Enable Transistor bias */
	TSCADCTSTransistorConfig(SOC_ADC_TSC_0_REGS, TSCADC_TRANSISTOR_DISABLE);

	TSCADCBiasConfig(SOC_ADC_TSC_0_REGS, TSCADC_INTERNAL_AC_BIAS);

	/* Enable tagging of data with channel ID */
    TSCADCStepIDTagConfig(SOC_ADC_TSC_0_REGS, 1);

    /* Set trigger to HW input event  */
    TSCADCHWEventMapSet(SOC_ADC_TSC_0_REGS, TSCADC_HW_INPUT_EVENT);

    /* Select input of external HW input event, DMTimer4 */
    HWREG(SOC_CONTROL_REGS + CONTROL_ADC_EVT_CAPT) = 1 ;

    /* Disable Write Protection of Step Configuration regs*/
    TSCADCStepConfigProtectionDisable(SOC_ADC_TSC_0_REGS);

    /* Configure step 1 for channel 1(AN0)*/
    StepConfigure(0, TSCADC_FIFO_0, TSCADC_POSITIVE_INP_CHANNEL1);

    /* Configure step 2 for channel 2(AN1)*/
    StepConfigure(1, TSCADC_FIFO_0, TSCADC_POSITIVE_INP_CHANNEL2);

    /* Configure step 3 for channel 3(AN2)*/
	StepConfigure(2, TSCADC_FIFO_0, TSCADC_POSITIVE_INP_CHANNEL3);

	/* Configure step 4 for channel 4(AN3)*/
	StepConfigure(3, TSCADC_FIFO_0, TSCADC_POSITIVE_INP_CHANNEL4);

    /* General purpose inputs */
    TSCADCTSModeConfig(SOC_ADC_TSC_0_REGS, TSCADC_GENERAL_PURPOSE_MODE);

    /* Enable step 1 */
    TSCADCConfigureStepEnable(SOC_ADC_TSC_0_REGS, 1, 1);

    /* Enable step 2 */
    TSCADCConfigureStepEnable(SOC_ADC_TSC_0_REGS, 2, 1);

    /* Enable step 3 */
	TSCADCConfigureStepEnable(SOC_ADC_TSC_0_REGS, 3, 1);

	/* Enable step 4 */
	TSCADCConfigureStepEnable(SOC_ADC_TSC_0_REGS, 4, 1);

	/* Set threshold of FIFO to 4 data */
    TSCADCFIFOIRQThresholdLevelConfig(SOC_ADC_TSC_0_REGS,TSCADC_FIFO_0, 4);

    /* Clear the status of all interrupts */
    CleanUpInterrupts();

    /* End of sequence interrupt is enable */
    TSCADCEventInterruptEnable(SOC_ADC_TSC_0_REGS, TSCADC_FIFO0_THRESHOLD_INT);

    /* Enable the TSC_ADC_SS module*/
    TSCADCModuleStateSet(SOC_ADC_TSC_0_REGS, TSCADC_MODULE_ENABLE);

    /* Enable Timer Triggering */
    TriggerTimerSetup();
}


/* Configures the step */
void StepConfigure(unsigned int stepSel, unsigned int fifo,
                   unsigned int positiveInpChannel)
{

    /* Configure ADC to Single ended operation mode */
	TSCADCTSStepOperationModeControl(SOC_ADC_TSC_0_REGS,
			TSCADC_SINGLE_ENDED_OPER_MODE, stepSel);

	TSCADCTSStepConfig(SOC_ADC_TSC_0_REGS, stepSel, TSCADC_NEGATIVE_REF_VSSA,
			positiveInpChannel, TSCADC_NEGATIVE_INP_ADCREFM,
			TSCADC_POSITIVE_REF_VDDA);

	TSCADCTSStepFIFOSelConfig(SOC_ADC_TSC_0_REGS, stepSel, fifo); /* select fifo 0 or 1*/
	TSCADCTSStepModeConfig(SOC_ADC_TSC_0_REGS, stepSel,
			TSCADC_ONE_SHOT_SOFTWARE_ENABLED); /* Configure ADC to one shot mode */

	TSCADCTSStepModeConfig(SOC_ADC_TSC_0_REGS, stepSel,
				TSCADC_CONTINIOUS_HARDWARE_SYNC);

//	TSCADCTSStepAverageConfig(SOC_ADC_TSC_0_REGS , stepSel, TSCADC_TWO_SAMPLES_AVG);
//	TSCADCTSStepOpenDelayConfig(SOC_ADC_TSC_0_REGS , stepSel, 0);
//	TSCADCTSStepSampleDelayConfig(SOC_ADC_TSC_0_REGS , stepSel, 2);

}

/* Clear status of all interrupts */
static void CleanUpInterrupts(void)
{
    TSCADCIntStatusClear(SOC_ADC_TSC_0_REGS, 0x7FF);

}

/* Reads the data from FIFO 0 and FIFO 1 */
static void ADCIsr()
{
    volatile unsigned int status;
    unsigned int wordsInFiFo =0;
    unsigned int sampleData = 0;
    unsigned int sample_ID =0;

    /* Read Interrupt event */
    status = TSCADCIntStatus(SOC_ADC_TSC_0_REGS);

    if(status & TSCADC_FIFO0_THRESHOLD_INT)
    {
         /* Read number of data in fifo 0 */
    	wordsInFiFo = TSCADCFIFOWordCountRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_0);

    	while ( wordsInFiFo > 0 ){
    		sampleData = HWREG(SOC_ADC_TSC_0_REGS + TSC_ADC_SS_FIFODATA(TSCADC_FIFO_0));
    		sample_ID = (sampleData>>16) & 0xF;

    		if(sample_ID < NR_CHANNELS )
    			sampleAIN[ sample_ID ] = sampleData & 0xFFF;
    		else
    			sampleAIN[ NR_CHANNELS-1 ] = sampleData & 0xFFF;

//    		if( sample_ID == 0 ){
//				sampleAIN0 = sample_temp & 0xFFF;
//			} else if (sample_ID == 1) {
//				sampleAIN1 = sample_temp & 0xFFF;
//			} else if (sample_ID == 2) {
//				sampleAIN2 = sample_temp & 0xFFF;
//			} else {
//				sampleAIN3 = sample_temp & 0xFFF;
//			}
    		/* See if fifo empty */
    		wordsInFiFo = TSCADCFIFOWordCountRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_0);
    	}
    	dataAVL = AVAILABLE;
    }
    /* Clear interrupt */
    TSCADCIntStatusClear(SOC_ADC_TSC_0_REGS, status);
}

/**
 *  Register interrupt ISRs
 */
void SetupIntc(void)
{
   /* Register ADCIsr */
    IntRegister(SYS_INT_ADC_TSC_GENINT, ADCIsr);
    IntPrioritySet(SYS_INT_ADC_TSC_GENINT, 0, AINTC_HOSTINT_ROUTE_IRQ);
    IntSystemEnable(SYS_INT_ADC_TSC_GENINT);

    /* Registering DMTimerIsr */
    IntRegister(SYS_INT_TINT4, TimerIsr);
    IntPrioritySet(SYS_INT_TINT4, 0, AINTC_HOSTINT_ROUTE_IRQ);
    IntSystemEnable(SYS_INT_TINT4);

}

/**
 * Setup DTimer to trigger an overflow interupt every SAMPLE_PERIOD
 *
 */
void TriggerTimerSetup(void) {

	unsigned int countVal = TIMER_OVERFLOW - (SAMPLING_PERIOD_MS * TIMER_1MS_COUNT);

	DMTimer4ModuleClkConfig();

	DMTimerDisable(SOC_DMTIMER_4_REGS);

	DMTimerCounterSet(SOC_DMTIMER_4_REGS, countVal);
	DMTimerReloadSet(SOC_DMTIMER_4_REGS, countVal);

	/* Configure the DMTimer for Auto-reload and compare mode */
	DMTimerModeConfigure(SOC_DMTIMER_4_REGS, DMTIMER_AUTORLD_NOCMP_ENABLE);

	/* Enable the DMTimer interrupts */
	DMTimerIntEnable(SOC_DMTIMER_4_REGS, DMTIMER_INT_OVF_EN_FLAG);

	/* Start the DMTimer */
	DMTimerEnable(SOC_DMTIMER_4_REGS);

}


static void TimerIsr(void) {
	timerCntIsr++;
	DMTimerIntStatusClear(SOC_DMTIMER_4_REGS, DMTIMER_INT_OVF_EN_FLAG);
}


void DisableTimerADC(){
	DMTimerDisable(SOC_DMTIMER_4_REGS);
}


void EnableTimerADC(){
	unsigned int countVal = TIMER_OVERFLOW - (SAMPLING_PERIOD_MS * TIMER_1MS_COUNT);
	DMTimerCounterSet(SOC_DMTIMER_4_REGS, countVal);
	DMTimerReloadSet(SOC_DMTIMER_4_REGS, countVal);
	DMTimerEnable(SOC_DMTIMER_4_REGS);
}




