/*
* File Name: dc-motor.c
*
* Description: DC MOTOR driver
*
*/
#include "dcMotor.h"

#include "pwm.h"
#include "std_types.h"

void DcMotor_Init(void)
{
    GPIO_setupPinDirection(MOTOR_PORT_ID, MOTOR_PIN0_ID, PIN_OUTPUT);
    GPIO_setupPinDirection(MOTOR_PORT_ID, MOTOR_PIN1_ID, PIN_OUTPUT);
    GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN0_ID, LOGIC_LOW);
    GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN1_ID, LOGIC_LOW);
}

void DcMotor_Rotate(DC_Motor_State state, uint8 speed)
{
    if (stop == state)
    {
        GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN0_ID, LOGIC_LOW);
        GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN1_ID, LOGIC_LOW);
    }
    else if (CLOCK_WISE == state)
    {
        GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN0_ID, LOGIC_HIGH);
        GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN1_ID, LOGIC_LOW);
        PWM_Timer0_Start(speed);
    }

    else if (ANTI_CLOCK_WISE == state)
    {
        GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN0_ID, LOGIC_LOW);
        GPIO_writePin(MOTOR_PORT_ID, MOTOR_PIN1_ID, LOGIC_HIGH);
        PWM_Timer0_Start(speed);
    }
}
