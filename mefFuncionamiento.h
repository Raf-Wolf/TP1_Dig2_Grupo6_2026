#ifndef MEFFUNCIONAMIENTO_H_
#define MEFFUNCIONAMIENTO_H_

/*==================[inclusions]=============================================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

/*==================[external data declaration]==============================*/
typedef enum {
	EST_T_SIN_T = 0,
    EST_T_PASOPEATON,
    EST_T_ESPERASEC
} estT_Funcionamiento_enum;

/*==================[external functions declaration]=========================*/
extern void mefFuncionamiento_init(void);
extern void mefFuncionamiento(void);
extern void mefFuncionamiento_task1ms(void);
extern estT_Funcionamiento_enum funcionamiento_getT(void);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* MEFFUNCIONAMIENTO_H_ */
