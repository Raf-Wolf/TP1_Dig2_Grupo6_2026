#ifndef MEFPASOPEATON_H_
#define MEFPASOPEATON_H_

/*==================[inclusions]=============================================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

/*==================[external data declaration]==============================*/
typedef enum {
	EST_T_PASOPEATON_SIN_T = 0,
    EST_T_PASOPEATON_FIN
} estT_PasoPeaton_enum;

/*==================[external functions declaration]=========================*/
extern void mefPasoPeaton_init(void);
extern void mefPasoPeaton(void);
extern void mefPasoPeaton_task1ms(void);
extern estT_PasoPeaton_enum pasoPeaton_getT(void);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* MEFPASOPEATON_H_ */
