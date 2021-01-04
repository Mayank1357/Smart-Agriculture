/*
 * USART.h
 *
 * Created: 17/07/2019 11:22:19 AM
 *  Author: asd
 */ 


#ifndef USART_H_
#define USART_H_
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include "lcd_4bit.h"

void UART_init ()
{
	UCSRB |= (1<<TXEN)|(1<<RXEN);
	UCSRC |= (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
	UBRRL = 0x33;
}

void UART_Tchar(unsigned char data)
{
	UDR = data;
	while(!(UCSRA&(1<<TXC)));
}

void UART_Tstring(char *p)
{
	while(*p)
	{
		UART_Tchar(*(p++));
		_delay_ms(1);
	}
}

char UART_Rchar()
{
	while(!(UCSRA & (1 << RXC)));
	return UDR;
}

char *UART_Rstring(char *buffer)
{
	char *p = buffer;
	
	do
	{
		*(buffer++) = UART_Rchar();
	} while (*(buffer -1 ) != '\r');
	
	*buffer = '\0';
	return p;
}



#endif /* USART_H_ */