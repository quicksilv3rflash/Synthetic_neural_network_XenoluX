/*
 * xenolux_universe_control_unit.c
 *
 * Created: 2015-11-26 09:09:02
 *  Author: Ondine
 */ 


/*
PIN CONNECTIONS BETWEEN AVR AND FPGA
PIND0-PIND5 are connected to nerve_tap_from_layer_0 - nerve_tap_from_layer_5

*/
#define F_CPU 1000000

#include <avr/io.h>
#include <util/delay.h>

uint8_t scan_nervetaps(void){
	uint8_t nervetapstatus;
	nervetapstatus = PIND;
	nervetapstatus &= 0x3F;
	if(nervetapstatus > 0){
		PORTB |= 10;
		_delay_us(1);
		PORTB &= 0xEF;
		_delay_us(1);
	}
	return nervetapstatus;
}

int main(void){
	DDRB = 0xFF;
	DDRD = 0x00;
	PORTB = 0x00;
	while(1){
		//scan_nervetaps();
		PORTB= 0xFF;
		_delay_us(1);
		PORTB = 0x00;
		_delay_us(1);
	}
	
}