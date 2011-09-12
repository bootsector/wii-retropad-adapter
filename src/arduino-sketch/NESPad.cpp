/*
  NESPad - Nintendo Entertainment System (NES) pad library for Arduino
  Copyright (c) 2010 Bruno Freitas - bootsector@ig.com.br
  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <WProgram.h>
#include "NESPad.h"

NESPad::NESPad() {
	this->setup(-1, -1, -1);
}

NESPad::NESPad(int clockPin, int latchPin, int dataPin) {
	this->setup(clockPin, latchPin, dataPin);
}

void NESPad::setup(int clockPin, int latchPin, int dataPin) {
	_clockPin = clockPin;
	_latchPin = latchPin;
	_dataPin = dataPin;

	nespadstate = 0;

	pinMode(_clockPin, OUTPUT);
	pinMode(_latchPin, OUTPUT);
	pinMode(_dataPin, INPUT);
}

int NESPad::read(int bits) {
	int state, i;

	if (_dataPin < 0)
		return 0;

	digitalWrite(_latchPin, LOW);
	digitalWrite(_clockPin, LOW);

	digitalWrite(_latchPin, HIGH);
	delayMicroseconds(1);
	digitalWrite(_latchPin, LOW);

	state = digitalRead(_dataPin);

	for (i = 1; i < bits; i++) {
		digitalWrite(_clockPin, HIGH);
		delayMicroseconds(1);
		digitalWrite(_clockPin, LOW);

		state = state | (digitalRead(_dataPin) << i);
	}

	nespadstate = ~state;

	return (int) nespadstate;
}
