/******************************************************************************
 *
 * File Name: eeprom.c
 *
 * Description: Source file for the external eeprom driver
 *
 *******************************************************************************/
#include "eeprom.h"
#include "i2c.h"

uint8 EEPROM_writeByte(uint16 address,uint8 data){
	/*send the start bit*/
	TWI_start();

	/*if the status is wrong stop the program*/
	if(TWI_getStatus()!=TWI_START)
		return ERROR;

	/*send the memory address, the most significant 4 bits in the EEPROM are fixed = 1010*/
	TWI_writeByte((uint8)(0xA0 | ((address&0x0700)>>7)));
	if(TWI_getStatus()!=TWI_MT_SLA_W_ACK)
		return ERROR;
	/*Send the least significant 8 bits of the EEPROM address.*/
	TWI_writeByte((uint8)address); // casted to uint8
	if(TWI_getStatus()!=TWI_MT_DATA_ACK)
		return ERROR;

	/*Send the data to be stored in that address*/
	TWI_writeByte(data);
	if(TWI_getStatus()!=TWI_MT_DATA_ACK)
		return ERROR;

	TWI_STOP();
	return SUCCESS;
}

uint8 EEPROM_readByte(uint16 address,uint8 *data){
	/*send the start bit*/
	TWI_start();

	/*if the status is wrong stop the program*/
	if (TWI_getStatus() != TWI_START)
		return ERROR;

	/*
	 * send the memory address, the most significant 4 bits in the EEPROM are fixed = 1010.
	 * R/W=0 (write).
	*/
	TWI_writeByte((uint8) (0xA0 | ((address & 0x0700) >> 7)));
	if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
		return ERROR;

	/*Send the least significant 8 bits of the EEPROM address.*/
	TWI_writeByte((uint8)address); // casted to uint8
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;


	/*send the repeated start bit */
	TWI_start();
	if (TWI_getStatus() != TWI_REP_START)
		return ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=1 (Read) */
	TWI_writeByte((uint8) ((0xA0) | ((address & 0x0700) >> 7) | 1));
	if (TWI_getStatus() != TWI_MT_SLA_R_ACK)
		return ERROR;

	/* Read Byte from Memory without send ACK */
	*data = TWI_readByteWithNACK();
	if (TWI_getStatus() != TWI_MR_DATA_NACK)
		return ERROR;

	/* Send the Stop Bit */
	TWI_STOP();

	return SUCCESS;
}

