/*
 * MPC230_IOExpander.h
 *
 * Created: 4/3/2019 2:44:36 PM
 *  Author: Navas
 */ 


#ifndef MPC230_IOEXPANDER_H_
#define MPC230_IOEXPANDER_H_

/************************************************************************
		REGISTERS OF MCP23017 (IINPUT/OUTPUT EXPANDERS)
************************************************************************/
#define MCP230_IODIRA	0x00   //Direction data register A 1=input, 0=output
#define MCP230_IODIRB   0x01   //Direction data register B 1=input, 0=output
#define MCP230_IPOLA	0x02   //Input polarity Register A 1!=input 0=input
#define MPC230_IPOLB	0x03   //Input polarity Register B 1!=input 0=input
#define MCP230_GPINTENA 0x04   //Interrupt on change control register A 1=Enable Interrupt 0=Disabled interrupt
#define MCP230_GPINTENB 0x05   //Interrupt on change control register B 1=Enable Interrupt 0=Disabled Interrupt
#define MCP230_DEFVALA	0x06   //Default Compare Register for interrupt register A
#define MCP230_DEFVALB	0x07   //Default Compare Register for interrupt register B
#define MCP230_INTCONA  0x08   //Interrupt Control Register A
#define MCP230_INTCONB  0x09   //Interrupt Control Register B
#define MCP230_IOCONA	0x0A   //Configuration Register
#define MCP230_IOCONB   0x0B   //Configuration Register
#define MCP230_GPPUA    0x0C   //Pull-ups configuration register A
#define MCP230_GPPUB    0x0D   //Pull-ups configuration register B
#define MCP230_INTFA    0x0E   //Interrupt Flag Register A
#define MCP230_INTFB    0x0F   //Interrupt Flag Register B
#define MCP230_INTCAPA  0x10   //Interrupt Captured Value for port A
#define MCP230_INTCAPB  0x11   //Interrupt Captured Value for port B
#define MCP230_GPIOA	0x12   //General Purpose IO PortA
#define MCP230_GPIOB	0x13   //General Purpose IO PortB
#define MCP230_OLATA    0x14   //Output Registar Latch A  1
#define MCP230_OLATB	0x15   //Output Registar Latch B


/************************************************************************
		MASKING BITS: Using in the Inputs.
		Note: Use this constant in junction with 
			  Pullups configuration registers.
************************************************************************/
#define MASK0  0b00000001      //Mask of bit0
#define MASK1  0b00000010      //Mask of bit1
#define MASK2  0b00000100      //Mask of bit2
#define MASK3  0b00001000      //Mask of bit3
#define MASK4  0b00010000      //Mask of bit4
#define MASK5  0b00100000      //Mask of bit5
#define MASK6  0b01000000      //Mask of bit6
#define MASK7  0b10000000      //Mask of bit7

/************************************************************************
							PROTOTYPES
************************************************************************/
void MCP230_write(unsigned char slave_add, unsigned char register_add, unsigned char data);                           //Write in the slave
unsigned char MCP230_read(unsigned char slave_add, unsigned char register_add);								          //Read data into slave
void MCP230_setPORTA(unsigned char slave_add, unsigned char pin_value);										          //Set HIGH/LOW all PORTx
void MCP230_setPORTB(unsigned char slave_add, unsigned char pin_value);										          //Set HIGH/LOW all PORTx
void MCP230_setDDRA(unsigned char slave_add, unsigned char pin_value);										          //Set all port as Input/OUTPUT
void MCP230_setDDRB(unsigned char slave_add, unsigned char pin_value);										          //Set all port as Input/OUTPUT
void MCP230_seton_off_PORTx(unsigned char slave_add, unsigned char port,unsigned char bit, unsigned char pin_value);  //IDEM MCP230_setPORTA/B, include bit
void MCP230_DDRx_PORTx(unsigned char slave_add, unsigned char port, unsigned char bit, unsigned direction);           //IDEM MCP230_setDDRx, include bit
void MCP230_setPullups(unsigned char slave_add,unsigned char port,unsigned char bit,unsigned char enable);            //Set pullups resistor.
unsigned char MCP230_readPORTx(unsigned char slave_add,unsigned char port);											  //Read gpios configured as inputs


/************************************************************************
							Functions
************************************************************************/

/*
  This function write data in the slave.
*/
void MCP230_write(unsigned char slave_add, unsigned char register_add, unsigned char data)
{
	I2C_Start(slave_add);
	I2C_Transmit(register_add);
	I2C_Transmit(data);
	I2C_Stop();
}/*MCP230_write*/

