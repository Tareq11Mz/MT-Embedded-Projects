#include <avr/io.h>
#include <util/delay.h>
#include "UART.h"
#include "Timer_1.h"
#include "lcd.h"
#include "keypad.h"

/*commands used to synch between HMI_ECU and CONTROL_ECU*/
#define CHANGE_PASSWORD '-'
#define OPEN_DOOR '+'
#define ENTER '='
#define PASSWORDS_NOT_MATCH 0xFF
#define PASSWORDS_MATCH 0xF1
#define PASSWORD_1 0x02
#define PASSWORD_2 0x03
#define PASSWORD_ADDRESS_LOCATION 0x50
#define PASSWORD_MATCH_ADDRESS_LOCATION 0x60
#define WRONG_PASS_3 0x20

uint8 g_staticPassword[6];
uint8 g_receivedByte;
uint8 g_countSeconds = 0;

/* TIMER1 configurations:
 *		initial_value = 0,
 *		compare_value = 31250 ticks per 1 second,
 *		pre-scaler = 256
 *		CTC mode of operation
 */
Timer1_ConfigType timer1_config_ptr = { 0, TICKS_PER_ONE_SECOND, prescaler_256,
		CTC_mode };

/* Description:
 * 	   take uint8 value and return true if it is a number
 */
uint8 isNumber(uint8 value) {
	return (value >= 0 && value <= 9);
}

/* Description:
 * 	   take password and re-entered password from the user through keypad
 * 	   receive byte from CONTROL_ECU to know if passwords match or not
 */
void TakePassword(void) {
	uint8 i = 0;
	uint8 key;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "enter password:");
	LCD_displayStringRowColumn(1, 0, "5 digits only");
	_delay_ms(1000);
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "enter password:");
	LCD_moveCursor(1, 0);

	while (i < 5) {

		key = KEYPAD_getPressedKey();
		if (isNumber(key)) {
			LCD_displayCharacter('*');
			UART_sendByte(key);
			i++;
		}
		_delay_ms(500);
	}

	key = KEYPAD_getPressedKey();
	/*wait until the entered key is = to continue*/
	while (KEYPAD_getPressedKey() != '=') {
		LCD_displayStringRowColumn(0, 0, "Press enter(=)");
	}
	_delay_ms(500);
	/*re-take the password*/
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, " re-enter");
	LCD_displayStringRowColumn(1, 0, "pass:");
	LCD_moveCursor(1, 6);

	i = 0;
	while (i < 5) {
		key = KEYPAD_getPressedKey();
		if (isNumber(key)) {
			LCD_displayCharacter('*');
			UART_sendByte(key);
			i++;
		}
		_delay_ms(500);
	}
	/*wait until the entered key is = to continue*/
	while (KEYPAD_getPressedKey() != ENTER) {
		LCD_displayStringRowColumn(0, 0, "Press enter(=)");
	}

	/*receive the byte that indicate the passwords match or not*/
	g_receivedByte = UART_receiveByte();
	_delay_ms(100);

}

/* Description:
 * 	   take password and from the user through keypad
 * 	   send the password to CONTROL_ECU
 * 	   receive byte from CONTROL_ECU to know if passwords match or not
 */
void verifyPassword(void) {
	uint8 key;
	uint8 i = 0;
	while (i < 5) {
		key = KEYPAD_getPressedKey();
		if (isNumber(key)) {
			LCD_displayCharacter('*');
			UART_sendByte(key);
			i++;
		}
		_delay_ms(500);
	}
	/*wait until user presses enter*/
	while (KEYPAD_getPressedKey() != ENTER) {
	}
	/*receive byte: PASSWORDS_MATCH or PASSWORDS_NOT_MATCH*/
	g_receivedByte = UART_receiveByte();
	_delay_ms(100);
}

/* Description:
 * 	   display ERROR message on the screen
 * 	   start the timer for 1 minute
 * 	   stop the timer
 */
void operateBuzzer(void) {
	LCD_clearScreen();
	/*display error message*/
	LCD_displayStringRowColumn(0, 0, "ERROR!");
	/*start the timer and count 1 minute*/
	Timer1_init(&timer1_config_ptr);
	g_countSeconds = 0;
	/*display ERROR message for 1 minute*/
	while (g_countSeconds != 60) {
	}
	g_countSeconds = 0;
	/*stop the timer*/
	Timer1_deInit();
}

/* Description:
 * 	   turn on timer1
 * 	   display door is unlocking for 15 seconds
 * 	   display hold for 3 seconds
 * 	   display door is locking for 15 seconds
 *	   turn off timer1 and clear seconds counter
 */
void operateDoor(void) {
	LCD_clearScreen();
	/*display a message to the user that the door in unlocking*/
	LCD_displayStringRowColumn(0, 0, "Door is");
	LCD_displayStringRowColumn(1, 0, "Unlocking..");
	/*start timer1*/
	Timer1_init(&timer1_config_ptr);
	g_countSeconds = 0;
	/*wait for 15 seconds*/
	while (g_countSeconds != 15) {
	}

	LCD_clearScreen();
	/*display a message to the user that the door is on hold*/
	LCD_displayStringRowColumn(0, 0, "Hold Door..");
	g_countSeconds = 0;
	/*hold door for 3 seconds*/
	while (g_countSeconds != 3) {
	}

	LCD_clearScreen();
	/*display a message to the user that the door in locking*/
	LCD_displayStringRowColumn(0, 0, "Door is");
	LCD_displayStringRowColumn(1, 0, "Locking..");
	g_countSeconds = 0;
	/*wait for 15 seconds*/
	while (g_countSeconds != 15) {
	}

	/*turn off the timer*/
	Timer1_deInit();
	g_countSeconds = 0;
	/*display a message to the user that the door in locked*/
	LCD_displayStringRowColumn(0, 0, "Door is locked");
	_delay_ms(500);
}

