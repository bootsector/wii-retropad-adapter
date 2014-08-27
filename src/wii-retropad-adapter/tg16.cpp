/*
* Wii RetroPad Adapter - Nintendo Wiimote adapter for retro-controllers!
* Copyright (c) 2011 Bruno Freitas - bruno@brunofreitas.com
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "tg16.h"
#include "digitalWriteFast.h"

void tg16_init(void) {
	// Configure Data pins
	pinModeFast(2, INPUT);
	digitalWriteFast(2, HIGH);

	pinModeFast(4, INPUT);
	digitalWriteFast(4, HIGH);

	pinModeFast(5, INPUT);
	digitalWriteFast(5, HIGH);

	pinModeFast(6, INPUT);
	digitalWriteFast(6, HIGH);

	// Configure Data Select and /OE pins
	pinModeFast(7, OUTPUT);
	digitalWriteFast(7, HIGH);

	pinModeFast(8, OUTPUT);
	digitalWriteFast(8, LOW);
}

int tg16_read(void) {
	int retval = 0;

	// Data Select HIGH
	digitalWriteFast(7, HIGH);

	// /OE LOW
	digitalWriteFast(8, LOW);

	for(int i = 0; i < 2; i++) {
		// /OE LOW
		digitalWriteFast(8, LOW);
		delayMicroseconds(1);

		// If four directions are low, then it's an Avenue6 Pad
		if(!digitalReadFast(2) && !digitalReadFast(4) && !digitalReadFast(5) && !digitalReadFast(6)) {
			// Data Select LOW
			digitalWriteFast(7, LOW);
			delayMicroseconds(1);

			retval |= (!digitalReadFast(2) << 8);  // III
			retval |= (!digitalReadFast(4) << 9);  // IV
			retval |= (!digitalReadFast(5) << 10); // V
			retval |= (!digitalReadFast(6) << 11); // VI
		} else {
			// Normal pad reading
			retval |= (!digitalReadFast(2) << 0); // UP
			retval |= (!digitalReadFast(4) << 1); // RIGHT
			retval |= (!digitalReadFast(5) << 2); // DOWN
			retval |= (!digitalReadFast(6) << 3); // LEFT

			// Data Select LOW
			digitalWriteFast(7, LOW);
			delayMicroseconds(1);

			retval |= (!digitalReadFast(2) << 4); // I
			retval |= (!digitalReadFast(4) << 5); // II
			retval |= (!digitalReadFast(5) << 6); // SELECT
			retval |= (!digitalReadFast(6) << 7); // RUN
		}

		// Data Select HIGH
		digitalWriteFast(7, HIGH);

		// /OE HIGH
		digitalWriteFast(8, HIGH);
	}

	return retval;
}
