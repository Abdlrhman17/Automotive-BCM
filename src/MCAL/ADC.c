#include "StdTypes.h"
#include "atmega32_mem_map.h"
#include "Utils.h"
#include "ADC_interface.h"

void ADC_Init(ADC_Vref_type vref, ADC_Prescaler_set_type scaler)
{
	/*PRESCALER*/
	scaler &= 0x07;
	ADCSRA &= 0xF8;
	ADCSRA |= scaler;
	
	/*VREF*/
	switch (vref)
	{
		case VREF_AREF:
		CLR_BIT(ADMUX,REFS0);
		CLR_BIT(ADMUX,REFS1);
		break;
		
		case VREF_AVCC:
		SET_BIT(ADMUX,REFS0);
		CLR_BIT(ADMUX,REFS1);
		break;
		
		case VREF_256:
		SET_BIT(ADMUX,REFS0);
		SET_BIT(ADMUX,REFS1);
		break;

	}
	
	/*CONVERSION READING*/
	CLR_BIT(ADMUX,ADLAR);  /*NOT left shifted*/
	
	/*ADC ENABLE*/
	SET_BIT(ADCSRA,ADEN);
}


u16 ADC_Read(ADC_Channel_type chnl)
{
	u16 ADC_READ;
	u8  ADC_L, ADC_H;
	
	/*SELECT CHANNEL*/
	ADMUX &= 0xE0;
	ADMUX |= chnl;
	
	/*START CONVERSION*/
	SET_BIT(ADCSRA,ADSC);
	
	/*W8 TO FINISH*/
	while(GET_BIT(ADCSRA,ADSC));
	
	/*GET READ*/
	ADC_L = ADCL;
	ADC_H = ADCH;
	
	ADC_READ = (ADC_H << 8) | ADC_L; /*OR Read the Reg as a short*/
	
	return ADC_READ; 
}



void ADC_StartConversion(ADC_Channel_type chnl)
{
	ADMUX &= 0x0E;
	ADMUX |= chnl;
	SET_BIT(ADCSRA,ADSC);
}
 
 
u16 ADC_GetRead(void)
{
	while(GET_BIT(ADCSRA,ADSC));
	return ADC;
}


u8 ADC_ReadPeriodic(u16* pdata)
{
	if(!GET_BIT(ADCSRA,ADSC))
	{
		*pdata = ADC;
		return 1;
	}
	return 0;
}


u16 ADC_ReadNoBlock(void)
{
	return ADC;
}

u16 ADC_ReadVoltage(ADC_Channel_type chnl)/*RETURNS VOLTAGE IN mV*/
{
	u16 volt = (((u32)ADC_Read(chnl) * 5000) / 1023);
	return volt;
}
