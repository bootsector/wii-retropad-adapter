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

/*
NES Controller pinouts:
----------------------

           +---+
GND      - | O  \
CLOCK    - | O O + - +5V
LATCH    - | O O | - NC
DATA OUT - | O O | - NC
           +-----+


NES Pad read data format:
--------------------
      +-------+------+------+----+-------+--------+---+---+
Bit:  |   7   |   6  |   5  |  4 |   3   |    2   | 1 | 0 |
      +-------+------+------+----+-------+--------+---+---+
Data: | RIGHT | LEFT | DOWN | UP | START | SELECT | B | A |
      +-------+------+------+----+-------+--------+---+---+


SNES Clock pulse and corresponding button

1   B
    Y
    Select
    Start
    North
    South
    West
    East
    A
    X
    L
    R
    [no button, always high]
    [no button, always high]
    [no button, always high]
16  [no button, always high]
*/

#ifndef NESPAD_H_
#define NESPAD_H_

#include <WProgram.h>

class NESPad {

private:
	int _clockPin;
	int _latchPin;
	int _dataPin;
	unsigned char nespadstate;

public:
	NESPad();
	NESPad(int clockPin, int latchPin, int dataPin);

	void setup(int clockPin, int latchPin, int dataPin);
	int read(int bits);
};

#endif /* NESPAD_H_ */
