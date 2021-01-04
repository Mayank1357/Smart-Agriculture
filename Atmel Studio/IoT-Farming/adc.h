/*
 * adc.h
 *
 * Created: 19/04/2020 7:33:54 AM
 *  Author: Asad Ali
 */ 


#ifndef ADC_H_
#define ADC_H_


int adc_init()
{
	/* This function initializes ADC 
		 PARAMETERS: None
		 RETURNS: None
	*/
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	ADMUX |= (1 << REFS0);													// setting AREF = Vcc 
	
	ADCSRA |= (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);		// using 128 prescaler
	
}

int adc_read(uint8_t pin)
{
	/* This function reads data from analog pin 
		 PARAMETERS: pin, input analog pin on board
		 RETURNS: digital data received. Stored in ADC register
	*/
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	ADMUX = (ADMUX & 0xf8) | pin; //changes last 3 bits only
	
	ADCSRA |= (1 << ADSC);
	
	while (ADCSRA & (1 << ADSC));
	
	return (ADC);
}


#endif /* ADC_H_ */