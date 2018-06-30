/*
 * TimerplusADC.c
 *
 * Created: 24/06/2018 04:34:23 p.m.
 * Author : Alejandro
 */ 

#define F_CPU 16000000UL
#define F_CPU_UART 16000000UL
#define USART_BAUDRATE  9600
#define BAUD_PRESCALE   (((F_CPU_UART / (USART_BAUDRATE * 16UL))) - 1)


#define SETBIT(ADDRESS,BIT) (ADDRESS |= (1<<BIT))
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))
#define FLIPBIT(ADDRESS,BIT) (ADDRESS ^= (1<<BIT))
#define CHECKBIT(ADDRESS,BIT) (ADDRESS & (1<<BIT))


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

unsigned char tiempo=0;
char buffer[5];
volatile uint16_t adc;

//Flags ADC 
unsigned char SLA_12_STATE = 0;
unsigned char SLA_CHANGE = 0x00;
unsigned char ADC_READ = 0x00;
unsigned char ADC_1_READ = 0x00;
unsigned int RD_SLA_PRE = 0;
unsigned int RD_SLA_AFT = 0; 



void setup_register(void);
void config_timer1(void);
void usart_init(void);
void adc_init(void);


int main(void)
{
	cli();
	adc_init();
	usart_init();
	config_timer1();
	setup_register();
	
	sei();
	
	while (1)
	{
	}
}

unsigned char USART_receive(void){
	
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
	
}

void USART_send( unsigned char data){
	
	//verifica si el buffer de Tx esta disponible para nueva data
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
	
}

void USART_putstring(char *StringPtr){
	
	while(*StringPtr != 0x00){
		USART_send(*StringPtr);
	StringPtr++;}
	
}

void config_timer1(){
	
	SETBIT(TCCR1B, CS12);
	CLEARBIT(TCCR1B,CS11);
	CLEARBIT(TCCR1B, CS10);
	//TCCR1B |=(1<<CS12)|(0<<CS11)|(0<<CS10);
	SETBIT(TIMSK1, TOIE1);
	//TIMSK1 |=(1<<TOIE1);
	
	TCNT1 =34286;  //Cada segundo
	
	
}
void usart_init(){
	
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);   // Turn on the transmission and reception circuitry
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01); // Use 8-bit character sizes
	UCSR0C &=~((1<<UPM00)|(1<<UPM01)|(1<<USBS0)); //No Parity, 1 bit stop
	
	UBRR0H = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value into the high byte of the UBRR register
	UBRR0L = BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR register

	UCSR0B |= (1 << RXCIE0); // Enable the USART Receive Complete interrupt (USART_RXC)
	
}
void setup_register(){
	
	SETBIT(DDRB,DDB5);
	//DDRB |=(1<<DDB5);
	CLEARBIT(PORTB, PORTB5);
	//PORTB &=~(1<<PORTB5);
	
}

void adc_init(){
	
	  SETBIT(ADMUX, REFS0);
	  CLEARBIT(ADMUX, REFS1);
	  //ADMUX |= (1<<REFS0)|(0<<REFS1); //Referencia a VCC
	  
	  CLEARBIT(ADMUX, MUX3);
	  CLEARBIT(ADMUX, MUX2);
	  CLEARBIT(ADMUX, MUX1);
	  SETBIT(ADMUX, MUX0);
	  //ADMUX |=(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(1<<MUX0); //Canal 0 del uC
	 
	 SETBIT(ADCSRA,ADEN);
	 SETBIT(ADCSRA, ADSC);
	 SETBIT(ADCSRA, ADATE);
	 SETBIT(ADCSRA,ADPS2);
	 SETBIT(ADCSRA, ADPS1);
	 SETBIT(ADCSRA, ADPS0);
	 
	  //ADCSRA|=(1<<ADEN)|(1<<ADSC)|(1<<ADATE); //No se utiliza la interrupcion por no ser necesaria.
	//ADCSRA|=(1<<ADEN)|(1<<ADSC)|(1<<ADATE)|(1<<ADIE); //Conversion habilitada, iniciada, autotrigered y interrupcion habilitada
	  //ADCSRA|=(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Prescaler de 128
	//ADMUX |=(1<<ADLAR);
}

ISR(TIMER1_OVF_vect){
	
	if(tiempo == 10){//  Cada 10 segundos 
		
		tiempo = 0;  
		adc=ADCW;       //lee el valor del canal 1 del ADC
		//PORTB ^=(1<<PORTB5);
		//USART_putstring("Firmware Supra Navas\r\n");
		USART_putstring("Leyendo Canal 1 ");      
		itoa(adc,buffer, 10);
		USART_putstring(buffer);
		USART_putstring("\r\n"); //salto de Linea y carrier return
		
		
		if(ADC_READ == 0x01)
		{
			if (ADC_1_READ ==0x01)
			{  //Como es la primera lectura igualo los valores de las mediciones
				RD_SLA_PRE = RD_SLA_AFT;    
				RD_SLA_AFT=adc;
				ADC_1_READ = 0x00; //limpio la bandera 
				
				if (RD_SLA_AFT >=196)
				{
					//Se tiene el carro encendido y esta cargando la bateria
					SLA_12_STATE = 1; 
				}
				else if ((RD_SLA_AFT >=180)&(RD_SLA_AFT <196))
				{
					//El carro esta apagado, y no se esta cargando la bateria
					SLA_12_STATE =2;
				}
				else if (RD_SLA_AFT<180)
				{
					//Carro apagado y Bateria Baja
					SLA_12_STATE = 3;
				}
			}
			else
			{  //Sino se vuelve a realizar la comparacion
				RD_SLA_PRE = RD_SLA_AFT;
				RD_SLA_AFT =adc;
				
				if (RD_SLA_AFT >=196)
				{
					 if (SLA_12_STATE !=1)
					 {
						 SLA_CHANGE = 0x01;
						 SLA_12_STATE =1;
					 }
				 }
				 else if ((RD_SLA_AFT >=180)&(RD_SLA_AFT<196))
				 {
					 if (SLA_12_STATE !=2)
					 {
						 SLA_CHANGE = 0x01;
						 SLA_12_STATE =2;
					 }
				}
				else if (RD_SLA_AFT <180)
				{
					if (SLA_12_STATE !=3)
					{
						SLA_CHANGE =0x01;
						SLA_12_STATE =3;
					}
				}
			}
		}
	}
	tiempo++;
}




