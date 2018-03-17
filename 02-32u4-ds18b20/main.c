#define __AVR_ATmega32U4__ 1

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "usb_serial.h"
#include "ds18b20.h"

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

void send_str(char source[]) {
	char* c = source;
	while (*c) {
		usb_serial_putchar(*c++);
	}
}

void init(void) {
	CPU_PRESCALE(0); // I don't know what it does
	usb_init();
	DDRC |= _BV(PC7); // internal led
}

int main(void) {
	init();
	double d = 0;
	char printbuff[10];

	for (;;) {
		d = ds18b20_gettemp();
		dtostrf(d, 3, 2, printbuff);

		if(usb_configured()){// begin USBSerial operation only when USB ready
			usb_serial_flush_input();
			if ((usb_serial_get_control() & USB_SERIAL_DTR)) {
				send_str("Current temp: ");
				send_str(printbuff);
				send_str("\r\n");
			}
		}

		PORTC |= _BV(PC7);
		_delay_ms(50);
		PORTC &= ~_BV(PC7);

		_delay_ms(950);

	}

	return 0;

}
