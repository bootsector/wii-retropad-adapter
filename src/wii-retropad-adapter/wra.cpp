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

#include "WMExtension.h"
#include "PS2X_lib.h"
#include "genesis.h"
#include "NESPad.h"
#include "digitalWriteFast.h"

// Define if the extra port is a SNES or a PSX connector. Uncomment for SNES.
//#define SNES_WRA

// Define if we're using WMExtension's callback function for buttons update.
//#define ENABLE_BUTTONS_CALLBACK

// Main pad loop. Points to the loop function of the selected pad (via Mode jumpers)
void (*pad_loop)(void) = NULL;

// Classic Controller Buttons
int bdl = 0; // D-Pad Left state
int bdr = 0; // D-Pad Right state
int bdu = 0; // D-Pad Up state
int bdd = 0; // D-Pad Down state
int ba = 0; // A button state
int bb = 0; // B button state
int bx = 0; // X button state
int by = 0; // Y button state
int bl = 0; // L button state
int br = 0; // R button state
int bm = 0; // MINUS button state
int bp = 0; // PLUS button state
int bhome = 0; // HOME button state
int bzl = 0; // ZL button state
int bzr = 0; // ZR button state

// Analog Buttons
byte lx = WMExtension::get_calibration_byte(2)>>2;
byte ly = WMExtension::get_calibration_byte(5)>>2;
byte rx = WMExtension::get_calibration_byte(8)>>3;
byte ry = WMExtension::get_calibration_byte(11)>>3;

// PS Pad neutral radius
#define PSPAD_NEUTRAL_RADIUS 10

// Operation Mode pins
#define PINMODE1 9
#define PINMODE2 10

#ifdef ENABLE_BUTTONS_CALLBACK
// Wiimote button data callback
void button_data_callback() {
	WMExtension::set_button_data(bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
			bm, bp, bhome, lx, ly, rx, ry, bzl, bzr);
}
#endif

/*
 * Return selected pad mode:
 * 1 - NES (7-Pin connector)
 * 2 - Genesis / Master System / Atari (DB9)
 * 3 - PSX / SNES (Extra)
 */
int getPadMode() {
	int mode;

	mode = (digitalReadFast(PINMODE1) << 1) | digitalReadFast(PINMODE2);

	return mode;
}

// Genesis pad loop
void genesis_loop() {
	int button_data;

	genesis_init();

	for (;;) {
		button_data = genesis_read();

		bdl = button_data & GENESIS_LEFT;
		bdr = button_data & GENESIS_RIGHT;
		bdu = button_data & GENESIS_UP;
		bdd = button_data & GENESIS_DOWN;
		ba = button_data & GENESIS_C;
		bb = button_data & GENESIS_B;
		bx = button_data & GENESIS_Y;
		by = button_data & GENESIS_A;
		bl = button_data & GENESIS_X;
		br = button_data & GENESIS_Z;
		bm = button_data & GENESIS_MODE;
		bp = button_data & GENESIS_START;
		bhome = (bdu && bp); // UP + START == HOME

#ifndef ENABLE_BUTTONS_CALLBACK
		WMExtension::set_button_data(bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
					bm, bp, bhome, lx, ly, rx, ry, bzl, bzr);
#endif
	}
}

// NES pad loop
void nes_loop() {
	int button_data;

	NESPad::init();

	for (;;) {

		button_data = NESPad::read(8);

		bdl = button_data & 64;
		bdr = button_data & 128;
		bdu = button_data & 16;
		bdd = button_data & 32;
		ba = button_data & 1;
		bb = button_data & 2;
		bm = button_data & 4;
		bp = button_data & 8;
		bhome = (bm && bp); // START + SELECT == HOME

#ifndef ENABLE_BUTTONS_CALLBACK
		WMExtension::set_button_data(bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
					bm, bp, bhome, lx, ly, rx, ry, bzl, bzr);
#endif

	}
}

#ifdef SNES_WRA

// SNES pad loop
void snes_loop() {
	int button_data;

	NESPad::init();

	for (;;) {
		button_data = NESPad::read(16);

		bdl = button_data & 64;
		bdr = button_data & 128;
		bdu = button_data & 16;
		bdd = button_data & 32;
		bb = button_data & 1;
		by = button_data & 2;
		bm = button_data & 4;
		bp = button_data & 8;
		ba = button_data & 512;
		bx = button_data & 1024;
		bl = button_data & 2048;
		br = button_data & 4096;
		bhome = (bm && bp); // START + SELECT == HOME

#ifndef ENABLE_BUTTONS_CALLBACK
		WMExtension::set_button_data(bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
					bm, bp, bhome, lx, ly, rx, ry, bzl, bzr);
#endif
	}
}

