/*
 * DHT11_UART.c
 *
 * Created: 4/22/2022 3:28:27 PM
 * Author : Bungdz
 */ 

#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>
//#include <util/twi.h>
#include <dht11.h>
#include "LCDC_4Bit.h"
#include <uart.h>
//#include "I2C_LCD_Run.h"
//#include "I2C.h"
volatile uint8_t i=0;
volatile uint16_t cout=0;
uint8_t I_RH,D_RH,I_Temp,D_Temp,CheckSum;
int old_temperator=0;
char buffer[20];

static int* dht11_temp;
//static int* dht11_humi;
//void Request(void)				/* Microcontroller send start pulse/request */
//{
	//_delay_ms(1000);
	//DDRA |= (1<<DHT11_PIN);
	//PORTA &= ~(1<<DHT11_PIN);	/* set to low pin */
	//_delay_ms(20);			/* wait for 20ms */
	//PORTA |= (1<<DHT11_PIN);	/* set to high pin */
	//_delay_us(40);
	//DDRA &= ~(1<<DHT11_PIN);
//}
//
//void Response(void)				/* receive response from DHT11 */
//{
	//
	//while(PINA & (1<<DHT11_PIN));/* DHT send response about 80us then to high voltage*/
	//while((PINA & (1<<DHT11_PIN))==0);/*doi len muc cao*/
	//while(PINA & (1<<DHT11_PIN));/* doi xuong muc thap de bat dau gui du lieu*/
//}
//
//uint8_t Receive_data(void)			/* receive data */
//{
	//for (int q=0; q<8; q++)
	//{
		//while((PINA & (1<<DHT11_PIN)) == 0);  /* check received bit 0 or 1 , doi len muc cao*/
		//_delay_us(30);/*neu sau 30us ma DATA van o muc cao thi do la logic 1*/
		//if(PINA & (1<<DHT11_PIN))/* if high pulse is greater than 30ms */
		//c = (c<<1)|(0x01);	/* then its logic HIGH */
		//else			/* otherwise its logic LOW */
		//c = (c<<1);
		//while(PINA & (1<<DHT11_PIN));/*keo xuong doi bit tiep theo*/
	//}
	//return c;/*xong 1 bit*/
//}

int main(void)
{
	//CLKPR = (1 << CLKPCE);
	//CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
	DHT11_init();	
	//DDRC|=(1<<0)|(1<<1);
	//PORTC = 0x03;
	//I2C_LCD_Init();
	//I2C_LCD_Clear();
	//I2C_LCD_BackLight(1);
	//Cong thuc: TCNT0=255- (8.000.000 * timeTran`)/presscale
	TCCR0|=(1<<CS01)|(1<<CS00);//Thanh ghi chon Mode va chon xung clock presscale 64;
	TCNT0=130;//thanh ghi luu gia tri dem
	TIMSK|=(1<<TOIE0);//thanh ghi mat na ngat de khi tran timer se nhay vao ham ngat, bit 0 voi Normal
	//Neu k dung mat na ngat, thi phai dung thanh ghi co ngat (TIFR), khi co` tran` timer0 TOV0 dc xet len 1 thi timer0 da tran	
	sei();//ngat toan cuc
		//LCD_Init();
		//_delay_ms(10);
		//LCD_Gotoxy(0,0);
		//LCD_Puts("Temperator:");
		//_delay_ms(2000);

	UART_Init(9600);
	_delay_ms(50);
	//I2C_LCD_write_string_XY(0, 0, "hello world");
	//LCD_Clear();
    while (1)
    {
			//Request();
			//Response();
			//I_RH=Receive_data();
			//D_RH=Receive_data();
			//I_Temp=Receive_data();
			//D_Temp=Receive_data();
			//CheckSum=Receive_data();
			//I2C_LCD_GotoXY(5,1);
			//I2C_LCD_Puts("A bung");
		//I2C_LCD_write_string_XY(0, 0, "hello world");
		
				 //dht11_temp=DHT11_read().temperature;
				 ////itoa((int)dht11_temp,buffer,10);
				 ////sprintf(buffer,"%d",(int)dht11_temp);
				 //LCD_Gotoxy(1,1);
				 //LCD_Puts("27");
				 //UART_putString("27");
				 //_delay_ms(1000);
		if (cout>=10000)
		{
			 dht11_temp=DHT11_read().temperature;
			 //itoa((int)dht11_temp,buffer,10);
			 sprintf(buffer,"%d",(int)dht11_temp);
			 //LCD_Gotoxy(13,0);
			 //LCD_Puts(buffer);
			 UART_putString((char*)buffer);
			 //UART_putString("27");       
			//if (old_temperator !=(int)dht11_temp)
			//{
				//LCD_Clear();
				//old_temperator =(int) dht11_temp;
				//itoa(old_temperator,buffer,10);
				//LCD_Gotoxy(0,0);
				//LCD_Puts("Temperator:");
				 //LCD_Gotoxy(13,0);
				 //LCD_Puts(buffer);
			//}
			cout=0;
		}
    }
	return 0;
}

ISR(TIMER0_OVF_vect){//ham ngat timer
	TCNT0=130;//luu lai gia tri bien dem
	cout++;
}