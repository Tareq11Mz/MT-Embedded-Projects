 /******************************************************************************
 * File Name: ultrasonic.c
 *
 * Description: Source file for the ULTRASONIC driver
 *******************************************************************************/
#include "ultrasonic.h"
#include "icu.h"
#include "common_macros.h"
#include "gpio.h"
#include "std_types.h"
#include <util/delay.h>
#include <math.h>



uint8 g_edgeCount = 0;
uint16 g_timeHigh = 0;
uint16 g_distance_cm = 0;


void Ultrasonic_edgeProcessing(void){
	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		Icu_clearTimerValue();
		/* Detect falling edge */
		Icu_setEdgeDetectionType(FALLING);
	}
	else if(g_edgeCount == 2)
	{
		/* Store the High time value */
		g_timeHigh = Icu_getInputCaptureValue();
		g_distance_cm = g_timeHigh / 58;
		g_edgeCount = 0;
		Icu_setEdgeDetectionType(RISING);
	}
}

void Ultrasonic_init(){
	/* initializing ICU */
	Icu_ConfigType icuConfig = {F_CPU_8,RISING};
	Icu_init(&icuConfig);
	Icu_setCallBack(Ultrasonic_edgeProcessing);

	/* setting PB5 (triger pin) as output pin */
	GPIO_setupPinDirection(PORTB_ID, PIN5_ID, PIN_OUTPUT);
}


void Ultrasonic_Trigger(void){
	GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_HIGH);
	_delay_us(10);
	GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_LOW);
}

uint16 Ultrasonic_readDistance(void){
	Ultrasonic_Trigger();
	return g_distance_cm;
}

