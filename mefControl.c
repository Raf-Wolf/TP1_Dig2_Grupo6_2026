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
#include "mefControl.h"
#include "stdint.h"
#include "lightSensor.h"
#include "SD2_board.h"

/*==================[macros and typedef]=====================================*/
typedef enum {
    EST_MEF_INIT = 0,
    EST_MEF_Luces_OFF,
    EST_MEF_Luces_ON,
    EST_MEF_Espera,
} estMef_enum;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static estMef_enum estMef;
static int32_t tim1ms;
static bool T5S_ft;


/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
void T5S_ST() {
	tim1ms = 5000;
	T5S_ft = false;
}

void SALIDA_LUMINARIA_ON(){
	board_setLed(BOARD_LED_ID_LUMINARIA, BOARD_LED_MSG_ON);
}

void SALIDA_LUMINARIA_OFF() {
	board_setLed(BOARD_LED_ID_LUMINARIA, BOARD_LED_MSG_OFF);
}

/*==================[external functions definition]==========================*/

extern void mefControl_init(void)
{
    estMef = EST_MEF_INIT;
}

extern void mefControl(void)
{
    switch (estMef)
    {
        case EST_MEF_INIT:

            SALIDA_LUMINARIA_OFF();

            estMef = EST_MEF_Luces_OFF;

            break;

        case EST_MEF_Luces_OFF:

            if(lightSensor_getLevel() == LIGHT_SENSOR_LEVEL_LOW) {
                SALIDA_LUMINARIA_ON();
                estMef = EST_MEF_Luces_ON;
            }

			break;

		case EST_MEF_Luces_ON:

            if(lightSensor_getLevel() == LIGHT_SENSOR_LEVEL_HIGH) {
                T5S_ST();
                estMef = EST_MEF_Espera;
            }

			break;

		case EST_MEF_Espera:

			if((lightSensor_getLevel() == LIGHT_SENSOR_LEVEL_LOW) && (!T5S_ft))
                estMef = EST_MEF_Luces_ON;
            if(T5S_ft) {
                SALIDA_LUMINARIA_OFF();
                estMef = EST_MEF_Luces_OFF;
            }

			break;
    }
}

extern void mefControl_task1ms(void)
{
	lightSensor_task1ms();

    if (tim1ms)
        tim1ms--;
    else
    	T5S_ft = true;

}


/*==================[end of file]============================================*/