/* Description:
 * 	call back function
 *  called in timer1 ISR
 */
void Timer1_INT(void) {
	g_countSeconds++;
}

int main(void) {
	SREG |= (1 << 7);
	uint8 key;
	/* Description:
	 * UART configurations:
	 *		baud rate = 9600,
	 *		char size 8 bits,
	 *		no parity checking
	 *		and one bit for stop bit
	 */
	UART_ConfigType uart_config_ptr = { baud_rate_9600, char_size_8,
			disabled_parity, one_bit_stop_bit };
	UART_init(&uart_config_ptr);

	/*set the call back function for timer1 to count seconds*/
	Timer1_setCallBack(Timer1_INT);

	/*call the initialize function for the LCD */
	LCD_init();

	/*
	 * take password 2 times as signing in
	 * if the two passwords match continue to the main options
	 */
	while (1) {
		/*ask the user to create new password*/
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 5, "Create");
		LCD_displayStringRowColumn(1, 2, "New Password");
		_delay_ms(1500);
		/*
		 * send to Control_ECU PASSWORD_1 command
		 * as the user will enter the password
		 */
		UART_sendByte(PASSWORD_1);
		/*take password 2 times from the user*/
		TakePassword();
		/*if passwords match break the while loop*/
		if (g_receivedByte == PASSWORDS_MATCH)
			break;
		else {
			LCD_clearScreen();
			/*display a message if the passwords do not match in sign in*/
			LCD_displayStringRowColumn(0, 0, "passwords don't");
			LCD_displayStringRowColumn(1, 0, "Match");
			_delay_ms(1000);
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 0, "try again");
			_delay_ms(500);
		}
	}

	while (1) {
		LCD_clearScreen();
		/*display the main options*/
		LCD_displayStringRowColumn(0, 0, "+ : Open Door");
		LCD_displayStringRowColumn(1, 0, "- : Change pass");
		_delay_ms(100);

		/*wait until the pressed key is open door or change password*/
		while (1) {
			key = KEYPAD_getPressedKey();
			if (key == '+' || key == '-')
				break;
		}
		LCD_clearScreen();

		/*send PASSWORD_2 to CONTROL_ECU to receive the password and verify it*/
		uint8 try;
		/*
		 * user has 3 tries to enter the password
		 * if password is wrong 3 times
		 * the buzzer is turned on
		 */
		for (try = 0; try < 3; ++try) {
			/*send to Control_ECU PASSWORD_2 command */
			UART_sendByte(PASSWORD_2);
			LCD_displayStringRowColumn(0, 0, "enter password:");
			LCD_moveCursor(1, 6);
			verifyPassword();
			/*if the password is right break the loop*/
			if (g_receivedByte == PASSWORDS_MATCH) {
				break;
			}
			LCD_displayStringRowColumn(0, 0, "WRONG PASS     ");
			LCD_displayStringRowColumn(1, 0, "              ");
			_delay_ms(500);
		}
		/*
		 * if the password was entered wrong for 3 times
		 * send to Control_ECU
		 * and turn the buzzer on
		 */
		if (g_receivedByte == PASSWORDS_NOT_MATCH) {
			UART_sendByte(WRONG_PASS_3);
			operateBuzzer();
		}
		/*
		 * if the pressed key is open door
		 * send to Control_ECU open door command
		 * and open the door
		 */
		else if (key == OPEN_DOOR) {
			UART_sendByte('+');
			operateDoor();
		}
		/*
		 * if the pressed key is change password
		 * the user has 3 chances to enter the password and verify it
		 * if the user doesn't pass any chance
		 * the buzzer is turned on for 1 minute
		 */
		else if (key == CHANGE_PASSWORD) {
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 5, "change");
			LCD_displayStringRowColumn(1, 2, "your password");
			_delay_ms(1000);
			uint8 try = 0;
			for (try = 0; try < 3; ++try) {
				UART_sendByte(CHANGE_PASSWORD);
				TakePassword();
				/*if passwords match break the while loop*/
				if (g_receivedByte == PASSWORDS_MATCH)
					break;
				else {
					/*if the password is wrong print a message to the user*/
					LCD_clearScreen();
					LCD_displayStringRowColumn(0, 0, "passwords don't");
					LCD_displayStringRowColumn(1, 0, "Match");
					_delay_ms(1000);
					LCD_clearScreen();
					/*if the user has not passed the 3 chances he cannot try again*/
					if (try < 2)
						LCD_displayStringRowColumn(0, 0, "try again");
					_delay_ms(500);
				}
			}
			/*
			 * if the user enters the password wrong for 3 times after choosing from main options
			 * send to Control_ECU WRONG_PASS_3 command
			 * and turn on the buzzer
			 */
			if (g_receivedByte == PASSWORDS_NOT_MATCH) {
				UART_sendByte(WRONG_PASS_3);
				operateBuzzer();
			}
			/*
			 * in the change password option
			 * if the 2 passwords entered by the user match
			 */
			else if (g_receivedByte == PASSWORDS_MATCH) {
				LCD_clearScreen();
				/*display a message to indicate that the password changed*/
				LCD_displayStringRowColumn(0, 0, "Pass changed");
				_delay_ms(500);
			}

		}

	}

}

