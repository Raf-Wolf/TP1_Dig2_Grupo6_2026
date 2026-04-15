/*==================[inclusions]=============================================*/

#include "SD2_board.h"
#include "board.h"
#include "mefModo.h"
#include "mefControl.h"
#include "mefAutos.h"
#include "key.h"
#include "lightSensor.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal functions definition]==========================*/

int main(void) {

    BOARD_BootClockRUN();
    board_init();
	BOARD_InitDebugConsole();

	key_init();

	/* systick cada 1 ms */
	SysTick_Config(SystemCoreClock / 1000U);

	mefModo_init();
	mefControl_init();
	mefAutos_init();
	lightSensor_init();

	while (1)
	{
	    mefModo();
	    mefControl();
	    mefAutos();
	}
}

void SysTick_Handler(void)
{
	key_periodicTask1ms();

	mefModo_task1ms();

	mefControl_task1ms();

	mefAutos_task1ms();
}
