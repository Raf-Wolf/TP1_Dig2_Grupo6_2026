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
#include "mefModo.h"
#include "mefFuncionamiento.h"
#include "mefPasoPeaton.h"
#include "mefEsperaSec.h"
#include "stdint.h"
#include "lightSensor.h"
#include "SD2_board.h"

/*==================[macros and typedef]=====================================*/
typedef enum {
	EST_MEF_INIT = 0,
    EST_MEF_Funcionamiento_Habitual,
    EST_MEF_Paso_Peaton,
	EST_MEF_Espera_Secundario
} estMef_enum;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static estMef_enum estMef;
static int32_t tim1ms;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

extern void mefModo_init(void)
{
	mefFuncionamiento_init();
	mefPasoPeaton_init();
	mefEsperaSec_init();
    estMef = EST_MEF_INIT;
}

extern void mefModo(void)
{
    switch (estMef)
    {
        case EST_MEF_INIT:
            board_setLed(BOARD_LED_ID_VERDE, BOARD_LED_MSG_ON);
            board_setLed(BOARD_LED_ID_ROJO,  BOARD_LED_MSG_OFF);
            board_setLed(BOARD_LED_ID_VERDE_EXT, BOARD_LED_MSG_OFF);
            board_setLed(BOARD_LED_ID_ROJO_EXT,  BOARD_LED_MSG_ON);

            estMef = EST_MEF_Funcionamiento_Habitual;

            break;

        case EST_MEF_Funcionamiento_Habitual:
    	    mefFuncionamiento();

    	    if(funcionamiento_getT() == EST_T_PASOPEATON) {
    	    	mefPasoPeaton_init();
    	    	estMef = EST_MEF_Paso_Peaton;
    	    }

    	    if(funcionamiento_getT() == EST_T_ESPERASEC) {
    	    	mefEsperaSec_init();
    	    	estMef = EST_MEF_Espera_Secundario;
    	    }

            break;
        case EST_MEF_Paso_Peaton:
    	    mefPasoPeaton();

    	    if(pasoPeaton_getT() == EST_T_PASOPEATON_FIN) {
    	    	estMef = EST_MEF_Funcionamiento_Habitual;
    	    }
        	break;
        case EST_MEF_Espera_Secundario:
    	    mefEsperaSec();

    	    if(esperasec_getT() == EST_T_ESPERASEC_FIN) {
    	    	mefFuncionamiento_init();
    	    	estMef = EST_MEF_Funcionamiento_Habitual;
    	    }
        	break;
    }
}

extern void mefModo_task1ms(void)
{
	switch (estMef)
	{
		case EST_MEF_INIT:
		break;

		case EST_MEF_Funcionamiento_Habitual:
			mefFuncionamiento_task1ms();
			break;
		case EST_MEF_Paso_Peaton:
			mefPasoPeaton_task1ms();
			break;
		case EST_MEF_Espera_Secundario:
			mefEsperaSec_task1ms();
			break;
	}

    if (tim1ms)
        tim1ms--;
}


/*==================[end of file]============================================*/
