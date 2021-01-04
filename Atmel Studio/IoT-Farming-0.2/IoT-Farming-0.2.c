/*
 * USART.c
 *
 * Created: 16/07/2019 2:20:22 PM
 *  Author: asd
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "lcd_4bit.h"
#include "adc.h"
#include "USART.h"
//#include "dht11.h"


int main()
{
	
	adc_init();

	UART_init();

	
	/*int str[10], i = 0;*/
	char input[20];
	char* a;	
	

	int ec, ph, mois, am_light, temp_soil, I_Temp, D_Temp, I_RH, D_RH/*CheckSum*/;
	char ec_str[10],ec_str2[10],ec_str3[10], ph_str[10], mois_str[10], am_str[10], temp_str[10], i_rh_str[10], d_rh_str[10], i_temp_str[10], d_temp_str[10];
		

	DDRB = 0xFF;
	while(1)
	{
			a = UART_Rstring(input);
			int i = 0;
			while (*a != '\0') {
				input[i++] = *(a++);
			}



		

		/////////////////////////////////////////////// ELECTRICAL CONDUCTIVITY //////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Read Electrical Conductivity for Salinity
		

		ec = adc_read(0);
		ec = ec * 160.0 / 1024.0;					// 160 milli S/m is max EC for soil measurements
		itoa(ec, ec_str, 10);
		
		
		
		
		///////////////////////////////////////////////////// pH VALUE ////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Soil pH value
		PORTB =0b00000001;
		_delay_ms(20);
		ph = adc_read(0);
		ph = (float)ph * 14.0 / 1023;				// 14 is max pH
		itoa(ph, ph_str, 10);


		
		
		////////////////////////////////////////////////// SOIL MOISTURE ////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Soil moisture
		PORTB =0b00000001;
		_delay_ms(50);
		mois = adc_read(0);
		mois = mois * 100.0 / 803;					// we get 615 on max moisture (100%)
		itoa(mois, mois_str, 10);
		

	

		
		////////////////////////////////////////////////// AMBIENT LIGHT ////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Ambient Light
		am_light = adc_read(3);
		am_light = am_light * 1000.0 / 916;			// we get 916 on 1000 LUX
		itoa(am_light, am_str, 10);

		

		////////////////////////////////////////////////// SOIL TEMPERATURE //////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Soil temperature
		PORTB =0b00000011;
		_delay_ms(50);
		temp_soil = adc_read(0);
		temp_soil = temp_soil / 2.01;				// LM35 gives output which is almost double the actual temperature in celsius
		itoa(temp_soil, temp_str, 10);




		////////////////////////////////////////////////// DHT11 ////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//dht_request();						/* send start pulse */
		//dht_response();						/* receive response */
		//I_RH = dht_receive();				/* store first eight bit in I_RH */
		//D_RH = dht_receive();				/* store next eight bit in D_RH */
		//I_Temp = dht_receive();				/* store next eight bit in I_Temp */
		//D_Temp = dht_receive();				/* store next eight bit in D_Temp */
		//CheckSum = dht_receive();			/* store next eight bit in CheckSum */
		
		
		// Relative humidity
		//itoa(I_RH,i_rh_str,10);
		//itoa(D_RH,d_rh_str,10);

		
		// Ambient temperature
		//itoa(I_Temp,i_temp_str,10);
		//itoa(D_Temp,d_temp_str,10);




		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////					
		// Checks input and returns respective reading
		if (strcmp(input, "ec") - 13 == 0) {
			UART_Tstring(strcat(ec_str, " mS/m"));
		} else if (strcmp(input, "ph") - 13 == 0) {
			UART_Tstring(ph_str);
		} else if (strcmp(input, "soil_moisture") - 13 == 0) {
			UART_Tstring(strcat(mois_str, "%"));
		} else if (strcmp(input, "soil_temp") - 13 == 0) {
			UART_Tstring(strcat(temp_str, "C"));
		} else if (strcmp(input, "light") - 13 == 0) {
			UART_Tstring(am_str);
		} 
		
		
	}

}