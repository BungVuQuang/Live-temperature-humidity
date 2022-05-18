/*
 * uart.h
 *
 * Created: 4/22/2022 3:29:36 PM
 *  Author: Bungdz
 */ 


#ifndef UART_H_
#define UART_H_
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdio.h>
//#include <avr/power.h>
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
//long USART_BAUDRATE
void UART_Init(long USART_BAUDRATE){
	//UBRRH=0;//dung khi UBRRL>2 mu~ 8
	//UBRRL=51;//115200
	//clock_prescale_set(clock_div_1);
	UBRRL = BAUD_PRESCALE;
	UBRRH = (BAUD_PRESCALE >> 8);
	UCSRA=0X00;
	UCSRB|=(1<<TXEN);
	UCSRC|=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);//khai bao dung UCSRC, va khung truyen 8 bit
	//sei();
}

//NHAN DU LIEU
unsigned char UART_gettchar(void){
	while(!(UCSRA&(1<<RXC)));
	return UDR;
}
//truyen du lieu
void UART_putString(char* data){
	for(int i=0;data[i]!=0;i++)
	{
		while(!(UCSRA&(1<<UDRE)));//cho den khi thanh ghi UDRE trong
		UDR=data[i];
	}	
}

void UART_putInt(int data){
		while(!(UCSRA&(1<<UDRE)));//cho den khi thanh ghi UDRE trong
		UDR=data;
}


#endif /* UART_H_ */