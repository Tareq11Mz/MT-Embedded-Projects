
#include "i2c.h"
#include <avr/io.h>
#include "common_macros.h"

void TWI_init(const TWI_ConfigType *Config_Ptr) {
	/*
	 * Bit Rate: is set using zero pre-scaler TWPS=00 and F_CPU=8Mhz
	 * SCL frequency = F_CPU/(16+2(TWBR)*4^TWPS)
	 */
	TWBR = Config_Ptr->bit_rate;
	TWSR = 0x00;
	/*set this MC address to the specified if any master wants to communicate with it*/
	TWAR = Config_Ptr->address;
	/*enable the TWI*/
	TWCR = (1 << TWEN);
}

void TWI_start() {
	/*
	 * Clear TWINT bit before starting.
	 * Set TWSTA bit to send start condition.
	 * Enable TWI module.
	 */
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	/*Wait until the start bit is sent (until SWINT bit is set)*/
	while (BIT_IS_CLEAR(TWCR, TWINT))
		;
}

void TWI_STOP() {
	/*
	 * Clear TWINT bit before stopping.
	 * Set TWSTO bit to send stop condition.
	 * Enable TWI module.
	 */
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void TWI_writeByte(uint8 data) {
	while (!(TWCR & (1 << TWINT)))
		;
	/*Put the data in the TWI data register*/
	TWDR = data;
	/*Clear the TWINT bit and enable TWI module*/
	TWCR = (1 << TWINT) | (1 << TWEN);
	/*Wait until TWINT bit is high (data is sent successfully) */
	while (BIT_IS_CLEAR(TWCR, TWINT))
		;
}

uint8 TWI_readByteWithACK() {
	/*
	 * Clear TWINT bit.
	 * Set TWEA bit to send ACK after receiving data.
	 * Enable TWI module.
	 */
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	/*Wait until TWINT bit is high (data is received successfully) */
	while (BIT_IS_CLEAR(TWCR, TWINT))
		;
	return TWDR;
}
uint8 TWI_readByteWithNACK() {
	/*
	 * Clear TWINT bit.
	 * clear TWEA bit to send NACK after receiving data.
	 * Enable TWI module.
	 */
	TWCR = (1 << TWINT) | (1 << TWEN);
	/*Wait until TWINT bit is high (data is received successfully) */
	while (BIT_IS_CLEAR(TWCR, TWINT))
		;
	return TWDR;
}
uint8 TWI_getStatus() {
	/*Get the last 5 bits in the status register*/
	return TWSR & 0xF8;
}
