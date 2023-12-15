
#ifndef I2C_H_
#define I2C_H_
#include "std_types.h"

#define TWI_PORT    PORTC_ID
#define TWI_SDA_PIN PIN1_ID
#define TWI_SCL_PIN PIN0_ID

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

/*devices addresses*/
typedef enum {
	address_1 = 0x02, address_2 = 0x03
} TWI_Address;

/*
 * Calculated according to this equation:
 * SCL frequency = (F_CPU)/(16+2*TWBR*4^TWPS)
 */
typedef enum {

	bit_rate_100Kbits = 0x20, bit_rate_400Kbits = 0x02
} TWI_BaudRate;

typedef struct {
	TWI_Address address;
	TWI_BaudRate bit_rate;
} TWI_ConfigType;

void TWI_init(const TWI_ConfigType *Config_Ptr);
void TWI_start();
void TWI_STOP();
void TWI_writeByte(uint8 data);
uint8 TWI_readByteWithACK();
uint8 TWI_readByteWithNACK();
uint8 TWI_getStatus();

#endif /* I2C_H_ */
