/*
 * dht11.h
 *
 * Created: 4/22/2022 3:29:22 PM
 *  Author: Bungdz
 */ 


#ifndef DHT11_H_
#define DHT11_H_

#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
//#include <uart.h>
#define DHT11_PIN 0
#define DHT11_PORT PORTB
#define DHT11_DIR DDRB
#define DHT11_PINA PINB
uint8_t c=0;
//char test[5];
enum dht11_status {
	DHT11_CRC_ERROR = -2,
	DHT11_TIMEOUT_ERROR,
	DHT11_OK
};

struct dht11_reading {
	int status;
	int temperature;
	int humidity;
};

//static int64_t last_read_time = -2000000;
static struct dht11_reading last_read;

static int _waitOrTimeout(uint16_t microSeconds, int level) {
	int micros_ticks = 0;
	while((DHT11_PINA & (1<<DHT11_PIN)) == level) {
		if(micros_ticks++ > microSeconds)
		return DHT11_TIMEOUT_ERROR;
		_delay_us(1);
	}
	//itoa(micros_ticks,test,10);
	//UART_putchar((char*)test);
	return micros_ticks;
}

static int _checkCRC(uint8_t data[]) {
	if(data[4] == (data[0] + data[1] + data[2] + data[3]))
	return DHT11_OK;
	else
	return DHT11_CRC_ERROR;
}

static void _sendStartSignal() {
	DHT11_DIR |= (1<<DHT11_PIN);
	DHT11_PORT &= ~(1<<DHT11_PIN);	/* set to low pin */
	_delay_ms(20);			/* wait for 20ms */
	DHT11_PORT |= (1<<DHT11_PIN);	/* set to high pin */
	_delay_us(40);
}

static int _checkResponse() {
	DHT11_DIR &= ~(1<<DHT11_PIN);
	while(DHT11_PINA & (1<<DHT11_PIN));/* DHT send response about 80us then to high voltage*/
	while((DHT11_PINA & (1<<DHT11_PIN))==0);/*doi len muc cao*/
	while(DHT11_PINA & (1<<DHT11_PIN));/* doi xuong muc thap de bat dau gui du lieu*/
	/* Wait for next step ~80us*/
	//if(_waitOrTimeout(90, 0) == DHT11_TIMEOUT_ERROR)
	//return DHT11_TIMEOUT_ERROR;
//
	///* Wait for next step ~80us*/
	//if(_waitOrTimeout(90, 1) == DHT11_TIMEOUT_ERROR)
	//return DHT11_TIMEOUT_ERROR;

	return DHT11_OK;
}

static struct dht11_reading _timeoutError() {
	struct dht11_reading timeoutError = {DHT11_TIMEOUT_ERROR, -1, -1};
	return timeoutError;
}

static struct dht11_reading _crcError() {
	struct dht11_reading crcError = {DHT11_CRC_ERROR, -1, -1};
	return crcError;
}

void DHT11_init(void) {
	/* Wait 1 seconds to make the device pass its initial unstable status */
	_delay_ms(1000);
}

//int64_t esp_timer_get_time(void)
//{
	//return 0;
//}

struct dht11_reading DHT11_read() {
	/* Tried to sense too son since last read (dht11 needs ~2 seconds to make a new read) */
	//if(esp_timer_get_time() - 2000000 < last_read_time) {
		//return last_read;
	//}
//
	//last_read_time = esp_timer_get_time();
	_delay_ms(50);
	uint8_t data[5] = {0,0,0,0,0};

	_sendStartSignal();

	if(_checkResponse() == DHT11_TIMEOUT_ERROR)
	return last_read = _timeoutError();
	
	/* Read response */
	for(int i = 0; i < 40; i++) {
		/* Initial data */
				while((DHT11_PINA & (1<<DHT11_PIN)) == 0);  /* check received bit 0 or 1 , doi len muc cao*/
				_delay_us(30);/*neu sau 30us ma DATA van o muc cao thi do la logic 1*/
				if(DHT11_PINA & (1<<DHT11_PIN))/* if high pulse is greater than 30ms */
				c = (c<<1)|(0x01);	/* then its logic HIGH */
				else			/* otherwise its logic LOW */
				c = (c<<1);
				while(DHT11_PINA & (1<<DHT11_PIN));/*keo xuong doi bit tiep theo*/
				if (i == 7)
				{
					data[0]= c;
					c=0;
				}
				if (i == 15)
				{
					data[1]= c;
					c=0;
				}
				if (i == 23)
				{
					data[2]= c;
					c=0;
				}
				if (i == 31)
				{
					data[3]= c;
					c=0;
				}
				if (i == 49)
				{
					data[4]= c;
					c=0;
				}
		//if(_waitOrTimeout(50, 0) == DHT11_TIMEOUT_ERROR)
		//return last_read = _timeoutError();
		//
		//if(_waitOrTimeout(70, 1) > 30) {
			///* Bit received was a 1 */
			//data[i/8] |= (1 << ((i%8)));
		//}
	}

	//if(_checkCRC(data) != DHT11_CRC_ERROR) {
		last_read.status = DHT11_OK;
		last_read.temperature = data[2];
		last_read.humidity = data[0];
		return last_read;
		//} else {
		//return last_read = _crcError();
	//}
}



#endif /* DHT11_H_ */