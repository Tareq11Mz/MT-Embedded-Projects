 /******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: Buzzer.c
 *
 * Description: c file buzzer
 *
 *******************************************************************************/

#include "buzzer.h"

void Buzzer_init(){
	/*
	 * set up pin direction of the buzzer
	 * set it off initially
	*/
	GPIO_setupPinDirection(BUZZER_PORT, BUZZER_PIN, PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);

}
void Buzzer_on(void){
	/*Turn on the Buzzer*/
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_HIGH);
}
void Buzzer_off(void){
	/*Turn off the Buzzer*/
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);

}
