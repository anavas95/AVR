/*
 * EEmemory.h
 *
 * Created: 2/26/2019 5:10:08 PM
 *  Author: Alejandro Navas
 */ 


#ifndef EEMEMORY_H_
#define EEMEMORY_H_

#define I2C_READ    1
#define I2C_WRITE   0



void eeprom_write(uint8_t slave_dir,uint16_t byte_eeprom, unsigned char data);   //Funcion que escribe en el esclavo, pasandole su direccion, el byte de direccion y el dato a escribir.
unsigned char eeprom_read(uint8_t slave_dir,uint16_t byte_eeprom);				 //Funcion que lee un dato del esclavo, pasandole su direcicon y el byte que se desea leer
void eepromi2c_write(uint8_t slave_dir, uint16_t page, uint8_t byte, uint8_t data);	//Funcion que escribe en la eeprom, pasandole su direccion, la pagina que se desea, el byte de direcion y el dato.
unsigned char eepromi2c_read(uint8_t slave_dir,uint16_t page, uint16_t byte);		//Funcion que lee un dato de la eeprom  pasandole su direccion, la pagina que se desea, el byte que se desea leer.


/*Escribe un byte de informacion en una direccion de la eeprom*/
void eeprom_write(uint8_t slave_dir,uint16_t byte_eeprom, unsigned char data)
{
	//uint8_t slave_dir;
	I2C_Start(slave_dir);
	I2C_Transmit(byte_eeprom>>8);
	I2C_Transmit(byte_eeprom);
	I2C_Transmit(data);
	I2C_Stop();
	
}/*eeprom_write*/

/*lee y devuelve la data almacenada en el byte solicitado a la memoria eeprom*/
unsigned char eeprom_read(uint8_t slave_dir,uint16_t byte_eeprom)
{
	unsigned char data = 0;
	//uint8_t slave_dir;
	I2C_Start(slave_dir);
	I2C_Transmit(byte_eeprom>>8);
	I2C_Transmit(byte_eeprom);
	I2C_REP_Start((slave_dir+1));
	
	data = I2C_Receive_Nack();
	I2C_Stop();
	
	return data;
}/*eeprom_read*/

/*escribe en la memoria eeprom especificando la pagina, la direccion y la data*/
void eepromi2c_write(uint8_t slave_dir,uint16_t page, uint8_t byte, uint8_t data)
{
	//uint8_t slave_dir;
	uint16_t byte_adress;
	byte_adress = (page*128)+byte;
	I2C_Start(slave_dir);
	I2C_Transmit(byte_adress>>8);
	I2C_Transmit(byte_adress);
	I2C_Transmit(data);
	I2C_Stop();
	//_delay_ms(100);
	
}/*eepromi2c_write*/

unsigned char eepromi2c_read(uint8_t slave_dir ,uint16_t page, uint16_t byte)
{
	
	uint16_t byte_adress;
	uint8_t data;
	//uint8_t slave_dir;
	byte_adress =(page*128)+byte;
	I2C_Start(slave_dir);
	I2C_Transmit(byte_adress>>8);
	I2C_Transmit(byte_adress);
	
	I2C_Start((slave_dir+ I2C_READ));
	data=I2C_Receive_Nack();
	I2C_Stop();
	return data;
}





#endif /* EEMEMORY_H_ */