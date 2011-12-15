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
#include "GCPad.h"
#include "digitalWriteFast.h"

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

	byte center_lx, center_ly;
	byte _lx, _ly, _rx, _ry;

	byte clx = WMExtension::get_calibration_byte(2)>>2;
	byte cly = WMExtension::get_calibration_byte(5)>>2;
	byte crx = WMExtension::get_calibration_byte(8)>>3;
	byte cry = WMExtension::get_calibration_byte(11)>>3;

	GCPad_init();

	button_data = N64Pad_read();

	center_lx = button_data[2]/4;
	center_ly = button_data[3]/4;

	for(;;) {
		button_data = N64Pad_read();

		bdl = button_data[0] & 0x40;
		bdr = button_data[0] & 0x80;
		bdu = button_data[0] & 0x10;
		bdd = button_data[0] & 0x20;

		bb = button_data[0] & 0x02;
		ba = button_data[0] & 0x01;

		bp = button_data[0] & 0x08;

		bl = button_data[1] & 0x04;
		br = button_data[1] & 0x08;

		bzl = bzr = button_data[0] & 0x04;

		bhome = (bdu && bp); // UP + START == HOME

		_ry = button_data[1] & 0x10 ? 1 : cry; // C Up
		_ry = button_data[1] & 0x20 ? 31 : cry; // C Down

		_rx = button_data[1] & 0x40 ? 1 : crx; // C Left
		_rx = button_data[1] & 0x80 ? 31 : crx; // C Right

		by = button_data[1] & 0x40; // Y == C Left
		bx = button_data[1] & 0x80; // X == C Right

		_lx = button_data[2]/4;
		_ly = button_data[3]/4;

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

	// Select pad loop based on pad auto-detection routine. Genesis pad is the default.
	switch (detectPad()) {
	case PAD_NES:
		pad_loop = nes_loop;
		break;
	case PAD_SNES:
		pad_loop = snes_loop;
		break;
	case PAD_PS2:
		pad_loop = ps2_loop;
		break;
	case PAD_GC:
		pad_loop = gc_loop;
		break;
	case PAD_N64:
		pad_loop = n64_loop;
		break;
	default:
		pad_loop = genesis_loop;
		break;
	}
}

void loop() {
	if (pad_loop) {
		pad_loop();
	}
}
