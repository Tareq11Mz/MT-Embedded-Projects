/******************************************************************************
 *
 * File Name: UART.c
 *
 * Description: Source file for UART driver
 *
 *******************************************************************************/

#include "UART.h"

#include <avr/io.h>
#include "common_macros.h"

void UART_init(UART_ConfigType * config_ptr){
	UCSRA = (1<<U2X);
	UCSRB = (1<<TXEN) | (1<<RXEN)  | (config_ptr->bit_data&0x04);
	UCSRC = (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1) |(config_ptr->stop_bit<<3) | ((config_ptr->parity&0x03)<<4) | ((config_ptr->bit_data&0x03)<<1);
	uint16 ubrr_val = (uint16)((F_CPU/(8UL*config_ptr->baud_rate))-1);

	UBRRH = ubrr_val>>8;
	UBRRL = ubrr_val;

}

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(uint8 data){
	//wait until the last data is shifted out and the transmit buffer is ready to receive new data.
	while(BIT_IS_CLEAR(UCSRA,UDRE));
	/*
	 * put the required data in the transmit buffer (UDR).
	 * it clears UDRE bit.
	 */
	UDR = data;
}

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_receiveByte(void){
//	wait until there is a new data IS received in the receive buffer
	while(BIT_IS_CLEAR(UCSRA,RXC));
	// save data, the RXC will be cleared after reading the data
	return UDR;
}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str){
	uint8 i =0;
	while(Str[i]!='\0'){
		// send byte by byte
		UART_sendByte(Str[i++]);
	}
}

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str){
	uint8 i = 0;
	// receive first byte
	Str[i] = UART_receiveByte();
	// check if the received byte is not #, if true store the byte in the string
	while(Str[i]!='#'){
		i++;
		Str[i] = UART_receiveByte();
	}
	// after receiving the string add null instead of # symbol
	Str[i] = '\0';
}
