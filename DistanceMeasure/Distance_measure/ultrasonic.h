/******************************************************************************
* File Name: ULTRASONIC.h
*
* Description: Header file for the ULTRASONIC driver
*******************************************************************************/

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_
#include "std_types.h"

#define ULTRASONIC_NUM_OF_EDGES               		2

#define ULTRASONIC_TRIGGER_PORT_ID                 PORTB_ID
#define ULTRASONIC_TRIGGER_PIN_ID                  PIN5_ID

void Ultrasonic_init(void);

void Ultrasonic_Trigger(void);

uint16 Ultrasonic_readDistance(void);

void Ultrasonic_edgeProcessing(void);


#endif /* ULTRASONIC_H_ */
