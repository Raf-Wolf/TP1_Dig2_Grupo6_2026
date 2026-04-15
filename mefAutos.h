#ifndef MEFAUTOS_H_
#define MEFAUTOS_H_

/*==================[inclusions]=============================================*/

#include "SD2_board.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
extern void mefAutos_init(void);
extern void mefAutos(void);
extern void mefAutos_task1ms(void);
extern uint16_t getAutos(void);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* MEFAUTOS_H_ */
