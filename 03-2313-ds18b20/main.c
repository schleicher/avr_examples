#include <avr/io.h>
#include <util/delay.h>
#include "ds18b20.h"

void init(void) {
	DDRB |= _BV(PB0); // white led connected to pin 9 - status led
	DDRB |= _BV(PB1); // blue led connected to pin 10 - low temp signal
	DDRB |= _BV(PB2); // red led connected to pin 11 - high temp signal
}

int main(void) {
	init();
	double t = 0; // holds last measured temperature

	for (;;) {
		t = ds18b20_gettemp();
		if (t <= 24) {
			// low temp
			PORTB |= _BV(PB1); // turn blue led on
			PORTB &= ~_BV(PB2); // turn red led off
		} else if (t >= 27) {
			// high temp
			PORTB |= _BV(PB2); // red led on
			PORTB &= ~_BV(PB1); // red led off
		} else {
			// normal temp - both leds off
			PORTB &= ~_BV(PB1);
			PORTB &= ~_BV(PB2);
		}
		// pulse - system works
		PORTB |= _BV(PB0);
		_delay_ms(10);
		PORTB &= ~_BV(PB0);

		_delay_ms(1990);
	}
	return 0;
}
