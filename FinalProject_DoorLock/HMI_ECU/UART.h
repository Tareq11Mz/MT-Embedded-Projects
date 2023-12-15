/******************************************************************************
 *
 * File Name: UART.c
 *
 * Description: Header file for UART driver
 *
 *******************************************************************************/

#include "std_types.h"

/*Parity bit in UART frame*/
typedef enum {
	disabled_parity, reserved, even_parity, odd_parity
} UART_Parity;

/*Stop bit in UART frame*/
typedef enum {
	one_bit_stop_bit, two_bit_stop_bit
} UART_StopBit;

/*char size in bits in UART frame*/
typedef enum {
	char_size_5, char_size_6, char_size_7, char_size_8, char_size_9 = 7
} UART_BitData;

/*Baud rate in UBRR register in UART*/
typedef enum {
	baud_rate_9600 = 9600
} UART_BaudRate;

typedef struct {
	UART_BaudRate baud_rate;
	UART_BitData bit_data;
	UART_Parity parity;
	UART_StopBit stop_bit;
} UART_ConfigType;

void UART_init(UART_ConfigType *config_ptr);
/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */

void UART_sendByte(uint8 data);
/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_receiveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #
