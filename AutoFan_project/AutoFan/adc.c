 /******************************************************************************
 * File Name: adc.c
 * ADC driver
 *******************************************************************************/
#include "avr/io.h"
#include "adc.h"
#include "common_macros.h"
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void ADC_init(const ADC_ConfigType * Configurable_Ptr)
{
	/* ADMUX Register Bits Description:
	 * ADLAR   = 0 right adjusted
	 * MUX4:0  = 00000 to choose channel 0 as initialization
	 */
	ADMUX = 0;
	if (Configurable_Ptr->ref_volt==INTERNAL)//REFS0 =1 ,REFS1 =1 for internal VREF=2.56
	ADMUX |= (1<<REFS1)|(1<<REFS0);

	else if (Configurable_Ptr->ref_volt==AREF)//REFS0 =0 ,REFS1 =0 for EXternal AREF
		ADMUX =0;
	else if (Configurable_Ptr->ref_volt==AVCC)//REFS0 =1  for EXternal AREF
		ADMUX |= (1<<REFS0);


	/* ADCSRA Register Bits Description:
	 *
	 * ADEN    = 1 Enable ADC
	 *
	 * ADATE   = 0 Disable Auto Trigger
	 *
	 * ADIE    = 0 Disable ADC Interrupt
	 *
	 * ADPS2:0 = 011 to choose ADC_Clock=F_CPU/8=1Mhz/8=125Khz --> ADC must operate in range 50-200Khz
	 */
	ADCSRA |= (1<<ADEN);

	/*
	 * configuring pre-scaler
	 */
	if(Configurable_Ptr->prescaler==Prescalar_1)
		ADCSRA|=Prescalar_1;
	else if (Configurable_Ptr->prescaler==Prescalar_2)
		ADCSRA|=Prescalar_2;
	else if (Configurable_Ptr->prescaler==Prescalar_4)
			ADCSRA|=Prescalar_4;
	else if (Configurable_Ptr->prescaler==Prescalar_8)
			ADCSRA|=Prescalar_8;
	else if (Configurable_Ptr->prescaler==Prescalar_16)
			ADCSRA|=Prescalar_16;
	else if (Configurable_Ptr->prescaler==Prescalar_32)
			ADCSRA|=Prescalar_32;
	else if (Configurable_Ptr->prescaler==Prescalar_64)
			ADCSRA|=Prescalar_64;
	else if (Configurable_Ptr->prescaler==Prescalar_128)
			ADCSRA|=Prescalar_128;
}

uint16 ADC_readChannel(uint8 channel_number)
{
	channel_number &= 0x07; /* channel number must be from 0 to 7 (000 to 111) */
	ADMUX &= 0xE0; /* Clearing the first 5 bits in the ADMUX as channel number MUX4:0 bits */
	ADMUX = ADMUX | channel_number; /*setting the channel number in MUX4:0 bits */
	SET_BIT(ADCSRA,ADSC); /* Starting conversion setting 1 to ADSC, bit no.4 */
	//ADSC returns to 0 auto by itself
	while(BIT_IS_CLEAR(ADCSRA,ADIF)); /* polling for conversion to complete, ADIF set to 1 and ADSC cleared */
	SET_BIT(ADCSRA,ADIF); /* Clearing ADIF by writing 1 to it */
	return ADC; /* Returning the Digital value */
}
