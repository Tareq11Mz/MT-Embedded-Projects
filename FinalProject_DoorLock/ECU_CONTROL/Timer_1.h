 /******************************************************************************
 *
 * Module: Timer
 *
 * File Name: Timer_1.h
 *
 * Description: h file Timer1
 *
 *******************************************************************************/

#ifndef TIMER_1_H_
#define TIMER_1_H_
#include "std_types.h"

//uint8 g_OverflowsPerMinute;

/*using pre-scaler 256*/
#define OVERFLOWS_PER_MINUE_NORMAL_MODE 29
/*for the simulation to be accurate the number of ticks can be around 25000 ticks*/
#define TICKS_PER_ONE_SECOND            31249


/*Clock select first three bits values for the pre-scaler in TCCR1B register*/
typedef enum {
	no_clock,
	prescaler_1,
	prescaler_8,
	prescaler_64,
	prescaler_256,
	prescaler_1024
} Timer1_Prescaler;
/*
 * WGM10, WGM11 and WGM12 values that select the mode for timer1
 * WGM10 and WGM11 bit 0 1 in TCCR1A register. WGM12 bit 3 in TCCR1B register
 * timer1 here supports two modes: normal mode and CTC mode
 */
typedef enum {
	normal_mode, CTC_mode = 4
} Timer1_Mode;

typedef struct {
	uint16 initial_value;
	uint16 compare_value; // it will be used in compare mode only.
	Timer1_Prescaler prescaler;
	Timer1_Mode mode;
} Timer1_ConfigType;
/*
 * Description:
 *      set TCNT1, OCR1A, pre-scaler and timer1 mode
*/
void Timer1_init(const Timer1_ConfigType * Config_Ptr);
/*
 * Description:
 *      clears all timer1 registers
*/
void Timer1_deInit(void);
/*
 * Description:
 *      set the call back function
*/
void Timer1_setCallBack(void(*a_ptr)(void));

#endif /* TIMER_1_H_ */
