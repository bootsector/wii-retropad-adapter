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
#include "saturn.h"
#include "NESPad.h"
#include "GCPad.h"
#include "digitalWriteFast.h"

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

// Analog stick neutral radius
#define ANALOG_NEUTRAL_RADIUS 10

// WRA 2.0 now uses last 3 DB9 pins in order to detect the extension cable used.
#define DETPIN1 6
#define DETPIN2 7
#define DETPIN3 8

// Possible values (as of today) returned by the detectPad() routine
#define PAD_GENESIS	0b111
#define PAD_NES 	0b110
#define PAD_SNES 	0b101
#define PAD_PS2 	0b100
#define PAD_GC	 	0b011
#define PAD_N64		0b010
#define PAD_NEOGEO	0b001

/*
 * This is the new auto-detect function (non jumper based) which detects the extension
 * cable plugged in the DB9 port. It uses last 3 data pins from DB9 (6, 7 and 9) for
 * the detection.
 *
 * 111 - Sega Genesis (Default)
 * 110 - NES
 * 101 - SNES
 * 100 - PS2
 * 011 - Game Cube
 * 010 - Nintendo 64
 * 001 - Neo Geo
 */
int detectPad() {
	int pad;

	pad = (digitalReadFast(DETPIN1) << 2) | digitalReadFast(DETPIN2) << 1 | digitalReadFast(DETPIN3);

	return pad;
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

		WMExtension::set_button_data(bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
				bm, bp, bhome, lx, ly, rx, ry, bzl, bzr);
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
		bhome = (bm && bp); // SELECT + START == HOME

		WMExtension::set_button_data(bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
				bm, bp, bhome, lx, ly, rx, ry, bzl, bzr);
	}
}

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
		ba = button_data & 256;
		bx = button_data & 512;
		bl = button_data & 1024;
		br = button_data & 2048;
		bhome = (bm && bp); // SELECT + START == HOME

		WMExtension::set_button_data(bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
				bm, bp, bhome, lx, ly, rx, ry, bzl, bzr);
	}
}

// PS2 pad loop
void ps2_loop() {
	PS2X psPad;

	byte center_lx, center_ly, center_rx, center_ry;
	byte _lx, _ly, _rx, _ry;

	byte clx = WMExtension::get_calibration_byte(2)>>2;
	byte cly = WMExtension::get_calibration_byte(5)>>2;
	byte crx = WMExtension::get_calibration_byte(8)>>3;
	byte cry = WMExtension::get_calibration_byte(11)>>3;

	while (psPad.config_gamepad(5, 3, 4, 2, false, false) == 1);

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

		bhome = (bm && bp); // SELECT + START == HOME

		_lx = psPad.Analog(PSS_LX)/4; //psPad.Analog(PSS_LX)>>2;
		_ly = psPad.Analog(PSS_LY)/4; //psPad.Analog(PSS_LY)>>2;
		_rx = psPad.Analog(PSS_RX)/8; //psPad.Analog(PSS_RX)>>3;
		_ry = psPad.Analog(PSS_RY)/8; //psPad.Analog(PSS_RY)>>3;

		if(_lx >= (center_lx - ANALOG_NEUTRAL_RADIUS) && _lx <= (center_lx + ANALOG_NEUTRAL_RADIUS)) {
			_lx = clx;
		}

		if(_ly >= (center_ly - ANALOG_NEUTRAL_RADIUS) && _ly <= (center_ly + ANALOG_NEUTRAL_RADIUS)) {
			_ly = cly;
		}


		if(_rx >= (center_rx - ANALOG_NEUTRAL_RADIUS) && _rx <= (center_rx + ANALOG_NEUTRAL_RADIUS)) {
			_rx = crx;
		}

		if(_ry >= (center_ry - ANALOG_NEUTRAL_RADIUS) && _ry <= (center_ry + ANALOG_NEUTRAL_RADIUS)) {
			_ry = cry;
		}

		lx = _lx;
		ly = ~_ly;
		rx = _rx;
		ry = ~_ry;

		WMExtension::set_button_data(bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
					bm, bp, bhome, lx, ly, rx, ry, bzl, bzr);
	}
}

