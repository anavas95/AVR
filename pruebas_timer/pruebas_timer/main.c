/*
 * pruebas_timer.c
 *
 * Created: 23/06/2018 09:08:24 p.m.
 * Author : Alejandro
 */ 

#define F_CPU 16000000UL
#define F_CPU_UART 16000000UL
#define USART_BAUDRATE  9600
#define BAUD_PRESCALE   (((F_CPU_UART / (USART_BAUDRATE * 16UL))) - 1)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char tiempo=0;

void setup_register(void);
void config_timer1(void);
void usart_init(void);

 
int main(void)
{
	usart_init();
	config_timer1();
    setup_register();
	
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
	
	cli();
	TCCR1B |=(1<<CS12)|(0<<CS11)|(1<<CS10);
	TIMSK1 |=(1<<TOIE1);
	
	TCNT1 =57724;
	
	sei();
	
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
	
	
	DDRB |=(1<<DDB5);
	PORTB &=~(1<<PORTB5);
	
}

ISR(TIMER1_OVF_vect){
	
	 if(tiempo == 4){
			
			//tiempo = 0;
			//PORTB ^=(1<<PORTB5);
			USART_putstring("Firmware Supra Navas\r\n");
			 }
			 
	 if (tiempo==6)
	 {
		 tiempo=0;
		 PORTB ^= (1<<PORTB5);
		 USART_putstring("soy la mera tos\r\n"); 
	 }
	tiempo++;
}