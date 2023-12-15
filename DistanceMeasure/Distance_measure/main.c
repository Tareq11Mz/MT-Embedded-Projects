#include "common_macros.h"
#include "ultrasonic.h"
#include "std_types.h"
#include "lcd.h"
#include "gpio.h"
#include "avr/io.h"
int main()
{
	LCD_init();
	Ultrasonic_init();
	LCD_displayString("DISTANCE= ");
	SET_BIT(SREG,7);
	while(1)
	{
		uint16 distance=Ultrasonic_readDistance();
		LCD_moveCursor(0, 12);
		LCD_intgerToString(distance);
	}
	return 0;


}

