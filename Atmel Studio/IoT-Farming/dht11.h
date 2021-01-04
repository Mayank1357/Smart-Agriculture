/*
 * dht11.h
 *
 * Created: 19/04/2020 7:35:34 AM
 *  Author: Asad Ali
 */ 


#ifndef DHT11_H_
#define DHT11_H_
#define dht_port PORTC
#define dht_pin PINC
#define dht_ddr DDRC
#define DHT11_PIN PC5

int c;

void dht_request()				/* Microcontroller sends start pulse/request */
{
	/* This function sends a pulse to start the DHT11 sensor.
	 Below is the sequence for the same
	 	 PARAMETERS: None
	 	 RETURNS: None
	*/
	////////////////////////////////////////////////////////////////////////////////
	
	dht_ddr |= (1<<DHT11_PIN);
	dht_port &= ~(1<<DHT11_PIN);	/* set to low pin */
	_delay_ms(20);					/* wait for 20ms */
	dht_port |= (1<<DHT11_PIN);		/* set to high pin */
}

void dht_response()	
{
	/* This function receives response from DHT11
		 PARAMETERS: None
		 RETURNS: None
	*/
	//////////////////////////////////////////////////////////////////////////////////////
	
	dht_ddr &= ~(1<<DHT11_PIN);
	while(dht_pin & (1<<DHT11_PIN));
	while((dht_pin & (1<<DHT11_PIN))==0);
	while(dht_pin & (1<<DHT11_PIN));
}

uint8_t dht_receive()	
{
	/* This function receives data from DHT11 sensor.
	 Runs a loops, accepting one bit at a time, for a whole byte
		 PARAMETERS: None
		 RETURNS: c, byte of data received
	*/
	////////////////////////////////////////////////////////////////////////////////////////////////
	
	for (int q = 0; q < 8; q++)
	{
		while((dht_pin & (1<<DHT11_PIN)) == 0);  /* check received bit 0 or 1 */

		_delay_us(30);
		
		if(dht_pin & (1<<DHT11_PIN))	/* if high pulse is received after 30ms */
			c = (c<<1)|(0x01);			/* then its logic HIGH */
		else							/* otherwise its logic LOW */
			c = (c<<1);
			
		while(dht_pin & (1<<DHT11_PIN));
	}
	return c;
}




#endif /* DHT11_H_ */