/*
 * lcd_4bit.h
 *
 * Created: 02/07/2019 9:59:48 AM
 *  Author: asd
 */ 

#ifndef LCD_4BIT_H_
#define LCD_4BIT_H_
#define lcd DDRB
#define lcd PORTB
#define RS PB0
#define RW PB1
#define EN PB2


void lcd_init () {
	lcd_cmd (0x02);
	lcd_cmd (0x28);
	lcd_cmd (0x06);
	lcd_cmd (0x0E);
	lcd_cmd (0x80);
}

void lcd_cmd (char cmd) {
	lcd = cmd&0xf0;

	lcd &= ~(1<<RS); 
	lcd &= ~(1<<RW);
	lcd |= (1<<EN);
	_delay_ms(10);
	lcd &= ~(1<<EN);
	
	lcd = (cmd<<4)&0xf0;

	lcd &= ~(1<<RS);
	lcd &= ~(1<<RW);
	lcd |= (1<<EN);
	_delay_ms(10);
	lcd &= ~(1<<EN);
	
}

void lcd_data (char data) {
	lcd = data&0xf0;

	lcd |= (1<<RS);
	lcd &= ~(1<<RW);
	lcd |= (1<<EN);
	_delay_ms(10);
	lcd &= ~(1<<EN);
	_delay_ms(10);

	lcd = (data<<4)&0xf0;

	lcd |=(1<<RS);
	lcd &= ~(1<<RW);
	lcd |= (1<<EN);
	_delay_ms(10);
	lcd &= ~(1<<EN);
		
}

void lcd_string(char *str) {
	while(*str != '\0') {
		lcd_data(*str);
		str++;
	}
}


#endif /* LCD_4BIT_H_ */