void gc_loop() {
	byte *button_data;

	byte center_lx, center_ly, center_rx, center_ry;
	byte _lx, _ly, _rx, _ry;

	byte clx = WMExtension::get_calibration_byte(2)>>2;
	byte cly = WMExtension::get_calibration_byte(5)>>2;
	byte crx = WMExtension::get_calibration_byte(8)>>3;
	byte cry = WMExtension::get_calibration_byte(11)>>3;

	GCPad_init();

	button_data = GCPad_read();

	center_lx = button_data[2]/4;
	center_ly = button_data[3]/4;
	center_rx = button_data[4]/8;
	center_ry = button_data[5]/8;

	for(;;) {
		button_data = GCPad_read();

		bdl = button_data[1] & 0x01;
		bdr = button_data[1] & 0x02;
		bdu = button_data[1] & 0x08;
		bdd = button_data[1] & 0x04;

		by = button_data[0] & 0x08;
		bb = button_data[0] & 0x02;
		bx = button_data[0] & 0x04;
		ba = button_data[0] & 0x01;

		bp = button_data[0] & 0x10;

		bl = button_data[1] & 0x40;
		br = button_data[1] & 0x20;

		bzl = bzr = button_data[1] & 0x10;

		bhome = (bdu && bp); // UP + START == HOME

		_lx = button_data[2]/4;
		_ly = button_data[3]/4;
		_rx = button_data[4]/8;
		_ry = button_data[5]/8;

		if(_lx >= (center_lx - ANALOG_NEUTRAL_RADIUS) && _lx <= (center_lx + ANALOG_NEUTRAL_RADIUS)) {
			_lx = clx;
		}

		if(_ly >= (center_ly - ANALOG_NEUTRAL_RADIUS) && _ly <= (center_ly + ANALOG_NEUTRAL_RADIUS)) {
			_ly = cly;
		}

		if(_rx >= (center_rx - ANALOG_NEUTRAL_RADIUS) && _rx <= (center_rx + ANALOG_NEUTRAL_RADIUS)) {
			_rx = crx;
		}

		if(_ry >= (center_ry - ANALOG_NEUTRAL_RADIUS) && _ry <= (center_ry + ANALOG_NEUTRAL_RADIUS)) {
			_ry = cry;
		}

		lx = _lx;
		ly = _ly;
		rx = _rx;
		ry = _ry;

		WMExtension::set_button_data(bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
				bm, bp, bhome, lx, ly, rx, ry, bzl, bzr);
	}
}

void n64_loop() {
	byte *button_data;
	bool swap_l_z = false;

	byte center_lx, center_ly;
	byte _lx, _ly, _rx, _ry;

	byte clx = WMExtension::get_calibration_byte(2)>>2;
	byte cly = WMExtension::get_calibration_byte(5)>>2;
	byte crx = WMExtension::get_calibration_byte(8)>>3;
	byte cry = WMExtension::get_calibration_byte(11)>>3;

	GCPad_init();

	button_data = N64Pad_read();

	center_lx = ((button_data[2] >= 128) ? button_data[2] - 128 : button_data[2] + 128) / 4;
	center_ly = ((button_data[3] >= 128) ? button_data[3] - 128 : button_data[3] + 128) / 4;

	// If plugged in with L pressed, L and Z buttons will be swapped (for Zelda games' sake!)
	if (button_data[1] & 0x20) {
		swap_l_z = true;
	}

	for(;;) {
		button_data = N64Pad_read();

		bdl = button_data[0] & 0x02;
		bdr = button_data[0] & 0x01;
		bdu = button_data[0] & 0x08;
		bdd = button_data[0] & 0x04;

		bb = button_data[0] & 0x40;
		ba = button_data[0] & 0x80;

		bp = button_data[0] & 0x10;

		br = button_data[1] & 0x10;

		if (!swap_l_z) {
			bl = button_data[1] & 0x20;
			bzl = bzr = button_data[0] & 0x20;
		} else {
			bl = button_data[0] & 0x20;
			bzl = bzr = button_data[1] & 0x20;
		}

		bhome = (bdu && bp); // UP + START == HOME

		_ry = cry;

		if(button_data[1] & 0x08) { // C Up
			_ry = 30;
		} else if(button_data[1] & 0x04) { // C Down
			_ry = 1;
		}

		_rx = crx;

		if(button_data[1] & 0x02) { // C Left
			_rx = 1;
		} else if(button_data[1] & 0x01) { // C Right
			_rx = 30;
		}

		by = button_data[1] & 0x02; // Y == C Left
		bx = button_data[1] & 0x01; // X == C Right

		_lx = ((button_data[2] >= 128) ? button_data[2] - 128 : button_data[2] + 128) / 4;;
		_ly = ((button_data[3] >= 128) ? button_data[3] - 128 : button_data[3] + 128) / 4;;

		if(_lx >= (center_lx - ANALOG_NEUTRAL_RADIUS) && _lx <= (center_lx + ANALOG_NEUTRAL_RADIUS)) {
			_lx = clx;
		}

		if(_ly >= (center_ly - ANALOG_NEUTRAL_RADIUS) && _ly <= (center_ly + ANALOG_NEUTRAL_RADIUS)) {
			_ly = cly;
		}

		lx = _lx;
		ly = _ly;
		rx = _rx;
		ry = _ry;

		WMExtension::set_button_data(bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
				bm, bp, bhome, lx, ly, rx, ry, bzl, bzr);
	}
}

