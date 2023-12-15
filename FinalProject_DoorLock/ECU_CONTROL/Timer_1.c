 /******************************************************************************
 *
 * Module: Timer
 *
 * File Name: Timer_1.c
 *
 * Description: c file Timer1
 *
 *******************************************************************************/
#include "Timer_1.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static volatile void(*g_callBackPtr)(void)=NULL_PTR;

/*
 * Timer1 is used to count minutes in overflow mode.
 * after one minute(32 overflows) in normal mode call the call back function
 */
ISR(TIMER1_OVF_vect) {
	if (g_callBackPtr != NULL_PTR)
		g_callBackPtr();

}
/*
 * Timer1 is used to count seconds in CTC mode.
 * ORCR1A = TICKS_PER_ONE_SECOND
 */
ISR(TIMER1_COMPA_vect){
	if (g_callBackPtr!=NULL_PTR)
		g_callBackPtr();
}

void Timer1_init(const Timer1_ConfigType *Config_Ptr) {
	TCNT1 = Config_Ptr->initial_value;
	if (Config_Ptr->mode == normal_mode){
		TIMSK |= (1<<TOIE1);
	}
	/*compare value is only used in compare mode*/
	if (Config_Ptr->mode == CTC_mode){
		OCR1A = Config_Ptr->compare_value;
		TIMSK |= (1<<OCIE1A);
	}
	/*
	 * set the pre-scaler value.
	 * set WGM12 value.
	 * */
	TCCR1B = (Config_Ptr->prescaler) | ((Config_Ptr->mode<<1)&0x08);
	/*set WGM10 and WGM11 values.*/
	TCCR1A =  (1<<FOC1A) | (1<<FOC1B) |(1<<COM1A1) ;

}
void Timer1_setCallBack(void(*a_ptr)(void)){
	g_callBackPtr = a_ptr;
}


/*clear all timer1 registers to disable it*/
void Timer1_deInit(void) {
	TCNT1 = 0;
	OCR1A = 0;
	TCCR1B = 0;
	TCCR1A = 0;
}
