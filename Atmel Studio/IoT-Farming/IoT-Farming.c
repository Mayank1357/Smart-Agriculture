/*
 * IoT-Farming.c
 *
 * Created: 18/04/2020 4:24:02 PM
 *  Author: Asad Ali
 */ 

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "lcd_4bit.h"
#include "adc.h"
#include "dht11.h"


int main(void)
{
	int ec, ph, mois, am_light, temp_soil;
	char str[20];							// buffer used when converting integer data to string

	DDRB = 0xff;							// for LCD
	lcd_init();
	
	adc_init();								// initialize ADC
	
	lcd_cmd(0x0C);							// clear cursor
	
	
    while(1)
    {
		/////////////////////////////////////////////// ELECTRICAL CONDUCTIVITY //////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Read Electrical Conductivity for Salinity
		lcd_string("EC: ");
		ec = adc_read(0);
		ec = ec * 160.0 / 1024.0;					// 160 milli S/m is max EC for soil measurements
		
		itoa(ec, str, 10);
		lcd_cmd(0x8F);								// Move to first line, 20th column
		lcd_string(str);
		lcd_cmd(0xC0);								// Move to second line

				
		
		///////////////////////////////////////////////////// pH VALUE ////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Soil pH value
		lcd_string("Soil pH: ");
		ph = adc_read(1);
		ph = (float)ph * 14.0 / 1023;				// 14 is max pH
		
		itoa(ph, str, 10);
		lcd_cmd(0xCF);								// Move to second line, 20th column
		lcd_string(str);
		lcd_cmd(0x94);								// Move to third line

		
		
		////////////////////////////////////////////////// SOIL MOISTURE ////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
		// Soil moisture
		lcd_string("Soil Moisture: ");
		mois = adc_read(2);
		mois = mois * 100.0 / 615;					// we get 615 on max moisture (100%)
		
		itoa(mois, str, 10);
		lcd_cmd(0xA3);								// Move to third line, 20th column
		lcd_string(str);
		lcd_cmd(0xD4);								// Move to fourth line

		
		
		////////////////////////////////////////////////// AMBIENT LIGHT ////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
		// Ambient Light
		lcd_string("Ambient light: ");
		am_light = adc_read(3);
		am_light = am_light * 1000.0 / 916;			// we get 916 on 1000 LUX
		
		itoa(am_light, str, 10);
		lcd_cmd(0xE3);								// Move to fourth line, 20th column
		lcd_string(str);
				
		_delay_ms(1000);							// wait for 1s and display the above four lines
		lcd_cmd(0x01);								// clear screen for more data
	
		
		
		////////////////////////////////////////////////// SOIL TEMPERATURE //////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Soil temperature
		lcd_string("Soil temp: ");
		temp_soil = adc_read(4);
		temp_soil = temp_soil / 2.01;				// LM35 gives output which is almost double the actual temperature in celsius
		
		itoa(temp_soil, str, 10);
		lcd_cmd(0x8F);								// Move to line 1, 20th column
		lcd_string(str);
		lcd_cmd(0xC0);								// Move to second line
				


		////////////////////////////////////////////////// DHT11 ////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		read_and_print_dht();

		
		_delay_ms(1000);							// wait 1s to display the above lines 
		lcd_cmd(0x01);								// clear screen
    }
}

void read_and_print_dht() {
	/*	This function reads and displays ambient temperature and humidity
		 from DHT11 sensor
			PARAMETERS: None
			RETURNS: None		
	*/
	int I_Temp, D_Temp, I_RH, D_RH, CheckSum;
	char data[20];						// buffer for value returned by dht11
		
	dht_request();						/* send start pulse */
	dht_response();						/* receive response */
	I_RH = dht_receive();				/* store first eight bit in I_RH */
	D_RH = dht_receive();				/* store next eight bit in D_RH */
	I_Temp = dht_receive();				/* store next eight bit in I_Temp */
	D_Temp = dht_receive();				/* store next eight bit in D_Temp */
	CheckSum = dht_receive();			/* store next eight bit in CheckSum */
				
	// if checksum is not equal to the sum of values it is an error
	
	if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum) {		
		lcd_string("Error");
	}
				
	else
	{
		////////////////////////////////////////////////// RELATIVE HUMIDITY /////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		itoa(I_RH,data,10);
					
		lcd_string("Humidity:");
		
		lcd_cmd(0xCF);										// move to line 2, 20th column 
		lcd_string(data);									// print integer data
		lcd_string(".");
					
		itoa(D_RH,data,10);
		lcd_string(data);									// print decimal data
		lcd_string("%");



		////////////////////////////////////////////////// AMBIENT TEMPERATURE////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		itoa(I_Temp,data,10);
		
		lcd_cmd(0x94);										// go to line 3
		lcd_string("Ambient Temp.:");
		lcd_cmd(0xA3);										// go to line 3, 20th column
		lcd_string(data);									// print integer data
		lcd_string(".");

		itoa(D_Temp,data,10);
		lcd_string(data);									// print decimal data
		lcd_string("C");
		
	}

}