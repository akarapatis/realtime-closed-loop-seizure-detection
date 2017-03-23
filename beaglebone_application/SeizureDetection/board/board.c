/**
 * @file		board.c
 * @brief		
 * @authors		Thanasis
 * @copyright	Delft University of Technology
 * @date 		2013
 * @version
 */

#include "cache.h"
#include "mmu.h"
#include "interrupt.h"

#include "hw_control_AM335x.h"
#include "hw_types.h"

#include "board.h"
#include "consoleUtils.h"
#include "gpio_v2.h"
#include "dmtimer.h"
#include "beaglebone.h"

/******************************************************************************
 **                      FUNCTION DEFINITIONS
 *******************************************************************************/
static void _MMUConfigAndEnable(void);
static void SEIZTimerSetup(void) ;
static void TimerIsrSEIZ(void);

/******************************************************************************
 **                      INTERNAL MACRO DEFINITIONS
 *******************************************************************************/
/* Definitions related to MMU Configuration. */
#define START_ADDR_DDR                 (0x80000000u)
#define START_ADDR_DEV                 (0x44000000u)
#define START_ADDR_OCMC                (0x40300000u)
#define NUM_SECTIONS_DDR               (512u)
#define NUM_SECTIONS_DEV               (960u)
#define NUM_SECTIONS_OCMC              (1u)

/******************************************************************************
 **                      INTERNAL VARIABLE DEFINITIONS
 *******************************************************************************/
/* Page tables start must be aligned to 16K boundary */
#ifdef __TMS470__
#pragma DATA_ALIGN(PageTable, MMU_PAGETABLE_ALIGN_SIZE);
static volatile unsigned int PageTable[MMU_PAGETABLE_NUM_ENTRY];

#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment=MMU_PAGETABLE_ALIGN_SIZE
static volatile unsigned int PageTable[MMU_PAGETABLE_NUM_ENTRY];

#elif defined(gcc)
static volatile unsigned int PageTable[MMU_PAGETABLE_NUM_ENTRY]
__attribute__((aligned(MMU_PAGETABLE_ALIGN_SIZE)));

#else
#error "Unsupported Compiler.\r\n"

#endif


void _MMUConfigAndEnable(void) {
	/*
	 ** Define DDR memory region of AM335x. DDR can be configured as Normal
	 ** memory with R/W access in user/privileged modes. The cache attributes
	 ** specified here are,
	 ** Inner - Write through, No Write Allocate
	 ** Outer - Write Back, Write Allocate
	 */
	REGION regionDdr = { MMU_PGTYPE_SECTION, START_ADDR_DDR, NUM_SECTIONS_DDR,
			MMU_MEMTYPE_NORMAL_NON_SHAREABLE(MMU_CACHE_WT_NOWA,
					MMU_CACHE_WB_WA), MMU_REGION_NON_SECURE,
			MMU_AP_PRV_RW_USR_RW, (unsigned int*) PageTable };
	/*
	 ** Define OCMC RAM region of AM335x. Same Attributes of DDR region given.
	 */
	REGION regionOcmc = { MMU_PGTYPE_SECTION, START_ADDR_OCMC,
			NUM_SECTIONS_OCMC,
			MMU_MEMTYPE_NORMAL_NON_SHAREABLE(MMU_CACHE_WT_NOWA,
					MMU_CACHE_WB_WA), MMU_REGION_NON_SECURE,
			MMU_AP_PRV_RW_USR_RW, (unsigned int*) PageTable };

	/*
	 ** Define Device Memory Region. The region between OCMC and DDR is
	 ** configured as device memory, with R/W access in user/privileged modes.
	 ** Also, the region is marked 'Execute Never'.
	 */
	REGION regionDev = { MMU_PGTYPE_SECTION, START_ADDR_DEV, NUM_SECTIONS_DEV,
			MMU_MEMTYPE_DEVICE_SHAREABLE, MMU_REGION_NON_SECURE,
			MMU_AP_PRV_RW_USR_RW | MMU_SECTION_EXEC_NEVER,
			(unsigned int*) PageTable };

	/* Initialize the page table and MMU */
	MMUInit((unsigned int*) PageTable);

	/* Map the defined regions */
	MMUMemRegionMap(&regionDdr);
	MMUMemRegionMap(&regionOcmc);
	MMUMemRegionMap(&regionDev);

	/* Now Safe to enable MMU */
	MMUEnable((unsigned int*) PageTable);
}


