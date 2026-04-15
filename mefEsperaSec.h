#ifndef MEFESPERASEC_H_
#define MEFESPERASEC_H_

/*==================[inclusions]=============================================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

/*==================[external data declaration]==============================*/

typedef enum {
	EST_T_ESPERASEC_SIN_T = 0,
    EST_T_ESPERASEC_FIN
} estT_EsperaSec_enum;

/*==================[external functions declaration]=========================*/
extern void mefEsperaSec_init(void);
extern void mefEsperaSec(void);
extern void mefEsperaSec_task1ms(void);
extern estT_EsperaSec_enum esperasec_getT(void);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* MEFESPERASEC_H_ */