#else

// PSX pad loop
void psx_loop() {
	PS2X psPad;

	byte center_lx, center_ly, center_rx, center_ry;
	byte _lx, _ly, _rx, _ry;

	byte clx = WMExtension::get_calibration_byte(2)>>2;
	byte cly = WMExtension::get_calibration_byte(5)>>2;
	byte crx = WMExtension::get_calibration_byte(8)>>3;
	byte cry = WMExtension::get_calibration_byte(11)>>3;

	while (psPad.config_gamepad(5, 3, 4, 2, false, false) != 0)
		;

	psPad.read_gamepad();

	center_lx = psPad.Analog(PSS_LX)/4;
	center_ly = psPad.Analog(PSS_LY)/4;
	center_rx = psPad.Analog(PSS_RX)/8;
	center_ry = psPad.Analog(PSS_RY)/8;

	for (;;) {
		psPad.read_gamepad();

		bdl = psPad.Button(PSB_PAD_LEFT);
		bdr = psPad.Button(PSB_PAD_RIGHT);
		bdu = psPad.Button(PSB_PAD_UP);
		bdd = psPad.Button(PSB_PAD_DOWN);

		by = psPad.Button(PSB_SQUARE);
		bb = psPad.Button(PSB_CROSS);
		bx = psPad.Button(PSB_TRIANGLE);
		ba = psPad.Button(PSB_CIRCLE);

		bl = psPad.Button(PSB_L1);
		br = psPad.Button(PSB_R1);

		bzl = psPad.Button(PSB_L2);
		bzr = psPad.Button(PSB_R2);

		bm = psPad.Button(PSB_SELECT);
		bp = psPad.Button(PSB_START);

		bhome = (bzl && bzr);

		_lx = psPad.Analog(PSS_LX)/4; //psPad.Analog(PSS_LX)>>2;
		_ly = psPad.Analog(PSS_LY)/4; //psPad.Analog(PSS_LY)>>2;
		_rx = psPad.Analog(PSS_RX)/8; //psPad.Analog(PSS_RX)>>3;
		_ry = psPad.Analog(PSS_RY)/8; //psPad.Analog(PSS_RY)>>3;

		if(_lx >= (center_lx - PSPAD_NEUTRAL_RADIUS) && _lx <= (center_lx + PSPAD_NEUTRAL_RADIUS)) {
			_lx = clx;
		}

		if(_ly >= (center_ly - PSPAD_NEUTRAL_RADIUS) && _ly <= (center_ly + PSPAD_NEUTRAL_RADIUS)) {
			_ly = cly;
		}


		if(_rx >= (center_rx - PSPAD_NEUTRAL_RADIUS) && _rx <= (center_rx + PSPAD_NEUTRAL_RADIUS)) {
			_rx = crx;
		}

		if(_ry >= (center_ry - PSPAD_NEUTRAL_RADIUS) && _ry <= (center_ry + PSPAD_NEUTRAL_RADIUS)) {
			_ry = cry;
		}

		lx = _lx;
		ly = ~_ly;
		rx = _rx;
		ry = ~_ry;

#ifndef ENABLE_BUTTONS_CALLBACK
		WMExtension::set_button_data(bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
					bm, bp, bhome, lx, ly, rx, ry, bzl, bzr);
#endif
	}
}

#endif

void setup() {
	// Set mode pins as input, turning pull-ups on
	pinModeFast(PINMODE1, INPUT);
	digitalWriteFast(PINMODE1, HIGH);

	pinModeFast(PINMODE2, INPUT);
	digitalWriteFast(PINMODE2, HIGH);

	// Prepare wiimote communications
#ifdef ENABLE_BUTTONS_CALLBACK
	WMExtension::set_button_data_callback(button_data_callback);
#endif
	WMExtension::init();

	// Select pad loop based on selected mode
	switch (getPadMode()) {
	case 1:
		pad_loop = nes_loop;
		break;
	case 2:
		pad_loop = genesis_loop;
		break;
	case 3:
#ifdef SNES_WRA
		pad_loop = snes_loop;
#else
		pad_loop = psx_loop;
#endif

		break;
	}
}

void loop() {
	if (pad_loop) {
		pad_loop();
	}
}
