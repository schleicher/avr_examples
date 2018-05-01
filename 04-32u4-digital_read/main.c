#define __AVR_ATmega32U4__ 1

#include <avr/io.h>
#include <util/delay.h>
//#include <stdlib.h>
//#include "usb_serial.h"

// #define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

/*
void send_str(char source[]) {
	char* c = source;
	while (*c) {
		usb_serial_putchar(*c++);
	}
}
*/

void init(void) {
	// CPU_PRESCALE(0); // I don't know what it does
	// usb_init();
	DDRC |= _BV(PC7); // internal led

	ADMUX = 0b00000111; // select adc7 as input
	ADMUX |= (1 << REFS0); // Set ADC reference to AVCC
	ADMUX &= ~(1 << ADLAR);   // clear for 10 bit resolution

	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescaler to 128 - 125KHz sample rate @ 16MHz
	ADCSRA |= (1 << ADEN);    // Enable the ADC
}

int main(void) {
	init();
	int ADCval;
	// char str[16]; // string to usb output

	for (;;) {

		// get analog data
		ADCSRA |= (1 << ADSC);    // Start the ADC conversion
		while(ADCSRA & (1 << ADSC));      // this line waits for the ADC to finish
		// convert to 10 bit value
		ADCval = ADCL;
		ADCval = (ADCH << 8) + ADCval;

		/*
		if(usb_configured()){// begin USBSerial operation only when USB ready
			usb_serial_flush_input();
			if ((usb_serial_get_control() & USB_SERIAL_DTR)) {
				send_str("Current lala: ");
				itoa(ADCval, str, 10); // value to string
				send_str(str);
				send_str("\r\n");
			}
		}
		*/

		if(ADCval > 250){
			PORTC |= _BV(PC7);
		} else {
			PORTC &= ~_BV(PC7);
		}

		_delay_ms(500);
	}

	return 0;

}
