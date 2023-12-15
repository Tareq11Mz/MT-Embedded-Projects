 /******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: Buzzer.h
 *
 * Description: h file buzzer
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_
#include "std_types.h"
#include "GPIO.h"

#define BUZZER_PORT PORTC_ID
#define BUZZER_PIN PIN4_ID
void Buzzer_init();

/* Description:
 *    Function to enable the Buzzer
*/
void Buzzer_on(void);

/* Description:
 *    Function to disable the Buzzer
 *    */
void Buzzer_off(void);

#endif /* BUZZER_H_ */
