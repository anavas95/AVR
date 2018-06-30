/*
 * GccApplication1.c
 *
 * Created: 23/06/2018 10:05:38 p.m.
 * Author : Alejandro
 */ 

#define F_CPU		16000000UL
#define F_CPU_UART  16000000UL
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU_UART / (USART_BAUDRATE * 16UL))) - 1)


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

char bufferadc[5];
unsigned char tiempo=0;

void adc_init();
void usart_init();
void config_timer1(void);
void USART_send( unsigned char data);
void USART_putstring(char *StringPtr);



int main(void)
{
	cli();
	usart_init();
    adc_init();
	config_timer1();
	sei();
    while (1) 
    {
    }
}

/*
ISR(ADC_vect){
	
	
	uint16_t adc=ADCW; 
	USART_putstring("Leyendo Canal 1 ");
	itoa(adc,bufferadc, 10);
	USART_putstring(bufferadc);
	USART_putstring("\r\n");
	//USART_putstring("");
	//USART_putstring(0x0A);
	//USART_putstring(0x0D);
	_delay_ms(10);
	//USART_putstring(adc);
	//USART_putstring("\r\n");

	
	
}*/


ISR(TIMER1_OVF_vect){
	
	uint16_t adc=ADCW; 
	
	if(tiempo == 4){
		
		USART_putstring("Leyendo Canal 1 ");
		itoa(adc,bufferadc, 10);
		USART_putstring(bufferadc);
		USART_putstring("\r\n");
		//tiempo = 0;
		//PORTB ^=(1<<PORTB5);
		//USART_putstring("Firmware Supra Navas\r\n");
	}
	
	if (tiempo==6)
	{
		tiempo=0;
		USART_putstring("Leyendo Canal 1 ");
		itoa(adc,bufferadc, 10);
		USART_putstring(bufferadc);
		USART_putstring("\r\n");
		PORTB ^= (1<<PORTB5);
		//USART_putstring("soy la mera tos\r\n");
	}
	tiempo++;
}
void adc_init(){
	
	
	ADMUX |= (0<<REFS0)|(1<<REFS1); //Referencia a VCC
	ADMUX |=(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(1<<MUX0); //Canal 0 del uC
	
	ADCSRA|=(1<<ADEN)|(1<<ADSC)|(1<<ADATE)|(1<<ADIE); //Conversion habilitada, iniciada, autotrigered y interrupcion habilitada
	ADCSRA|=(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Prescaler de 128
	//ADMUX |=(1<<ADLAR);
}

void usart_init(){
	
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);   // Turn on the transmission and reception circuitry
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01); // Use 8-bit character sizes
	UCSR0C &=~((1<<UPM00)|(1<<UPM01)|(1<<USBS0)); //No Parity, 1 bit stop
	
	UBRR0H = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value into the high byte of the UBRR register
	UBRR0L = BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR register

	UCSR0B |= (1 << RXCIE0); // Enable the USART Receive Complete interrupt (USART_RXC)
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

unsigned char USART_receive(void){
	
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
	
}

void config_timer1(){
	
	//cli();
	TCCR1B |=(1<<CS12)|(0<<CS11)|(1<<CS10);
	TIMSK1 |=(1<<TOIE1);
	
	TCNT1 =57724;
	
	//sei();
	
}


