/*
 * i2c_twi.h
 *
 * Created: 2/26/2019 4:55:46 PM
 *  Author: Alejandro Navas
 */ 


#ifndef I2C_TWI_H_
#define I2C_TWI_H_

#include <util/twi.h>

#define TWSTAT (TWSR0 & 0xF8)

void I2C_Init(void);
uint8_t I2C_Transmit(uint8_t data);
uint8_t I2C_Receive_ACK(void);
uint8_t I2C_Receive_Nack(void);
uint8_t I2C_Start(uint8_t address_rw);
uint8_t I2C_REP_Start(uint8_t address_rw);
void I2C_Stop(void);


/*Setea los registros necesarios para utilizar la interfaz 
i2c(TWI) del microcontrolador*/
void I2C_Init(void)
{
	
	TWSR0 = 0;					//Prescaler igual a 0
	TWBR0 = 12;					//Baudrate SCL=400kHz. F_CPU = 16Mhz
	
}/*i2c_init*/


/*Envia(escribe) las direcciones de acceso a la memoria del esclavo
retorna 0 si logro transmitir
retorna 1 si es incapaz de iniciar la transmision
ejemplo: ret=i2c_transmit(0x00)*/
uint8_t I2C_Transmit(uint8_t data)
{
	TWDR0 = data;
	TWCR0 = ((1<<TWINT)|(1<<TWEN));
	while(!(TWCR0 & (1<<TWINT)));
	
	if((TWSTAT != TW_MT_SLA_ACK))
	{
		return 1;
	}
	
	return 0;
}/*i2c_transmit*/

/*Inicia la recepcion de datos desde el esclavo
 de forma continua, es decir se le solicita más de 
 un byte de forma secuencial*/
uint8_t I2C_Receive_ACK(void)
{
	TWCR0 = ((1<<TWINT)|(1<<TWEA)|(1<<TWEN));
	while(!(TWCR0 & (1<<TWINT)));
	return TWDR0;
}/*i2c_receive_ack*/

/*Inicia la recepcion de un solo byte desde el esclavo*/
uint8_t I2C_Receive_Nack(void)
{
	TWCR0 = ((1<<TWINT)|(1<<TWEN));
	while(!(TWCR0 &(1<<TWINT)));
	return TWDR0;
}/*i2c_receive_nack*/


/*Inicia la transmision de datos hacia el esclavo
retorna 0 si fue logro iniciar la comunicacion con el esclavo
retorna 1 si no logro iniciar la comunicacion con el esclavo*/
uint8_t I2C_Start(uint8_t address_rw)
{
	TWCR0 = ((1<<TWINT)|(1<<TWSTA)|(1<<TWEN));
	while(!(TWCR0 &(1<<TWINT)));
	
	if((TWSTAT != TW_START) && (TWSTAT != TW_REP_START))
	{
		return 1;
	}
	
	TWDR0 = address_rw;
	TWCR0 = ((1<<TWINT)|(1<<TWEN));
	while(!(TWCR0 & (1<<TWINT)));
	
	if((TWSTAT != TW_MT_SLA_ACK) && (TWSTAT != TW_MR_SLA_ACK))
	{
		return 1;
	}
	
	return 0;
} /*i2c_start*/


/*Inicia una condicion de inicio repetido en caso de que
se le este solicitando más de un byte de informacion al esclavo*/
uint8_t I2C_REP_Start(uint8_t address_rw)
{
	return (I2C_Start(address_rw));
}/*i2c_rep_start*/

/*Termina la comunicacion con el esclavo, liberando el bus de comunicacion*/
void I2C_Stop(void)
{
	TWCR0 = ((1<<TWINT)|(1<<TWEN)|(1<<TWSTO));
	while(TWCR0 & (1<<TWSTO));
	
}/*i2c_stop*/




#endif /* I2C_TWI_H_ */