#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "Timer_1.h"
#include "i2c.h"
#include "dcMotor.h"
#include "Buzzer.h"
#include "std_types.h"
#include "eeprom.h"

/*commands used to synch between HMI_ECU and CONTROL_ECU*/
#define CHANGE_PASSWORD '-'
#define OPEN_DOOR '+'
#define PASSWORDS_NOT_MATCH 0xFF
#define PASSWORDS_MATCH 0xF1
#define PASSWORD_1 0x02
#define PASSWORD_2 0x03
#define PASSWORD_ADDRESS_LOCATION 0x50
#define PASSWORD_MATCH_ADDRESS_LOCATION 0x60
#define WRONG_PASS_3 0x20

/* Description:
 * global variable to store the right password
 * a global variable to keep track
 * of the received commands from HMI_ECU
 */
uint8 g_staticPassword[6];
uint8 g_receivedByte;
uint8 g_countSeconds = 0;

/* Description:
 * TIMER1 configurations:
 *		initial_value = 0,
 *		compare_value = 31250 ticks per 1 second,
 *		pre-scaler = 256
 *		CTC mode of operation
 */
Timer1_ConfigType timer1_config_ptr = { 0, TICKS_PER_ONE_SECOND, prescaler_256,
		CTC_mode };
/*
 * Description:
 * 		store the password received from HMI_ECU
 *		store the re-entered password received from HMI_ECU
 *		compare the 2 passwords and store the password if it is right
 *		send a command to HMI_ECU whether the passwords match or not
 */
void storePassword(void) {
	uint8 password[6], password_match[6];
	uint8 i = 0;
	uint8 flag = PASSWORDS_MATCH;
	for (i = 0; i < 5; ++i) {
		/*
		 * receive the password for the first time from HMI_ECU
		 * store the password in the EEPROM
		 * */
		password[i] = UART_receiveByte();
		EEPROM_writeByte(PASSWORD_ADDRESS_LOCATION + i, password[i]);
		_delay_ms(500);
	}
	for (i = 0; i < 5; ++i) {
		/*
		 * receive the password for the first time from HMI_ECU
		 * store the password in the EEPROM
		 * */
		password_match[i] = UART_receiveByte();
		EEPROM_writeByte(PASSWORD_MATCH_ADDRESS_LOCATION + i,
				password_match[i]);
		_delay_ms(500);
	}

	for (i = 0; i < 5; ++i) {
		/*if passwords do not match clear the flag*/
		if (password[i] != password_match[i]) {
			flag = PASSWORDS_NOT_MATCH;
			break;
		}
	}
	if (flag == PASSWORDS_MATCH) {
		for (i = 0; i < 5; ++i) {
			g_staticPassword[i] = password[i];
		}
	}

	/* send the flag to the GMI_ECU */
	UART_sendByte(flag);
}

/* Description:
 * 	   receive password and from HMI_ECU
 * 	   verify if the stored password is the same as the user entered
 * 	   send command to the HMI_ECU whether the passwords match or not
 */
void verifyPassword(void) {
	uint8 i;
	uint8 password[6];
	uint8 flag = PASSWORDS_MATCH;
	for (i = 0; i < 5; ++i) {
		/*receive the password for the second time from HMI_ECU*/
		password[i] = UART_receiveByte();
		_delay_ms(500);
	}
	for (i = 0; i < 5; ++i) {
		/*if passwords do not match clear the flag*/
		if (password[i] != g_staticPassword[i]) {
			flag = PASSWORDS_NOT_MATCH;
			break;
		}
	}
	/*send a flag whether the password matched or not*/
	UART_sendByte(flag);

}

/* Description:
 * if the received byte is open the door
 * rotate the DC-motor CW for 15 seconds
 * then hold it for 3 seconds
 * and then rotate it ACW for 15 seconds
 */
void operateMotor(void) {
	/*turn on timer1*/
	Timer1_init(&timer1_config_ptr);

	/*rotate CW for 15 seconds (Unlocking the door)*/
	DcMotor_Rotate(CLOCK_WISE, 100);
	g_countSeconds = 0;
	while (g_countSeconds != 15) {
	}

	/*hold for 3 seconds (holding the door)*/
	DcMotor_Rotate(stop, 0);
	g_countSeconds = 0;
	while (g_countSeconds != 3) {
	}

	/*rotate ACW for 15 seconds (Locking the door)*/
	DcMotor_Rotate(ANTI_CLOCK_WISE, 100);
	g_countSeconds = 0;
	while (g_countSeconds != 15) {
	}

	/*stop the DC-motor*/
	DcMotor_Rotate(stop, 0);

	/*clear the seconds counter*/
	g_countSeconds = 0;
	/*turn off the timer*/
	Timer1_deInit();
}

/* Description:
 * enable the buzzer for 1 minute
 * then turn the buzzer off
 */
void operateBuzzer(void) {
	/*turn on timer1*/
	Timer1_init(&timer1_config_ptr);

	/*clear seconds counter*/
	g_countSeconds = 0;
	Buzzer_on();
	while (g_countSeconds != 60) {
	}
	Buzzer_off();
	g_countSeconds = 0;

	/*turn off the timer*/
	Timer1_deInit();
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

	TWI_ConfigType twi_config_ptr = { address_1, bit_rate_400Kbits };
	TWI_init(&twi_config_ptr);
	/*set the call back function for timer1 to count seconds*/
	Timer1_setCallBack(Timer1_INT);

	/*call the initialize functions for both buzzer and DC-motor*/
	Buzzer_init();
	DcMotor_Init();

	while (1) {
		/*the received command from HMI_ECU*/
		g_receivedByte = UART_receiveByte();

		/*if the user wants to create password or change password for the first time*/
		if (g_receivedByte == CHANGE_PASSWORD || g_receivedByte == PASSWORD_1) {
			storePassword();
		}
		/*
		 * if the user is re-entering(when changing or creating it) the password
		 * or wants to open the door
		 */
		else if (g_receivedByte == PASSWORD_2) {
			verifyPassword();
		}

		/*
		 * if the received command is open door
		 * start the de-motor
		 */
		else if (g_receivedByte == OPEN_DOOR) {
			operateMotor();
		}
		/*
		 * if the received command is WRONG_PASS_3
		 * means that the user entered the password wrong for 3 times
		 * in this case turn on the buzzer
		 */
		else if (g_receivedByte == WRONG_PASS_3) {
			operateBuzzer();
		}
	}
}

