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

#include "SD2_board.h"
#include "lightSensor.h"

#include "fsl_adc16.h"
#include "fsl_port.h"

/*==================[macros and typedef]=====================================*/
#define DEMO_ADC16_CHANNEL_GROUP    0U
#define DEMO_ADC16_USER_CHANNEL     3U /* PTE22*/

#define SENSOR_LUZ_PORT             PORTE
#define SENSOR_LUZ_PIN              22

/* TODO: DETERMINAR VALORES */
#define LOW_LEVEL                   990
#define HIGH_LEVEL                  4085
#define HISTERESIS					150
#define THRESHOLD					HIGH_LEVEL/4

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static adc16_channel_config_t adc16_channel;
static uint16_t adcProm;
static int32_t tim1ms;
static volatile uint16_t adcValue[5];
static volatile uint16_t i, k;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void ADC_Init(void)
{
    adc16_config_t adc16_config;

    PORT_SetPinMux(SENSOR_LUZ_PORT, SENSOR_LUZ_PIN, kPORT_PinDisabledOrAnalog);

    /*
     * adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceVref;
     * adc16ConfigStruct.clockSource = kADC16_ClockSourceAsynchronousClock;
     * adc16ConfigStruct.enableAsynchronousClock = true;
     * adc16ConfigStruct.clockDivider = kADC16_ClockDivider8;
     * adc16ConfigStruct.resolution = kADC16_ResolutionSE12Bit;
     * adc16ConfigStruct.longSampleMode = kADC16_LongSampleDisabled;
     * adc16ConfigStruct.enableHighSpeed = false;
     * adc16ConfigStruct.enableLowPower = false;
     * adc16ConfigStruct.enableContinuousConversion = false;
     */

    ADC16_GetDefaultConfig(&adc16_config);
    ADC16_Init(ADC0, &adc16_config);
    ADC16_EnableHardwareTrigger(ADC0, false); /* Make sure the software trigger is used. */

    adc16_channel.channelNumber = DEMO_ADC16_USER_CHANNEL;
    adc16_channel.enableInterruptOnConversionCompleted = true; /* Enable the interrupt. */
    adc16_channel.enableDifferentialConversion = false;

    NVIC_EnableIRQ(ADC0_IRQn);
}

static void ADC_IniciarConv(void)
{
    /*
     When in software trigger mode, each conversion would be launched once calling the "ADC16_ChannelConfigure()"
     function, which works like writing a conversion command and executing it. For another channel's conversion,
     just to change the "channelNumber" field in channel configuration structure, and call the function
     "ADC16_ChannelConfigure()"" again.
     Also, the "enableInterruptOnConversionCompleted" inside the channel configuration structure is a parameter for
     the conversion command. It takes affect just for the current conversion. If the interrupt is still required
     for the following conversion, it is necessary to assert the "enableInterruptOnConversionCompleted" every time
     for each command.
    */
    ADC16_SetChannelConfig(ADC0, DEMO_ADC16_CHANNEL_GROUP, &adc16_channel);
}


/*==================[external functions definition]==========================*/

extern void lightSensor_init(void)
{
    ADC_Init();
    for(k=0; k<5; k++) {
    	adcValue[k] = 0;
    }
}

extern lightSensor_level_enum lightSensor_getLevel(void)
{
	adcProm=0;

    for(k=0; k<5; k++) {
    	if(adcValue[k]==0) break;

    	adcProm += adcValue[k];
    }

	adcProm /= k;

	if (adcProm < THRESHOLD - HISTERESIS) {
		return LIGHT_SENSOR_LEVEL_HIGH;
	}
	else if (adcProm < THRESHOLD){
		return LIGHT_SENSOR_LEVEL_MED;
	} else{
	return LIGHT_SENSOR_LEVEL_LOW;
	}
}

extern void lightSensor_task1ms(void)
{
    if (tim1ms <= 0) {
        tim1ms = 100;			//Realiza conversión cada 100ms. Controlado por el Systick
        ADC_IniciarConv();
    } else
        tim1ms--;
}

void ADC0_IRQHandler(void)
{
	for(i = 4; i > 0; i--) {
		adcValue[i] = adcValue[i-1];
	}

	adcValue[0] = ADC16_GetChannelConversionValue(ADC0, DEMO_ADC16_CHANNEL_GROUP);
    /* Read conversion result to clear the conversion completed flag. */
}

/*==================[end of file]============================================*/
