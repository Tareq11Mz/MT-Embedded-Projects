#include "lcd.h"
#include "pwm.h"
#include "adc.h"
#include "dcMotor.h"
#include "lm35sensor.h"
int main()
{
    LCD_init();

    DcMotor_Init();
    DC_Motor_State state = stop;

    ADC_ConfigType ConfigType;
    ConfigType.prescaler = Prescalar_8;
    ConfigType.ref_volt = INTERNAL;
    ADC_ConfigType *Config_Ptr = &ConfigType;
    ADC_init(Config_Ptr);

    uint8 temperature = 0, Prev_temp = 0;
    while (1)
    {

        temperature = LM35_getTemperature(Config_Ptr);
        if (temperature != Prev_temp)
        {
            if (temperature < 30)

            {
                LCD_clearScreen();
                state = stop;
                DcMotor_Rotate(state, 0);
                LCD_moveCursor(0, 3);
                LCD_displayString("FAN is OFF");
                LCD_moveCursor(1, 3);
                LCD_displayString("TEMP = ");
                LCD_intgerToString(temperature);
                LCD_displayString(" C");
            }
            else if (temperature >= 30 && temperature < 60)
            {
                LCD_clearScreen();
                state = CLOCK_WISE;
                DcMotor_Rotate(state, 25);
                LCD_moveCursor(0, 3);
                LCD_displayString("FAN is ON");
                LCD_moveCursor(1, 3);
                LCD_displayString("TEMP = ");
                LCD_intgerToString(temperature);
                LCD_displayString(" C");
            }
            else if (temperature >= 60 && temperature < 90)
            {
                LCD_clearScreen();
                state = CLOCK_WISE;
                DcMotor_Rotate(state, 50);
                LCD_moveCursor(0, 3);
                LCD_displayString("FAN is ON");
                LCD_moveCursor(1, 3);
                LCD_displayString("TEMP = ");
                LCD_intgerToString(temperature);
                LCD_displayString(" C");
            }
            else if (temperature >= 90 && temperature < 120)
            {
                LCD_clearScreen();
                state = CLOCK_WISE;
                DcMotor_Rotate(state, 75);
                LCD_moveCursor(0, 3);
                LCD_displayString("FAN is ON");
                LCD_moveCursor(1, 3);
                LCD_displayString("TEMP = ");
                LCD_intgerToString(temperature);
                LCD_displayString(" C");
            }
            else
            {
                LCD_clearScreen();
                state = CLOCK_WISE;
                DcMotor_Rotate(state, 100);
                LCD_moveCursor(0, 3);
                LCD_displayString("FAN is ON");
                LCD_moveCursor(1, 3);
                LCD_displayString("TEMP = ");
                LCD_intgerToString(temperature);
                LCD_displayString(" C");
            }
        }
        Prev_temp = temperature;
    }
}
