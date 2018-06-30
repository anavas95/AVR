/*
 * ADC.c
 *
 * Created: 13/06/2018 10:15:09 a.m.
 * Author : Alejandro Navas
 */ 


#include <avr/io.h>


//*******************Prototipos de la Funcion************
//void adc_init(void);
//uint16_t adc_read(uint8_t ch);

int main(void)
{
	DDRB |= (1<<DDB5);  //Led on Board
	PORTB &=~(1<<PB5); //Led on Board off
  //adc_init(); //Llamado de la funcion
	ADMUX |= ((1<<REFS0)|(1<<MUX0)|(1<<ADLAR)); //ADC reference to VCC, ADC1
	ADCSRA|=((1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)); //ADC Enable,AD Start Conversion, Preescaler 128
	ADCSRB&=~((1<<ADTS0)|(1<<ADTS1)|(1<<ADTS2)); //Free Running mode
	ADCSRA |=(1<<ADSC);
    while (1) 
    {
	
	if(ADCH<128){
	
			PORTB |=(1<<PB5);
		
		}//end if
		else{
			PORTB &=~(1<<PB5);
			
		}//end else
    }//end while
}//End main


//******Declaracion de las funciones****************

//void adc_init(void){
	
	//ADMUX |= ((1<<REFS0)|(1<<MUX1)); //ADC reference to VCC, ADC1
	//ADCSRA|=((1<<ADEN)|(1<<ADATE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)); //ADC Enable, Preescaler 128
	//ADCSRB&=~((1<<ADTS0)|(1<<ADTS1)|(1<<ADTS2)); //Free Running mode 
	
//}
	

	
	