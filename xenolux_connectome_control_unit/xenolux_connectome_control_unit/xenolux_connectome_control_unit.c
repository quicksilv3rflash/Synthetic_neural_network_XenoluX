/*
 * xenolux_connectome_control_unit.c
 *
 * Created: 2015-10-28 13:10:29
 *  Author: Ondine
 * all hail machine empire
 */ 

/*
PIN CONNECTIONS BETWEEN AVR AND FPGA
//PORTB0-PORTB3 are connected to mux_status_bit0 - mux_status_bit_3
//PORTC0-PORTC2 are connected to mux_address_bit_0 - mux_address_bit_2
//PORTC4-PORTC6 are connected to layer_address_bit_0 - layer_address_bit_2
//PORTC7 is connected to PIN_28
//PORTD0-PORTD2 are connected to neuron_address_bit_0 - neuron_address_bit_2
*/
#define F_CPU 1000000

#include <avr/io.h>
#include <util/delay.h>



/* 
synapsemuxconfigure() is used to configure an individual synapse multiplexer.
It takes the following inputs:
layer (0 to 5); which 6-neuron layer of the network is being addressed
neuron (0 to 5); which of the 6 neurons in the layer is being addressed
synapse (-6, -3, -1, 1, 3, 6); which synapse of the addressed neuron is to be modified:
	- is inhibitory, + is excitatory
	1: 17%, 3: 50%, 6: 100%
spliceto (0 to 7); 0-5 are axons in from previous layer, 6 is null, 7 is stim line
*/
int splice(uint8_t layer, uint8_t neuron, int8_t synapse, uint8_t spliceto){
	uint8_t muxaddress = 0x00;
	uint8_t muxstatus = 0x00;
	uint8_t neuronaddress = 0x00;
	uint8_t layeraddress = 0x00;
	//yes I know [switch : case] statements exist but for some reason they weren't compiling right...
	//set up layer address bits
	if(layer == 0){
		layeraddress = 0x02;
	}
	if(layer == 1){
		layeraddress = 0x06;
	}
	if(layer == 2){
		layeraddress = 0x01;
	}
	if(layer == 3){
		layeraddress = 0x05;
	}
	if(layer == 4){
		layeraddress = 0x03;
	}
	if(layer == 5){
		layeraddress = 0x07;
	}
	//set up neuron address bits
	if(neuron == 0){
		neuronaddress = 0x02;
	}
	if(neuron == 1){
		neuronaddress = 0x06;
	}
	if(neuron == 2){
		neuronaddress = 0x01;
	}
	if(neuron == 3){
		neuronaddress = 0x05;
	}
	if(neuron == 4){
		neuronaddress = 0x03;
	}
	if(neuron == 5){
		neuronaddress = 0x07;
	}
	//set up mux address bits
	if(synapse == -6){
		muxaddress = 0x03;
	}
	if(synapse == -3){
		muxaddress = 0x01;
	}
	if(synapse == -1){
		muxaddress = 0x02;
	}
	if(synapse == 1){
		muxaddress = 0x06;
	}
	if(synapse == 3){
		muxaddress = 0x05;
	}
	if(synapse == 6){
		muxaddress = 0x07;
	}
	//set up mux status bits
	if(spliceto == 0){
		muxstatus = 0x0D;
	}
	if(spliceto == 1){
		muxstatus = 0x05;
	}
	if(spliceto == 2){
		muxstatus = 0x0F;
	}
	if(spliceto == 3){
		muxstatus = 0x07;
	}
	if(spliceto == 4){
		muxstatus = 0x0B;
	}
	if(spliceto == 5){
		muxstatus = 0x03;
	}
	if(spliceto == 6){
		muxstatus = 0x00;
	}
	if(spliceto == 7){
		muxstatus = 0x01;
	}
	
		
	PORTC = 0x00;	
	//PORTC0-PORTC2 are connected to mux_address_bit_0 - mux_address_bit_2
	PORTC |= muxaddress;
	//PORTC4-PORTC6 are connected to layer_address_bit_0 - layer_address_bit_2
	PORTC |= (layeraddress << 4);
	//PORTB0-PORTB3 are connected to mux_status_bit0 - mux_status_bit_3
	PORTB = muxstatus;
	//PORTD0-PORTD2 are connected to neuron_address_bit_0 - neuron_address_bit_2
	PORTD = neuronaddress;
	
	//wait and then flag mux_write_enable (PORTC3)
	_delay_ms(1);
	PORTC |= 0x08;
	_delay_ms(1);
	//clear mux_write_enable (PORTC3)
	PORTC &= 0xF7;
	//then clear everything
	PORTC = 0x00;
	PORTB = 0x00;
	PORTD = 0x00; 
	
	return 0;
}



int main(void){
	//set used ports (B,C,D) to output
	DDRB = ~0;
	DDRC = ~0;
	DDRD = ~0;
	


	PORTD = 0x00;


//debug notes: splicing layer 5's neuron 3 [1/2 exc aka "3" synapse] to layer 4's axon out 0 kills oscillation
//when L5N3S1-CHCLK. However, splicing L5N3S3-L4N3 does not. 
//L5N3S3-L4N1 kills
//L5N3S3-L4N0 kills
//L5N3S3-L4N2 kills
//L5N3S3-L4N3 ok
//L5N3S3-L4N4 ok
//L5N3S3-L4N5 ok
		splice(5,3,-6,0);
		splice(5,3,-3,6);
		splice(5,3,-1,6);
		splice(5,3,1,7);
		splice(5,3,3,6);
		splice(5,3,6,6);
		
		splice(4,0,-6,6);
		splice(4,0,-3,6);
		splice(4,0,-1,6);
		splice(4,0,1,6);
		splice(4,0,3,7);
		splice(4,0,6,6);


    while(1){

    }
}