/*
  This function read data in the slave.
*/
unsigned char MCP230_read(unsigned char slave_add, unsigned char register_add)
{
	unsigned char data;
	I2C_Start(slave_add);
	I2C_Transmit(register_add);		
	I2C_REP_Start((slave_add+1));
	data=I2C_Receive_Nack();
	I2C_Stop();
	return data;		
}/*MCP230_read*/

/*
  This function set especific value in the register. Is used to set High o Low all portA.
  Note: Future, select register is set.
*/
void MCP230_setPORTA(unsigned char slave_add, unsigned char pin_value)
{
	MCP230_write(slave_add,MCP230_GPIOA,pin_value);
}/*MCP230_setPORTA*/

/*
  This function set especific value in the register. Is used to set High o Low all portA.
  Note: Future, select register is set.
*/
void MCP230_setPORTB(unsigned char slave_add, unsigned char pin_value)
{
	MCP230_write(slave_add,MCP230_GPIOB,pin_value);
}/*MCP230_setPORTB*/

/*
  This function set  direction of the PORTA.
  The value of the pin_value variable is: 
											1=Output
											0=Input.
  Note: The value of pin_value is denied, as specified in the datasheet.
*/
void MCP230_setDDRA(unsigned char slave_add, unsigned char pin_value)
{
	MCP230_write(slave_add, MCP230_IODIRA,~(pin_value));
}/*MCP230_setDDRA*/

/*
  This function set  direction of the PORTA.
  The value of the pin_value variable is: 
											1=Output
											0=Input.
  Note: The value of pin_value is denied, as specified in the datasheet.
*/
void MCP230_setDDRB(unsigned char slave_add, unsigned char pin_value)
{
	MCP230_write(slave_add, MCP230_IODIRB,~(pin_value));
}/*MCP230_setDDRB*/

/*
  This function set HIGH or LOW, the bits of the PORTA/PORTB.
	The value of parameters is:
			Slave_add=slave of slave
			port=register of the GPIOA or GPIOB
			bit=bit of last port configurated,enum to 0 at 7.
			pin_value= 1=HIGH.
			           0=LOW.
  Note:This function only affect the bits is set as outputs.
*/
void MCP230_seton_off_PORTx(unsigned char slave_add,unsigned char port, unsigned char bit, unsigned char pin_value)
{
	if(bit<8)
	{
		unsigned char port_bit=MCP230_read(slave_add, port);
		
		if(pin_value == 1)
		{
			port_bit|=(1<<bit);
		}
		else
		{
			port_bit &=~(1<<bit);
		}
		MCP230_write(slave_add,port, port_bit);
	}	
}/*MCP230_on_off_PORTx*/


/*
  This function set direction(Input/Output) of the bits of the PORTA/PORTB.
	The value of parameters is:
			Slave_add=slave of slave
			port=register of the  IODIRA or IODIRB
			bit=bit of last port configurated,enum to 0 at 7.
			Direction= 1=Input.
			           0=Output.
  Note: The value of the directions is denied respect to conventional.
*/
void MCP230_DDRx_PORTx(unsigned char slave_add, unsigned char port, unsigned char bit, unsigned direction)
{
	if(bit<8)
	{
		unsigned char bit_port=MCP230_read(slave_add, port);
		
		if(direction == 1)
		{
			bit_port |=(1<<bit);
		}
		if(direction==0)
		{
			bit_port &=~(1<<bit);
		}
		
		MCP230_write(slave_add,port,bit_port);
	}
}/*MCP230_DDRx_PORTx*/


/*
  This function set PullUps resistors in the bits is configured as inputs.
	The value of parameters is:
			Slave_add=slave of slave
			port=register of the  GPPUA or GPPUB
			bit=bit of last port configurated,enum to 0 at 7.
			Enable=    1=Active PullUps.
			           0=Desactive PullUps.
  Note: Use in junctions with instructions set inputs registers.
*/

void MCP230_setPullups(unsigned char slave_add,unsigned char port,unsigned char bit,unsigned char enable)
{
	if(bit<8)
	{
		unsigned char bit_port=MCP230_read(slave_add,port);
		
		if(enable == 1)
		{
			bit_port |=(1<<bit);
		}
		if(enable == 0)
		{
			bit_port &=~(1<<bit);
		}
		MCP230_write(slave_add,port,bit_port);
	}
}

/*
  This function return data of how the GPIOS states are. 
	The value of parameters is:
			Slave_add=slave of slave
			port=register of the  GPIOA or GPIOB
		
  Note: Use in junctions with MASK bits.
*/
unsigned char MCP230_readPORTx(unsigned char slave_add,unsigned char port)
{
	unsigned char data=MCP230_read(slave_add,port);
	return data;
}

#endif /* MPC230_IOEXPANDER_H_ */