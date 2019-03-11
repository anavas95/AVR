/*
 * uart0_uart1.h
 *
 * Created: 2/26/2019 5:26:34 PM
 *  Author: Alejandro Navas
 */ 


#ifndef UART0_UART1_H_
#define UART0_UART1_H_

void usart_init();
void USART_send( unsigned char data);
void USART_putstring(char *StringPtr);
unsigned char USART_receive(void);


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

unsigned char USART_receive(void)
{
	while(!((UCSR0A & (1<<RXC0))));
	return UDR0;
}





#endif /* UART0_UART1_H_ */