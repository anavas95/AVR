/*
 * Hello_World.c
 *
 * Created: 08/06/2018 08:47:52 a.m.
 * Author : Alejandro Navas
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
//#include <util/delay.h>



int main(void)
{
	//Outputs
	DDRB |=(1<<DDB5);  //Bit 5 del Puerto B como salida.
	PORTB &=~(1<<PB5); //Inicializada en LOW.
	
	//Inputs
	DDRB &=~(1<<DDB7); //Bit 7 del puerto B como entrada
	PORTB |=(1<<PB7);  //Pull-up resistors.
	
	
    while (1) 
    {
		if ((PINB & (1<<PINB7))==0)  //Si el bit 7 se encuentra en bajo
		{
			PORTB |= (1<<PB5);       //Coloca en el bit 5 en High
		}
		else
		{
			PORTB &=~(1<<PB5);	     //Si no el bit 5 se encuentra en Low 
		}
				
	
    }
}