void neogeo_loop() {
	int button_data;

	NESPad::init();

	for (;;) {
		button_data = NESPad::read(16);

		bdl = button_data & 0x02;
		bdr = button_data & 0x800;
		bdu = button_data & 0x03;
		bdd = button_data & 0x1000;
		bb = button_data & 0x01;
		by = button_data & 0x8000;
		bm = button_data & 0x100;
		bp = button_data & 0x4000;
		ba = button_data & 0x400;
		bx = button_data & 0x200; // D button is also 0x2000
		bhome = (bm && bp); // SELECT + START == HOME

		WMExtension::set_button_data(bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
				bm, bp, bhome, lx, ly, rx, ry, bzl, bzr);
	}
}

void saturn_loop() {
	int button_data;

	saturn_init();

	for (;;) {
		button_data = saturn_read();

		bdl = button_data & SATURN_LEFT;
		bdr = button_data & SATURN_RIGHT;
		bdu = button_data & SATURN_UP;
		bdd = button_data & SATURN_DOWN;
		ba = button_data & SATURN_C;
		bb = button_data & SATURN_B;
		bx = button_data & SATURN_Y;
		by = button_data & SATURN_A;
		bl = button_data & SATURN_X;
		br = button_data & SATURN_Z;

		bzl = button_data & SATURN_L;
		bzr = button_data & SATURN_R;

		bp = button_data & SATURN_START;

		bhome = (bdu && bp); // UP + START == HOME

		WMExtension::set_button_data(bdl, bdr, bdu, bdd, ba, bb, bx, by, bl, br,
				bm, bp, bhome, lx, ly, rx, ry, bzl, bzr);
	}
}


void setup() {
	// Set pad detection pins as input, turning pull-ups on
	pinModeFast(DETPIN1, INPUT);
	digitalWriteFast(DETPIN1, HIGH);

	pinModeFast(DETPIN2, INPUT);
	digitalWriteFast(DETPIN2, HIGH);

	pinModeFast(DETPIN3, INPUT);
	digitalWriteFast(DETPIN3, HIGH);

	// Prepare wiimote communications
	WMExtension::init();
}

void loop() {
	// Select pad loop based on pad auto-detection routine. Genesis pad is the default.
	switch (detectPad()) {
	case PAD_NES:
		nes_loop();
		break;
	case PAD_SNES:
		snes_loop();
		break;
	case PAD_PS2:
		ps2_loop();
		break;
	case PAD_GC:
		gc_loop();
		break;
	case PAD_N64:
		n64_loop();
		break;
	case PAD_NEOGEO:
		neogeo_loop();
		break;
	default:
#if SATURN == 1
		saturn_loop();
#else
		genesis_loop();
#endif
		break;
	}
}
