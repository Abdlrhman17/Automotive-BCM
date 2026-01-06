
#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_

#include "StdTypes.h"

typedef enum
{
	Channel_0 = 0,
	Channel_1,
	Channel_2,
	Channel_3,
	Channel_4,
	Channel_5,
	Channel_6,
	Channel_7,

}ADC_Channel_type;


typedef enum
{
	ADC_PRESCALER_2 = 1,
	ADC_PRESCALER_4,
	ADC_PRESCALER_8,
	ADC_PRESCALER_16,
	ADC_PRESCALER_32,
	ADC_PRESCALER_64,
	ADC_PRESCALER_128,
	
}ADC_Prescaler_set_type;

typedef enum
{
	VREF_AREF = 0,
	VREF_AVCC,
	VREF_256
}ADC_Vref_type;


void ADC_Init(ADC_Vref_type vref, ADC_Prescaler_set_type scaler);


u16 ADC_Read(ADC_Channel_type chnl);


u16 ADC_ReadVoltage(ADC_Channel_type chnl);


u16 ADC_ReadNoBlock(void);


#endif /* ADC_INTERFACE_H_ */