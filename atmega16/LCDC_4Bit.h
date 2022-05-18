/*
 * uart.h
 *
 * Created: 4/22/2022 3:29:36 PM
 *  Author: Bungdz
 */ 


#ifndef LCD_4BIT_H_
#define LCD_4BIT_H_


#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <stdlib.h>
#include <stdio.h>
#define F_CPU 8000000UL

#define RS_Pin PA0
#define RS_GPIO_Port PORTA
#define EN_Pin PA1
#define EN_GPIO_Port PORTA
#define D4_Pin PINA2
#define D4_GPIO_Port PORTA
#define D5_Pin PINA3
#define D5_GPIO_Port PORTA
#define D6_Pin PINA4
#define D6_GPIO_Port PORTA
#define D7_Pin PINA5
#define D7_GPIO_Port PORTA

void LCD_Enable(void)
{
	//PORTA&=~RS_Pin;
	PORTA|=EN_Pin;
	_delay_ms(1);
	PORTA&=~EN_Pin;
	_delay_ms(3);	
}

void LCD_Send4Bit(unsigned char data)
{
	//DDRA |= RS_Pin | EN_Pin | D4_Pin | D5_Pin | D6_Pin | D7_Pin;
	//PORTA|=(Data&0x01)?D4_Pin:(~D4_Pin);
	//PORTA|=((Data>>1)&0x01)?D5_Pin:(~D5_Pin);
	//PORTA|=((Data>>2)&0x01)?D6_Pin:(~D6_Pin);
	//PORTA|=((Data>>3)&0x01)?D7_Pin:(~D7_Pin);
	PORTA = (PORTA & 0x0F) | (data & 0xF0); /* sending upper nibble */
	PORTA |= (1<<RS_Pin);		/* RS=1, data reg. */
	PORTA|= (1<<EN_Pin);
	_delay_us(1);
	PORTA &= ~ (1<<EN_Pin);

	_delay_us(200);

	PORTA = (PORTA & 0x0F) | (data << 4); /* sending lower nibble */
	PORTA |= (1<<EN_Pin);
	_delay_us(1);
	PORTA &= ~ (1<<EN_Pin);
	_delay_ms(2);
}

void LCD_SendCommand(unsigned char command)
{
	//LCD_Send4Bit(command >>4);/* Gui 4 bit cao */
	//LCD_Enable();
	//LCD_Send4Bit(command);	/* Gui 4 bit thap*/
	//LCD_Enable();
		PORTA = (PORTA & 0x0F) | (command & 0xF0); /* sending upper nibble */
		PORTA &= ~(1<<RS_Pin);		/* RS=1, data reg. */
		PORTA|= (1<<EN_Pin);
		_delay_us(1);
		PORTA &= ~ (1<<EN_Pin);

		_delay_us(200);

		PORTA = (PORTA & 0x0F) | (command << 4); /* sending lower nibble */
		PORTA |= (1<<EN_Pin);
		_delay_us(1);
		PORTA &= ~ (1<<EN_Pin);
		_delay_ms(2);
}
/* USER CODE END 0 */
void LCD_Clear(void)
{
 	LCD_SendCommand(0x01);  
	_delay_ms(1);
	LCD_SendCommand(0x80);  
}

void LCD_Init(void)
{
	DDRA =0xFF;
	//LCD_SendCommand(0x00);
	//PORTA&=~RS_Pin;
	_delay_ms(20);
	//LCD_SendCommand(0x03);
	//LCD_Enable();
	//LCD_Enable();
	//LCD_Enable();
	LCD_SendCommand(0x02);
	//LCD_Enable();
	LCD_SendCommand(0x28); // giao thuc 4 bit, hien thi 2 hang, ki tu 5x8
	LCD_SendCommand(0x0C); // cho phep hien thi man hinh
	LCD_SendCommand(0x06); // tang ID, khong dich khung hinh
	LCD_SendCommand(0x01); // xoa toan bo khung hinh
	LCD_SendCommand(0x80);
}

void LCD_Gotoxy(unsigned char x, unsigned char y)
{
	unsigned char address;
  	if(!y)address=(0x80+x);
  	else address=(0xC0+x);
  	LCD_SendCommand(address);

}

void LCD_PutChar(unsigned char Data)
{
  //PORTA|=(RS_Pin);
 // PORTA|=EN_Pin;
  LCD_Send4Bit(Data);
  //PORTA&=~(RS_Pin);
}

void LCD_Puts(char *s)
{
   while (*s){
      	LCD_PutChar(*s);
     	s++;
   	}
}

#endif /* UART_H_ */