void boardInit(void){

	/* Configure and enable the MMU. */
	_MMUConfigAndEnable();

	/* Enable IRQ in CPSR.*/
    IntMasterIRQEnable();

    /* Initialize the ARM Interrupt Controller.*/
    IntAINTCInit();

    /* Enable all levels of Cache. */
	CacheEnable(CACHE_ALL);

    /* Initialize console for debugging  */
    ConsoleUtilsInit();
	ConsoleUtilsSetType(CONSOLE_UART);



	/* Registering DMTimerIsr */
	IntRegister(SYS_INT_TINT3, TimerIsrSEIZ);
	IntPrioritySet(SYS_INT_TINT3, 0, AINTC_HOSTINT_ROUTE_IRQ);
	IntSystemEnable(SYS_INT_TINT3);
	SEIZTimerSetup();

	/****************************************************
	 *		GPIO PINs initialization
	 ****************************************************/
	/* Enabling functional clocks for GPIO1 instance. */
	GPIO1ModuleClkConfig();
	/* Enabling the GPIO module. */
	GPIOModuleEnable(SOC_GPIO_1_REGS);
	/* Resetting the GPIO module. */
	GPIOModuleReset(SOC_GPIO_1_REGS);

	/* Set output modes */
	pinMode(CHAN0_PIN, PINS_1, OUTPUT);
	pinWrite(CHAN0_PIN, PINS_1, LOW);

	pinMode(CHAN1_PIN, PINS_1, OUTPUT);
	pinWrite(CHAN1_PIN, PINS_1, LOW);

	pinMode(CHAN2_PIN, PINS_1, OUTPUT);
	pinWrite(CHAN2_PIN, PINS_1, LOW);

	pinMode(CHAN3_PIN, PINS_1, OUTPUT);
	pinWrite(CHAN3_PIN, PINS_1, LOW);


	pinMode(OVF_PIN,  PINS_1, OUTPUT);
	pinWrite(OVF_PIN, PINS_1, LOW);

	pinMode(SEIZ_PIN, PINS_1, OUTPUT);
	pinWrite(SEIZ_PIN, PINS_1, LOW);

	pinMode(WORKING_LED, PINS_1, OUTPUT);
	pinWrite(WORKING_LED, PINS_1, LOW);

	pinMode(ERROR_LED, PINS_1, OUTPUT);
	pinWrite(ERROR_LED, PINS_1, LOW);


}

void pinMode(uint32_t pinNumber, uint32_t pinBank, uint8_t mode) {
	unsigned int dir;

	dir = (mode == INPUT) ? GPIO_DIR_INPUT : GPIO_DIR_OUTPUT;

	/* Enable pin */
	GPIODirModeSet(pinBank, pinNumber, dir);

}

void pinWrite(uint32_t pinNumber, uint32_t pinBank, uint8_t value) {
	// Copy of GPIOPinWrite
	if (GPIO_PIN_HIGH == value) {
		HWREG(pinBank + GPIO_SETDATAOUT) = pin2reg[pinNumber];
	} else {
		HWREG(pinBank + GPIO_CLEARDATAOUT) = pin2reg[pinNumber];
	}
}

#define OFFLED	1
#define LOCK	2
#define ONLED	3

static int timerSeizType=0;
static uint8_t *lockVar;
static int locktime=0;
static int delayTime=0;
static int offtime=0;

void SEIZTimerSetup(void) {

	DMTimer3ModuleClkConfig();

	DMTimerDisable(SOC_DMTIMER_3_REGS);

	DMTimerCounterSet(SOC_DMTIMER_3_REGS, 0);

	/* Configure the DMTimer for Auto-reload and compare mode */
	DMTimerModeConfigure(SOC_DMTIMER_3_REGS, DMTIMER_ONESHOT_NOCMP_ENABLE);

	/* Enable the DMTimer interrupts */
	DMTimerIntEnable(SOC_DMTIMER_3_REGS, DMTIMER_INT_OVF_EN_FLAG);

}


static void TimerIsrSEIZ(void) {


	DMTimerIntStatusClear(SOC_DMTIMER_3_REGS, DMTIMER_INT_OVF_EN_FLAG);
	DMTimerDisable(SOC_DMTIMER_3_REGS);

	switch (timerSeizType) {
		case ONLED:
			pinWrite(SEIZ_PIN, PINS_1, HIGH);
			DMTimerCounterSet(SOC_DMTIMER_3_REGS, TIMER_OVERFLOW - (offtime * TIMER_1MS_COUNT));
			DMTimerEnable(SOC_DMTIMER_3_REGS);
			timerSeizType = OFFLED;
//			ConsoleUtilsPrintf("Is ON\n");
			break;
		case OFFLED:
			pinWrite(SEIZ_PIN, PINS_1, LOW);
			DMTimerCounterSet(SOC_DMTIMER_3_REGS, TIMER_OVERFLOW - (locktime * TIMER_1MS_COUNT));
			DMTimerEnable(SOC_DMTIMER_3_REGS);
			timerSeizType = LOCK;
//			ConsoleUtilsPrintf("Is off\n");
			break;

		case LOCK:
//			ConsoleUtilsPrintf("Is unlocked\n");
			*lockVar = 0;
			break;

		default:

			ConsoleUtilsPrintf("undefined\n");
			pinWrite(SEIZ_PIN, PINS_1, LOW);
			*lockVar = 0;
			break;
	}


}



void EnableTimerSEIZ(int millisDelay, int millisOFF, int milliLOCK, const uint8_t *lockSeiz){

	offtime = millisOFF;
	locktime = milliLOCK;
	lockVar = lockSeiz;
	delayTime = millisDelay;


	if( lockSeiz == NULL ){
		pinWrite(ERROR_LED, PINS_1, HIGH );
		ConsoleUtilsPrintf("Error: pointer for lock var is NULL");
	}

	if( millisDelay == 0 ){
		//TURN LED ON IMMEDIATELY
		pinWrite(SEIZ_PIN, PINS_1, HIGH);
		timerSeizType = OFFLED;
		DMTimerCounterSet(SOC_DMTIMER_3_REGS, TIMER_OVERFLOW - (millisOFF * TIMER_1MS_COUNT));
		DMTimerEnable(SOC_DMTIMER_3_REGS);
	}
	else{
		//Wait for led to go on
		timerSeizType = ONLED;
		DMTimerCounterSet(SOC_DMTIMER_3_REGS, TIMER_OVERFLOW - (millisDelay * TIMER_1MS_COUNT));
		DMTimerEnable(SOC_DMTIMER_3_REGS);
	}

}





