#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "fsl_adc16.h"
char pot(int canal);
int caso(int var);

void delay()
{
	volatile uint32_t ui32Loop = 0xFFFFF;
	while (ui32Loop > 0)
	{
		ui32Loop--;
	}
}

int main(void) {

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    BOARD_InitDebugConsole();
#endif

    adc16_config_t adc16ConfigStruct;	//Configura ADC
    adc16_channel_config_t adc16ChannelConfigStruct[2];	//Configura canal de ADC
    ADC16_GetDefaultConfig(&adc16ConfigStruct);
    adc16ConfigStruct.resolution = kADC16_ResolutionSE16Bit; //establece la resolucion de salida
    ADC16_Init(ADC0, &adc16ConfigStruct); //inicializa ADC0
    ADC16_EnableHardwareTrigger(ADC0, false);
    ADC16_DoAutoCalibration(ADC0); //calibra el ADC
    adc16ChannelConfigStruct[0].channelNumber = 0;	//canal 0
    adc16ChannelConfigStruct[0].enableInterruptOnConversionCompleted = false;
    adc16ChannelConfigStruct[0].enableDifferentialConversion = false;
    adc16ChannelConfigStruct[1].channelNumber = 3;	//canal 3
    adc16ChannelConfigStruct[1].enableInterruptOnConversionCompleted = false;
    adc16ChannelConfigStruct[1].enableDifferentialConversion = false;
    int salida1, salida2, variable, var;

    while(1)
    {
    	delay();
    	ADC16_SetChannelConfig(ADC0, 0, &adc16ChannelConfigStruct[0]);
    	while (0U == (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(ADC0, 0))){}
    	salida1 = ADC16_GetChannelConversionValue(ADC0, 0);//Potenciometro

    	ADC16_SetChannelConfig(ADC0, 0, &adc16ChannelConfigStruct[1]);
    	while (0U == (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(ADC0, 0))){}
    	salida2 = ADC16_GetChannelConversionValue(ADC0, 0);//sensores

		var = pot(salida1);
		variable = caso(var);

    	if(salida2 < (variable+2699) && salida1 > variable)
    		GPIO_SetPinsOutput(GPIOE, 0x200000);
    	else
    		GPIO_ClearPinsOutput(GPIOE, 0x200000);

		PRINTF("Potenciometro:%d Sensores: %d\r\n", salida1, salida2);
    }
}

char pot(int canal){
	char i;
	if(canal < 1600  && canal > 0) i = 0;
	if(canal < 4600  && canal > 1600)i = 1;
	if(canal < 9200  && canal > 4600) i = 2;
	if(canal < 13800 && canal > 9200) i = 3;
	if(canal < 18400 && canal > 13800)i = 4;
	if(canal < 23000 && canal > 18400)i = 5;
	if(canal < 27600 && canal > 23000)i = 6;
	if(canal < 32200 && canal > 27600)i = 7;
	if(canal < 36800 && canal > 32200)i = 8;
	if(canal < 41400 && canal > 36800)i = 9;
	if(canal < 46000 && canal > 41400)i = 10;
	if(canal < 50600 && canal > 46000)i = 11;
	if(canal < 55200 && canal > 50600)i = 12;
	if(canal < 59800 && canal > 55200)i = 13;
	if(canal < 64400 && canal > 59800)i = 14;
	return i;
}

int caso(int var){
	 int variable;
	 switch(var)
	 {
	 		case 0:  variable = 1100;
	 		break;
	 		case 1:  variable = 3800;
	 		break;
	 		case 2:  variable = 6500;
	 		break;
	 		case 3:  variable = 9200;
	 		break;
	 		case 4:  variable = 11900;
	 		break;
	 		case 5:  variable = 14600;
	 		break;
	 		case 6:  variable = 17300;
	 		break;
	 		case 7:  variable = 20000;
	 		break;
	 		case 8:  variable = 22700;
	 		break;
	 		case 9:  variable = 25400;
	 		break;
	 		case 10: variable = 28100;
	 		break;
	 		case 11: variable = 30800;
	 		break;
	 		case 12: variable = 33500;
	 		break;
	 		case 13: variable = 36200;
	 		break;
	 		case 14: variable = 38900;
	 		break;
	 }
	 return variable;
}


