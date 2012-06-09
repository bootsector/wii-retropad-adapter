/*
* Wii RetroPad Adapter - Nintendo Wiimote adapter for retro-controllers!
* Copyright (c) 2011 Bruno Freitas - bootsector@ig.com.br
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

#include <WProgram.h>
#include "saturn.h"
#include "digitalWriteFast.h"

#define D1 2
#define D0 3
#define S0 4
#define S1 6
#define D3 7
#define D2 8

void saturn_init() {
	pinModeFast(D1, INPUT);
	pinModeFast(D0, INPUT);
	pinModeFast(D3, INPUT);
	pinModeFast(D2, INPUT);
	pinModeFast(S0, OUTPUT);
	pinModeFast(S1, OUTPUT);

	digitalWriteFast(D1, HIGH);
	digitalWriteFast(D0, HIGH);
	digitalWriteFast(D3, HIGH);
	digitalWriteFast(D2, HIGH);
}

int saturn_read() {
	int retval = 0;

	// Reading Z, Y, X and R
	digitalWriteFast(S0, LOW);
	digitalWriteFast(S1, LOW);

	retval |= (!digitalReadFast(D0) << 0); // Z
	retval |= (!digitalReadFast(D1) << 1); // Y
	retval |= (!digitalReadFast(D2) << 2); // X
	retval |= (!digitalReadFast(D3) << 3); // R

	// Reading B, C, A and Start
	digitalWriteFast(S0, HIGH);
	digitalWriteFast(S1, LOW);

	retval |= (!digitalReadFast(D0) << 4); // B
	retval |= (!digitalReadFast(D1) << 5); // C
	retval |= (!digitalReadFast(D2) << 6); // A
	retval |= (!digitalReadFast(D3) << 7); // Start

	// Reading Up, Down, Left, Right
	digitalWriteFast(S0, LOW);
	digitalWriteFast(S1, HIGH);

	retval |= (!digitalReadFast(D0) << 8); // Up
	retval |= (!digitalReadFast(D1) << 9); // Down
	retval |= (!digitalReadFast(D2) << 10); // Left
	retval |= (!digitalReadFast(D3) << 11); // Right

	// Reading L
	digitalWriteFast(S0, HIGH);
	digitalWriteFast(S1, HIGH);

	retval |= (!digitalReadFast(D3) << 12); // L

	return retval;
}
