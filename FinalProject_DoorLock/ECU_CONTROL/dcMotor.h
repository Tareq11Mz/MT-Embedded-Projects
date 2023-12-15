/******************************************************************************
 * File Name: dc-motor.h
 * Description: DC MOTOR deriver
 *******************************************************************************/
#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include "gpio.h"

#define MOTOR_PORT_ID PORTA_ID
#define MOTOR_PIN0_ID PIN0_ID
#define MOTOR_PIN1_ID PIN1_ID



typedef enum
{
    stop,
    CLOCK_WISE,
    ANTI_CLOCK_WISE
} DC_Motor_State;

//prototypes
void DcMotor_Init(void);

void DcMotor_Rotate(DC_Motor_State state, uint8 speed);

#endif /* DCMOTOR_H_ */
