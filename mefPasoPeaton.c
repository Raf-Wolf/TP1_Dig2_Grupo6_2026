/*
###############################################################################
#
# Copyright 2023, Gustavo Muro
# All rights reserved
#
# This file is part of SISTEMAS DIGITALES 2 - FCEIA - UNR.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from this
#    software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#                                                                             */

/*==================[inclusions]=============================================*/
#include "mefPasoPeaton.h"
#include "stdint.h"
#include "lightSensor.h"
#include "SD2_board.h"

/*==================[macros and typedef]=====================================*/
typedef enum {
	EST_MEF_INIT = 0,
    EST_MEF_LVR_Titila,
	EST_MEF_Paso_Secundario,
	EST_MEF_LVS_Titila
} estMef_enum;

#define T1M  	1000 * 60
#define T10S  	10000
#define T200mS 	200

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static estMef_enum estMef;
static int32_t tim1ms, tim1ms_2;
static bool TM1_ft, TM2_ft;
static estT_PasoPeaton_enum Trans;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static void T1M_ST(void) {
	tim1ms = T1M;
	TM1_ft = false;
}

static void T10S_ST(void) {
	tim1ms = T10S;
	TM1_ft = false;
}

static void T200mS_ST(void) {
	tim1ms_2 = T200mS;
	TM2_ft = false;
}


/*==================[external functions definition]==========================*/

extern void mefPasoPeaton_init(void)
{
    estMef = EST_MEF_INIT;
    Trans = EST_T_PASOPEATON_SIN_T;
}

extern void mefPasoPeaton(void)
{
    switch (estMef)
    {
        case EST_MEF_INIT:
            board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_ON);
            board_setLed(BOARD_LED_ID_ROJO,  BOARD_LED_MSG_OFF);
            board_setLed(BOARD_LED_ID_VERDE_EXT, BOARD_LED_MSG_OFF);
            board_setLed(BOARD_LED_ID_ROJO_EXT,  BOARD_LED_MSG_ON);

            T10S_ST();
            T200mS_ST();

            estMef = EST_MEF_LVR_Titila;

            break;

        case EST_MEF_LVR_Titila:

        	if((TM2_ft) && (!TM1_ft)) {
				T200mS_ST();
				board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_TOGGLE);
			}

        	if(TM1_ft) {

				T1M_ST();

	            board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_OFF);
	            board_setLed(BOARD_LED_ID_ROJO,  BOARD_LED_MSG_ON);
	            board_setLed(BOARD_LED_ID_VERDE_EXT, BOARD_LED_MSG_ON);
	            board_setLed(BOARD_LED_ID_ROJO_EXT,  BOARD_LED_MSG_OFF);

				estMef = EST_MEF_Paso_Secundario;
			}
            break;

        case EST_MEF_Paso_Secundario:

        	if(TM1_ft) {

				T10S_ST();
				T200mS_ST();

				estMef = EST_MEF_LVS_Titila;
			}
        	break;

        case EST_MEF_LVS_Titila:

        	if((TM2_ft) && (!TM1_ft)) {
				T200mS_ST();
				board_setLed(BOARD_LED_ID_VERDE_EXT, BOARD_LED_MSG_TOGGLE);
			}
        	if(TM1_ft) {

        		board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_ON);
				board_setLed(BOARD_LED_ID_ROJO,  BOARD_LED_MSG_OFF);
				board_setLed(BOARD_LED_ID_VERDE_EXT, BOARD_LED_MSG_OFF);
				board_setLed(BOARD_LED_ID_ROJO_EXT,  BOARD_LED_MSG_ON);

				Trans = EST_T_PASOPEATON_FIN;
			}

        	break;
    }
}

extern void mefPasoPeaton_task1ms(void)
{
	if (tim1ms)
		tim1ms--;
	else {
		TM1_ft=true;
	}
	if (tim1ms_2)
		tim1ms_2--;
	else {
		TM2_ft=true;
	}
}

extern estT_PasoPeaton_enum pasoPeaton_getT(void) {
	return Trans;
}

/*==================[end of file]============================================*/
