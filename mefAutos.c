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
#include "mefAutos.h"
#include "stdint.h"
#include "SD2_board.h"
#include "key.h"

/*==================[macros and typedef]=====================================*/
typedef enum {
	EST_MEF_INIT = 0,
    EST_MEF_Incremento,
    EST_MEF_Decremento
} estMef_enum;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static estMef_enum estMef;
static uint16_t Autos;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

extern void mefAutos_init(void)
{
    estMef = EST_MEF_INIT;
}

extern void mefAutos(void)
{
    switch (estMef)
    {
        case EST_MEF_INIT:

			Autos = 0;
			estMef = EST_MEF_Incremento;

            break;

        case EST_MEF_Incremento:

        	if(key_getPressEv(BOARD_SW_ID_3)) {
        		Autos++;
        	}

        	if(get_LRS() == false) {
        		estMef = EST_MEF_Decremento;
        	}

            break;
        case EST_MEF_Decremento:

        	if((Autos > 0) && (key_getPressEv(BOARD_SW_ID_3))) {
        		Autos--;
        	}

        	if(get_LRS() == true) {
        		estMef = EST_MEF_Incremento;
        	}

        	break;
    }
}

extern void mefAutos_task1ms(void)
{

}

extern uint16_t getAutos(void) {
	return Autos;
}

/*==================[end of file]============================================*/
