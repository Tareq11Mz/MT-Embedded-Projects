 /******************************************************************************
 *
 * Module: ADC
 *
 * File Name: adc.h
 *
 * Description: header file for the ATmega32 ADC driver
 *
 *
 *******************************************************************************/

#ifndef ADC_H_
#define ADC_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define ADC_MAXIMUM_VALUE    1023
#define ADC_REF_VOLT_VALUE   5

typedef enum { //refrence values for voltage
	AVCC,AREF,INTERNAL
}ADC_ReferenceVolatge;

typedef enum //Prescalar values
{
Prescalar_1,Prescalar_2,Prescalar_4,Prescalar_8,
Prescalar_16,Prescalar_32,Prescalar_64,Prescalar_128
}ADC_Prescaler;

typedef struct{
	ADC_ReferenceVolatge ref_volt;
	ADC_Prescaler prescaler;
}ADC_ConfigType;
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void ADC_init(const ADC_ConfigType * Configurable_Ptr);
uint16 ADC_readChannel(uint8 channel_number);
#endif /* ADC_H_ */
